//
//  config.cpp
//  f_decomp
//
//  Created by Mikhail Kochegarov on 5/24/13.
//  Copyright (c) 2013 Mikhail Kochegarov. All rights reserved.
//

#include "config.h"

// DGROUP:0053A25C
void* (*Config::create_object)(size_t) = malloc;

// DGROUP:0053A260
void* (*Config::realloc_object)(void*, unsigned long) = realloc;

// DGROUP:0053A264
void (*Config::delete_object)(void*) = free;


// BEGTEXT:004C5C1E
// CHP func

// BEGTEXT:004C5C40
// malloc_func

// BEGTEXT:004C5C48
// realloc_func

// BEGTEXT:004C5C50
// free_func

// BEGTEXT:004C5C58
// ebx = child_size, ecx = parent, eax = config_obj, edx = children_list_size
bool Config::create_config(ConfigObject* config_obj, uint32_t children_list_size, uint32_t child_size, SetupObject* setup) {
    bool result = true;
    config_obj->children_count = 0;
    config_obj->children_list_size = children_list_size;
    config_obj->child_size = child_size;
    if (setup) {
        config_obj->read = setup->read;
        config_obj->save = setup->save;
        config_obj->val1 = setup->val1;
        config_obj->val2 = setup->val2;
        config_obj->parent = setup->parent;
    }
    
    if (config_obj->children_list_size > 0) {
        config_obj->children = (void**)create_object(config_obj->children_list_size * (sizeof(void*) * 2));
        if (!config_obj->children) result = false;
    } else {
        config_obj->children = nullptr;
    }
    
    if (result) {
        config_obj->signature = SIGN_OBJECT_CONFIG;
    }
    
    return result;
}

// BEGTEXT:004C5CBC
bool Config::resize_config(ConfigObject* config, uint32_t new_size) {
    if (config->signature != SIGN_OBJECT_CONFIG) return false;
    
    if (new_size <= config->children_count) return false;
    
    void** new_obj = (void**)Config::realloc_object(config->children, new_size * sizeof(void*) * 2);
    if (new_obj) {
        config->children = new_obj;
        config->children_list_size = new_size;
        return true;
    }
    
    return false;
}

// BEGTEXT:004C5CF8
bool Config::free_config(ConfigObject* config) {
    if (config->signature != SIGN_OBJECT_CONFIG) return false;

    for(int i=0; i < config->children_count; i++) {
        if (config->children[i*2])
            delete_object(config->children[i*2]);
        
        if (config->children[i*2+1])
            delete_object(config->children[i*2+1]);
    }
    if (config->children)
        delete_object(config->children);
    memset(config, sizeof(ConfigObject), 0);
    return true;
}

// BEGTEXT:004C5D74
bool Config::find_keyid(ConfigObject* config, const char* key, int* index) {
    
    if (config->signature != SIGN_OBJECT_CONFIG) return false;
    
    if (config->children_count == 0) {
        return false;
    }
    
    int start_index = 0;
    int end_index = config->children_count - 1;
    
    // Binary search
    int spot = 0;
    while (end_index >= start_index) {
        *index = (end_index + start_index) / 2;
        char* compare_string = (char*)(config->children[*index * 2]);
        spot = strcmp(key, compare_string);
        
        if (spot == 0) {
            return true;
        } else if (spot > 0) {
            start_index = *index + 1;
            
        } else {
            end_index = *index - 1;
        }
    }
    if (spot > 0) {
        *index += 1;
    }
    
    return false;
}

// BEGTEXT:004C5DF8
bool Config::find_child(void* config, const char* key, int* index) {
    if (((uint32_t*)config)[0] == SIGN_OBJECT_CONFIG) {
        ConfigObject* conf_object = (ConfigObject*)config;
        
        return find_keyid(conf_object, key, index);
    }
    return false;
}

// BEGTEXT:004C5E24
bool Config::create_child(ConfigObject* config, const char* key, void* value) {
    
    if (config->signature != SIGN_OBJECT_CONFIG) return false;
    
    int id = 0;
    
    if (find_keyid(config, key, &id)) return false;
    
    if (config->children_count == config->children_list_size) {
        resize_config(config, config->children_list_size + 1);
    }
    
    void* key_object = create_object(strlen(key) + 1);
    strcpy((char*)key_object, key);
    
    if (value) {
        void* obj;
        if (config->child_size == 0) {
            obj = nullptr;
        } else {
            obj = create_object(config->child_size);
            
            if (!obj) {
                delete_object(key_object);
            }
        }
        
        if (obj) {
            if (config->child_size > 0) {
                memcpy(obj, value, config->child_size);
            }
        }
        
        if (id < config->children_count) {
            int last_index = config->children_count-1;
            
            while (last_index >= id) {
                config->children[(last_index+1) * 2] = config->children[last_index * 2];
                config->children[(last_index+1) * 2 + 1] = config->children[last_index * 2 + 1];
                config->children[last_index * 2] = nullptr;
                config->children[last_index * 2 + 1] = nullptr;
                last_index --;
            }
        }
        
        config->children[id * 2] = (void*)key_object;
        config->children[id * 2 + 1] = value;
        
        config->children_count++;
        
    }
    return true;
}

