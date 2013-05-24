//
//  memory.h
//  f_decomp
//
//  Created by Mikhail Kochegarov on 5/14/13.
//  Copyright (c) 2013 Mikhail Kochegarov. All rights reserved.
//

#ifndef __f_decomp__memory__
#define __f_decomp__memory__

#include <iostream>

#include "debug.h"

#define SIGN_OBJECT_START   0xFEEDFACE
#define SIGN_OBJECT_END     0xBEEFCAFE
#define OBJECT_HEADER_SIZE  (sizeof(uint32_t) + sizeof(SIGN_OBJECT_END))
// overhead = size, header sign, footer sign
#define OBJECT_OVERHEAD (OBJECT_HEADER_SIZE + sizeof(SIGN_OBJECT_END))


// from BEGTEXT:004AEBDB till BEGTEXT:004AEE89
class Memory {
private:
    
    // DGROUP:00539D18
    static void* (*create_object_func)(size_t);
    // DGROUP:00539D1C
    static void* (*realloc_object_func)(void*, unsigned long);
    // DGROUP:00539D20
    static void (*delete_object_func)(void*);
    // DGROUP:00539D24
    static uint32_t current_count_of_objects;
    // DGROUP:00539D28
    static uint32_t max_count_of_objects;
    // DGROUP:00539D2C
    static uint32_t current_size_of_objects;
    // DGROUP:00539D30
    static uint32_t max_size_of_objects;

    // BEGTEXT:004AEC38
    static void* create_object(size_t n_size);

    // BEGTEXT:004AECB8
    static void* realloc_object(void* obj, unsigned long n_size);
    
    // BEGTEXT:004AED8C
    static void delete_object(void* obj);

public:
        
    // BEGTEXT:004AEBE0
    static char* create_object_with_value(const char* value);
    
    // BEGTEXT:004AEC30
    static void* run_create_object_func(size_t n_size);

    // BEGTEXT:004AECB0
    static void* run_realloc_object_func(void* obj, unsigned long n_size);
    
    // BEGTEXT:004AED84
    static void run_delete_object_func(void* obj);
    
    // BEGTEXT:004AEDBC
    static void debug_print();
    
    // BEGTEXT:004AEE08
    static void set_object_functions(void* (size_t), void* (void*, unsigned long), void (void*));

    // BEGTEXT:004AEE24
    static void convert_to_object(void* obj, uint32_t size);
    
    // BEGTEXT:004AEE44
    static bool check_object(void* obj);
    
};

#endif /* defined(__f_decomp__memory__) */