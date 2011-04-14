#! /usr/bin/env python
# This scripts translates "formats.db" to a python database
# (dictionary) of regular expression parsers and instruction
# attributes.

# The input database is tab-delimited and should contain the following fields:
# mnemonic     format    attributes
# the attributes can be either empty or be set to a comma-delimited combination of:
# "ll" (long latency) 
# "dl" (delayed)
# "o:N" (outputs to phy register N)
# "i:N" (inputs from phy register N)

formats = \
"""
   
# \d+
+ [+-]
, ,
0 imm64
1 $rs1
2 $rs2
3 $rs3
4 $rsx
5 $fsx
6 %fcc0
7 %fcc1
8 %fcc2
9 %fcc3
> imm4
A #asi
B $fs2
C %csr
D %c\d+
E %ccr
F %fsr
G imm19rel
H $fd
I imm11
J $fd
L imm30rel
M %asr\d+
N pn
O $rs2d
P %pc
Q %cq
R $fs2
S (?:)
T pt
U $fdx
V $fs1
W %tick
X imm13
Y imm13
Z %xcc
[ \[
] \]
^ imm9
a a
b %c\d+
c %c\d+
d $rd
e $fs1
f $fs2
g $fd
h imm22
i imm13
j imm10
k imm16
l imm22rel
m %asr
n imm22
o %asi
p %psr
q %fq
r $rs1d
s %fprs
t %tbr
u $rdx
v $fs1
w %wim
x #line
y %y
z %icc
"""

### Step 1: transform the symbol list above to a dictionary. ##

symbolic_formats = {}
for f in formats.split('\n'):
    f = f.strip()
    if not f: continue
    
    k = f[0]
    v = f[2:]

    if k in symbolic_formats:
        print "Duplicate key:", k
    symbolic_formats[k] = v

format_symbols = {}

### Step 2: enrich the translation dict with attributes ### 

tr = ''.join(('%c' % i for i in xrange(256)))
for k, v in symbolic_formats.items():

    # delete all digits from the format expansion
    v = v.translate(tr, '0123456789')

    # detect format properties
    type = None
    mode = ''
    double = False
    if v.startswith('$f'):
        type = 'f'
        if 's' in v:
            mode += 'r'
        if 'd' in v:
            mode += 'w'
        if 'x' in v:
            double = True
    elif v.startswith('$r'):
        type = 'i'
        if 's' in v:
            mode += 'r'
        if 'd' in v:
            mode += 'w'
        if 'x' in v:
            double = True
    elif v.startswith('imm'):
        type = 'v'

    # compute canonical form
    v = v.replace('$f', '$r').replace('$rs','$r').replace('$rd','$r').replace('$rx','$r')
    v = v.replace('$r',"''' + reg + r'''")
    v = v.replace('immrel', 'imm')
    v = v.replace('imm', "''' + imm + r'''")
    if v[0] in '%#':
        v = v + '\S*'

    format_symbols[k] = (v, type, mode, double)

### Step 3: read in the instruction database and expand format ###
import sys

insnprops = {}
f = file(sys.argv[1])
for insn in f:
    insn = insn.strip()
    if not insn: continue
    insn = insn + '\t\t'
    name, fmt, attrs = insn.split('\t')[:3]

    name = name.strip()
    fmt = fmt.strip()
    attrs = [x.strip().lower() for x in attrs.split(',')]
    

    reparser = []
    ins = []
    outs = []
    dbls = []
    imm = []
    i = 0
    for char in fmt:
        re, ty, mo, db = format_symbols[char]

        reparser.append(re)
        if 'r' in mo:
            ins.append(i)
        if 'w' in mo:
            outs.append(i)
        if db:
            dbls.append(i)
        if ty == 'v':
            imm.append(i)
        if ty is not None:
            i += 1

    reparser = '\s*'.join(reparser)
    if reparser:
        reparser = "\s*%s\s*" % reparser
    else:
        reparser = "\s*"
    reparser += '$'

    ll = 'll' in attrs
    dl = 'dl' in attrs
    isbr = 'br' in attrs
    iscbr = 'cbr' in attrs
    ei = []
    eo = []
    for x in attrs:
        if x.startswith('i:'):
            ei.append(eval(x[2:]))
        elif x.startswith('o:'):
            eo.append(eval(x[2:]))

    if name not in insnprops:
        insnprops[name] = []

    insnprops[name].append((reparser, (ins, outs, dbls, ll, dl, ei, eo, imm, isbr, iscbr)))

### Step 4: factor the regular expressions ###

regexs = {}
pos = 0
for k,l in insnprops.items():
    for i,v in enumerate(l):
      re = v[0]
      if re not in regexs:
          regexs[re] = (pos,'re%03d'%pos)
          pos += 1
      (idx,alias) = regexs[re]
      l[i] = (alias, v[1])
regexs_inv = {}
for k,v in regexs.items():
    idx,alias = v
    assert idx not in regexs_inv
    regexs_inv[idx] = (k, alias)

### Step 5: factor the instruction forms ###

forms = {}
pos = 0
for k,l in insnprops.items():
    for i,v in enumerate(l):
      f = repr(v[1])
      if f not in forms:
          forms[f] = (pos,'form%03d'%pos)
          pos += 1
      (idx,alias) = forms[f]
      l[i] = (v[0], alias)
forms_inv = {}
for k,v in forms.items():
    idx,alias = v
    assert idx not in forms_inv
    forms_inv[idx] = (k, alias)


### Step 6: Generate the output python code #### 

# parser for a register
regref = r'(\$[lgsd]?f?\d+)'
# parser for an immediate, recognizes %hi(name) and the like
immref = r'([^%$]\w*|%(?:(?:hi|lo)x?|hh|hm|lm|h44|m44|uhi|ulo|(?:tgd|tldm|tie)_(?:hi22|lo10)|(?:tldo|tle)_(?:hix22|lox10))\([^)]+\))'

print """# This file was generated by decode.py. Do not edit!
# For each instruction the information available is:'
# re_parser, input_regs, output_regs, double_regs, long_latency, delayed, extra_phy_inputs, extra_phy_outputs'
import re
class insn_metadata(object):
    def __init__(self, info):
        self.inputs, self.outputs, self.double_regs, self.long_latency, self.delayed, self.extra_inputs, self.extra_outputs, self.immediates, self.is_branch, self.is_condbranch = info
"""
print 'reg = r"""' + regref + '"""'
print 'imm = r"""' + immref + '"""' 

l = regexs_inv.keys()
l.sort()
for idx in l:
    re,alias = regexs_inv[idx]
    print "%s = re.compile(r'''%s''')" % (alias,re)

l = forms_inv.keys()
l.sort()
for idx in l:
    f,alias = forms_inv[idx]
    print "%s = insn_metadata(%s)" % (alias,f)

print 'insninfo = {'

l = insnprops.keys()
l.sort()

for k in l:
    v = insnprops[k]
    print "'%s' : [" % k
    for item in v:
        print "      (%s, %s)," % item
    print '      ],'

print '}'