// BEGTEXT:004C5F84
bool Config::delete_child(ConfigObject* config, const char* key) {
    int id = 0;
    if (config->signature != SIGN_OBJECT_CONFIG || !find_keyid(config, key, &id)) {
        return false;
    }
    
    // delete key object
    delete_object(config->children[id * 2]);
    // delete object value
    if (config->children[id * 2 + 1]) {
        delete_object(config->children[id * 2 + 1]);
    }
    
    config->children_count -=1;
    
    while (id < config->children_count) {
        config->children[id * 2] = config->children[(id+1) * 2];
        config->children[id * 2 + 1] = config->children[(id+1) * 2 + 1];
        id++;
    }
    //there is no such code in original, but it is very handy for debug process
    config->children[id * 2] = nullptr;
    config->children[id * 2 + 1] = nullptr;
    return true;
}

// BEGTEXT:004C6020
void Config::sub_4C6020() {
    //Not used function.
}

// BEGTEXT:004C6080
void Config::sub_4C6080() {
    //Not used function
}

// BEGTEXT:004C612C
bool Config::read_little_endian_int(FILE* fp, uint32_t* result) {
    unsigned char bytes[4];
    *result = 0;
    
    //original done by fgetc, but fread is a little bit efficiant
    if (fread(bytes, 4, 1,fp) != 0) {
        *result = bytes[3] | (bytes[2]<<8) | (bytes[1]<<16) | (bytes[0]<<24);
        return true;
    }
    
    return false;
}

// BEGTEXT:004C6190
bool Config::read_header(FILE* fp, ConfigObject* config) {
    uint32_t readed_value;
    if (!read_little_endian_int(fp, &readed_value)) return false;
    config->children_count = readed_value;
    if (!read_little_endian_int(fp, &readed_value)) return false;
    config->children_list_size = readed_value;
    if (!read_little_endian_int(fp, &readed_value)) return false;
    config->child_size = readed_value;
    if (!read_little_endian_int(fp, &readed_value)) return false;
    // assume not to read pointer values from config, it is not good idea
    //config->children = (void**)readed_value;
    return true;
}

// BEGTEXT:004C61F4
bool Config::read(FILE* fp, int val0, ConfigObject* config) {
    if (config->signature != SIGN_OBJECT_CONFIG) return false;
    
    if (config->children_count > 0) {
        for(int i=0; i< config->children_count; i++) {
            delete_object(config->children[i*2]);
            delete_object(config->children[i*2+1]);
        }
    }
    
    if (config->children) {
        delete_object(config->children);
    }
    if (!read_header(fp, config)) return false;
    
    config->children = nullptr;
    
    if (config->children_list_size > 0) {
        config->children = (void**)create_object(config->children_list_size * sizeof(void*) * 2);
        if (!config->children) return false;
        
        if (config->children_count > 0) {
            for (int i=0 ;i < config->children_count; i++) {
                config->children[i*2] = nullptr;
                config->children[i*2+1] = nullptr;
            }
            
            for(int i=0; i< config->children_count; i++) {
                int size = fgetc(fp);
                if (size <= 0) return false;
                
                void* name_obj = create_object(size);
                config->children[i*2] = name_obj;
                
                if (!name_obj) return false;
                
                if (fgets((char*)name_obj, size, fp) == nullptr) return false;
                
                if (config->child_size > 0) {
                    
                    config->children[i*2 + 1] = create_object(config->child_size);
                    ConfigObject* inner = (ConfigObject*)config->children[i*2 + 1];
                    
                    if (!config->read(inner, fp, config->child_size)) {
                        //TODO: code here
                    }
                    
                }
            }
            
            return true;
        }
    }
    return false;
}

// BEGTEXT:004C6388
bool Config::write_little_endian_int(FILE* fp, int32_t value) {
    return (fputc( (value >> 24) & 0xff, fp) != -1) &&
           (fputc( (value >> 16) & 0xff, fp) != -1) &&
           (fputc( (value >> 8) & 0xff, fp) != -1) &&
           (fputc( value & 0xff, fp) != -1);
}

// BEGTEXT:004C63FC
bool Config::write_header(FILE* fp, ConfigObject* config) {
    return write_little_endian_int(fp, config->children_count) &&
        write_little_endian_int(fp, config->children_list_size) &&
        write_little_endian_int(fp, config->child_size) &&
        write_little_endian_int(fp, 0); // assume not to write pointer values to config, it is not good idea    
}

// BEGTEXT:004C6440
bool Config::write(FILE* fp, ConfigObject* config) {
    // call to this function not found
    return true;
}

// BEGTEXT:004C6534
void Config::set_functions(void* c(size_t), void* r(void*, unsigned long), void d(void*)) {
    if (!c || !r || !d) {
        create_object = malloc;
        realloc_object = realloc;
        delete_object = free;
    } else {
        create_object = c;
        realloc_object = r;
        delete_object = d;
    }
}