//
//  debug.cpp
//  f_decomp
//
//  Created by Mikhail Kochegarov on 5/14/13.
//  Copyright (c) 2013 Mikhail Kochegarov. All rights reserved.
//

#include "debug.h"

void Debug::debug_info(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf(fmt, args);
}