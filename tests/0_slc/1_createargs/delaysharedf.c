//
// delaysharedf.c: this file is part of the SL toolchain.
//
// Copyright (C) 2009-2015 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#include <svp/compiler.h>
#include <svp/testoutput.h>

sl_def(foo, void, sl_shfparm(double, a))
{
  sl_setp(a, sl_getp(a) + 1.0);
}
sl_enddef

// 2009-04-02: FIXME: we acknowledge that muTC-ptl
// does not support this construct fully yet; but
// we want slc's testsuite to properly succeed. So
// we mark the test to ignore the output on muTC-ptl:
// XIGNORE: ptl*:D

// 2015-07-23: MIPSel does not support FP yet.
// XIGNORE: mips*:R

sl_def(t_main, void)
{
  int busy;
  sl_create(,,,,,,,
	    foo, sl_shfarg(double, a));
  // FIXME: for ptl here we should find a way to
  // force a context switch, otherwise the problem
  // is not demonstrated. In the previous version
  // of this test, a call was done to the C library's
  // putc() function, but we can't do this in SL
  // because putc is a thread function and we cannot
  // nest creates.
  for (busy = 0; busy < 10000; ++busy) nop();
  sl_seta(a, 42.0);
  sl_sync();

  output_float(sl_geta(a), 1, 2);
  output_char('\n', 1);
}
sl_enddef
