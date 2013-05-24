//
//  memory.cpp
//  f_decomp
//
//  Created by Mikhail Kochegarov on 5/14/13.
//  Copyright (c) 2013 Mikhail Kochegarov. All rights reserved.
//

#include "memory.h"

unsigned int Memory::current_count_of_objects = 0;
unsigned int Memory::max_count_of_objects = 0;
unsigned int Memory::current_size_of_objects = 0;
unsigned int Memory::max_size_of_objects = 0;

void* (*Memory::create_object_func)(size_t) = create_object;
void* (*Memory::realloc_object_func)(void*, unsigned long) = realloc_object;
void (*Memory::delete_object_func)(void*) = delete_object;

char* Memory::create_object_with_value(const char* value) {
    
    uint32_t size_t = (uint32_t)strlen(value);
    if (size_t == 0) size_t = 1;
    
    //create object for string with trailing \0
    size_t++;
    void* result = create_object_func(size_t);
    
    memcpy(result, value, size_t);
    return (char*)result;
}

void* Memory::run_create_object_func(size_t n_size) {
    return create_object_func(n_size);
}

void* Memory::create_object(size_t n_size) {
    void* result = nullptr;
    
    if (n_size) {
        uint32_t full_size = (uint32_t)(n_size + OBJECT_OVERHEAD);
        result = malloc(full_size);
        if (result) {
            ((uint32_t*)result)[0] = full_size;
            ((uint32_t*)((char*)result + OBJECT_HEADER_SIZE - sizeof(SIGN_OBJECT_START)))[0] = SIGN_OBJECT_START;
            
            current_count_of_objects++;
            if (current_count_of_objects > max_count_of_objects) {
                max_count_of_objects = current_count_of_objects;
            }
            
            current_size_of_objects += full_size;
            if (current_size_of_objects > max_size_of_objects) {
                max_size_of_objects = current_size_of_objects;
            }
            
            ((uint32_t*)((char*)result + full_size - sizeof(SIGN_OBJECT_END)))[0] = SIGN_OBJECT_END;
            result = (void*)((char*)result + OBJECT_HEADER_SIZE);
        }
    }    
    return result;
}

void* Memory::run_realloc_object_func(void* obj, unsigned long n_size) {
    return realloc_object_func(obj, n_size);
}

void* Memory::realloc_object(void* obj, unsigned long n_size) {
    if (n_size <= 0) return obj;
    if (!obj) {
        return create_object_func((int)n_size);
    }
    
    obj = (void*)((char*) obj - OBJECT_HEADER_SIZE);

    
    uint32_t size = ((uint32_t*)obj)[0];
    current_size_of_objects -= size;
    
    if (!check_object(obj)) return nullptr;

    uint32_t full_size = (uint32_t)(n_size + OBJECT_OVERHEAD);
    obj = realloc((char*)obj, full_size);
    if (!obj) {
        current_count_of_objects--;
        Debug::debug_info("(%s,%u): ", "memory.c", 195);
        Debug::debug_info("Realloc failure.\n");
        return nullptr;
    }
    
    current_size_of_objects += full_size;
    if (current_size_of_objects > max_size_of_objects) {
        max_size_of_objects = current_size_of_objects;
    }
    
    ((uint32_t*)obj)[0] = full_size;
    ((uint32_t*)((char*)obj + OBJECT_HEADER_SIZE - sizeof(SIGN_OBJECT_START)))[0] = SIGN_OBJECT_START;
    ((uint32_t*)((char*)obj + full_size - sizeof(SIGN_OBJECT_END)))[0] = SIGN_OBJECT_END;
    return (void*)((char*)obj + OBJECT_HEADER_SIZE);
}

void Memory::run_delete_object_func(void* obj) {
    delete_object_func(obj);
}

void Memory::delete_object(void* obj) {
    if (obj) {
        obj = (void*)((char*) obj - OBJECT_HEADER_SIZE);
        
        if (check_object(obj)) {
            current_size_of_objects -= ((uint32_t*)obj)[0];
            free(obj);
            current_count_of_objects--;
        }
    }
}

void Memory::debug_print() {
    if (create_object_func == create_object) {
        Debug::debug_info("Current memory allocated: %6d blocks, %9u bytes total\n", current_count_of_objects, current_size_of_objects);
        Debug::debug_info("Max memory allocated:     %6d blocks, %9u bytes total\n", max_count_of_objects, max_size_of_objects);
    }
}

void Memory::set_object_functions(void* create_func(size_t), void* realloc_func(void*, unsigned long), void delete_func(void*)) {
    create_object_func = create_func;
    realloc_object_func = realloc_func;
    delete_object_func = delete_func;
}

void Memory::convert_to_object(void* obj, uint32_t size) {
    ((uint32_t*)obj)[0] = size;
    ((uint32_t*)((char*)obj + sizeof(uint32_t)))[0] = SIGN_OBJECT_START;
    ((uint32_t*)((char*)obj + size - sizeof(SIGN_OBJECT_END)))[0] = SIGN_OBJECT_END;
}

bool Memory::check_object(void* obj) {
    uint32_t size = ((uint32_t*)obj)[0];
    
    if (((uint32_t*)obj)[1] != SIGN_OBJECT_START) {
        Debug::debug_info("Memory header stomped.\n");
        return false;
    }
    if (((uint32_t*)((char*)obj + size - sizeof(SIGN_OBJECT_END)))[0] != SIGN_OBJECT_END) {
        Debug::debug_info("Memory footer stomped.\n");
        return false;
    }
    return true;
}

