//
//  config.h
//  f_decomp
//
//  Created by Mikhail Kochegarov on 5/24/13.
//  Copyright (c) 2013 Mikhail Kochegarov. All rights reserved.
//

#ifndef __f_decomp__config__
#define __f_decomp__config__

#include <iostream>

#define SIGN_OBJECT_CONFIG 0xFEBAFEBA
#define PATH_DELIMITER '/'

struct ConfigObject {
    // + 0
    uint32_t signature = 0;
    // + 4
    uint32_t children_count = 0;
    // + 8
    uint32_t children_list_size = 0;
    // + C
    uint32_t child_size = 0;
    // + 10
    bool (*read)(void*, FILE*, int) = nullptr;
    // + 14
    bool (*save)(void*, FILE*, int) = nullptr;
    // + 18
    int32_t val1 = 0;
    // + 1C
    int32_t val2 = 0;
    // + 20
    void* parent = nullptr;
    
    //+ 24 children is key-value pairs sorted by keys, keys as links to strings
    void** children = nullptr;
};

struct SetupObject {
    bool (*read)(void*, FILE*, int);
    bool (*save)(void*, FILE*, int);
    int32_t val1;
    int32_t val2;
    void* parent;
};

class Config {

private:
    static void* (*create_object)(size_t);
    
    static void* (*realloc_object)(void*, unsigned long);
    
    static void (*delete_object)(void*);
    
    static bool resize_config(ConfigObject* config, uint32_t new_size);
    
    static bool find_keyid(ConfigObject* config, const char* key, int* index);
    
    static void sub_4C6020();
    
    static void sub_4C6080();
    
    static bool read_little_endian_int(FILE* fp, uint32_t* result);
    
    static bool read_header(FILE* fp, ConfigObject* config);
    
    static bool write_little_endian_int(FILE* fp, int32_t value);
    
    static bool write_header(FILE* fp, ConfigObject* config);
    
    static bool write(FILE* fp, ConfigObject* parent);
    
public:
    static bool create_config(ConfigObject* config_obj, uint32_t children_list_size, uint32_t child_size, SetupObject* setup);

    static bool find_child(void* config, const char* key, int* index);

    static void set_functions(void* c(size_t), void* r(void*, unsigned long), void d(void*));

    static bool create_child(ConfigObject* config, const char* key, void* value);
    
    static bool read(FILE* fp, int val0, ConfigObject* config);
    
    static bool free_config(ConfigObject* config);
    
    static bool delete_child(ConfigObject* config, const char* key);
        
};
#endif /* defined(__f_decomp__config__) */
