# t-utc/slimpl.m4: this file is part of the SL toolchain.
# 
# Copyright (C) 2008,2009,2010 The SL project
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# The complete GNU General Public Licence Notice can be found as the
# `COPYING' file in the root directory.
#
#
# ###############################################
#  Macro definitions for the new muTC syntax
# ###############################################

# Pass transparently thread definitions.
m4_define([[sl_def]],[[m4_dnl
m4_define([[sl_thparms]],[[m4_shiftn(2,$@)]])m4_dnl
thread [[$2]] [[$1]]m4_if((sl_thparms),(),(void),(sl_thparms))m4_dnl
]])

# No special action at the end of a definition
m4_define([[sl_enddef]],[[]])

# With the new syntax, a declaration looks the same as a definition.
m4_define([[sl_decl]], m4_defn([[sl_def]]))
m4_define([[sl_decl_fptr]], [[sl_decl((*[[$1]]), m4_shift($@))]])

# Pass transparently parameter declarations.
m4_define([[sl_shparm]], [[ shared [[$1]] [[$2]]]])
m4_define([[sl_glparm]], [[ [[$1]] [[$2]]]])
m4_define([[sl_glparm_mutable]], [[m4_dnl
m4_error([[sl_glparm_mutable not implemented yet for this target]])]])
m4_copy([[sl_shparm]],[[sl_shfparm]])
m4_copy([[sl_glparm]],[[sl_glfparm]])
m4_copy([[sl_glparm_mutable]],[[sl_glfparm_mutable]])


# Pass transparently the index declaration.
m4_define([[sl_index]], [[index [[$1]]]])

# Pass transparently the create statement; auto-generate
# break variable names.
m4_define([[sl_create]], [[m4_dnl
m4_define([[_sl_brk]],m4_if(sl_breakable([[$7]]),1,[[[[$1]]_brk]],))m4_dnl
create([[$1]]; [[$2]]; [[$3]]; [[$4]]; [[$5]]; [[$6]]; _sl_brk)m4_dnl
 [[$8]](m4_shiftn(8,$@))m4_dnl
]])


# Pass transparently shared and global argument declarations.
m4_define([[sl_sharg]],[[m4_dnl
 [[$2]]m4_ifblank([[$3]],,[[ = $3]]) m4_dnl
]])
m4_define([[sl_glarg]],m4_defn([[sl_sharg]]))
m4_copy([[sl_sharg]],[[sl_shfarg]])
m4_copy([[sl_glarg]],[[sl_glfarg]])

# Pass transparently the sync construct.
m4_define([[sl_sync]],[[m4_dnl
m4_ifblank([[$2]],,[[$2 = ]])sync([[$1]])m4_dnl
]])

# Pass transparently all references to argument/parameter
# names.
m4_define([[sl_geta]],[[$1]])
m4_define([[sl_seta]],[[$1 = $2]])
m4_define([[sl_getp]],[[$1]])
m4_define([[sl_setp]],[[$1 = $2]])

# Pass transparently break and kill
m4_define([[sl_break]],[[break ($1)]])
m4_define([[sl_kill]],[[kill($1)]])

# Pass transparently sl_getfid
m4_define([[sl_getfid]],[[$1]])

# Pass transparently sl_getbr
m4_define([[sl_getbr]],[[$1]]_brk)

# Spawn stuff
m4_define([[sl_spawndecl]], [[m4_dnl
[[long $1 __attribute__((unused))]]m4_dnl
]])
m4_define([[sl_spawnsync]], [[(void)0]])

m4_define([[sl_spawn]], [[m4_dnl
do { sl_create($@); sl_sync([[$1]]); } while(0)m4_dnl
]])


# ## End macros for new muTC syntax ###
