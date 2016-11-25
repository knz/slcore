//
// mgsim.c: this file is part of the SL toolchain.
//
// Copyright (C) 2011 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#if defined(__slc_os_sim__)
#include <svp/mgsim.h>

#undef mgsim_control
void mgsim_control(unsigned long val, unsigned int type, unsigned int command, unsigned int flags)
{
    __inline_mgsim_control(val, type, command, flags);
}

#endif
