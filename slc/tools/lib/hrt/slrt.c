//
// slrt.c: this file is part of the SL toolchain.
//
// Copyright (C) 2010 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#include <svp/delegate.h>

sl_place_t __main_place_id = PLACE_DEFAULT;

const char *__tag__ = "\0slr_runner:host:";
const char *__datatag__ = "\0slr_datatag:hrt_n-host-host-hrt:";

#include "../load.c"

