//
// crif.c: this file is part of the SL toolchain.
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

int e;

sl_def(foo, void) {} sl_enddef

int main(void)
{
  sl_create(,,,,,,, foo);
  sl_sync();
  if (e == 0) {
      e = e + 1;
  }
  return e - 1;
}

