//
// globpass.c: this file is part of the SL toolchain.
//
// Copyright (C) 2009 The SL project.
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

sl_def(print_glob, void, sl_glparm(int, num))
{
  if (sl_getp(num) > 10)
    nop();
}
sl_enddef

int main(void)
{
  sl_create(,,,9,,,, print_glob, sl_glarg(int, g, 5));
  sl_sync();
  return 0;
}

