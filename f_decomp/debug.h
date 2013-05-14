//
//  debug.h
//  f_decomp
//
//  Created by Mikhail Kochegarov on 5/14/13.
//  Copyright (c) 2013 Mikhail Kochegarov. All rights reserved.
//

#ifndef __f_decomp__debug__
#define __f_decomp__debug__

#include <iostream>

class Debug {
public:
    static void debug_info(const char* fmt, ...);
};

#endif /* defined(__f_decomp__debug__) */
