//
//  game_config.h
//  f_decomp
//
//  Created by Mikhail Kochegarov on 5/24/13.
//  Copyright (c) 2013 Mikhail Kochegarov. All rights reserved.
//

#ifndef __f_decomp__game_config__
#define __f_decomp__game_config__

#include <iostream>
#include "config.h"
#include "memory.h"

#define CHAR_SPECIAL    0x1
#define CHAR_SPACE      0x2 //TAB LF VT FF CR ' '
#define CHAR_SPEC       0x4 //.,;' ...
#define CHAR_PRINTABLE  0x8
#define CHAR_HEX        0x10
#define CHAR_NUMBER     0x20
#define CHAR_CHAR       0x40
#define CHAR_CAPITALIZE 0x80

class GameConfig {
private:
    
    static char section_name[256];
    
    static char char_table[256];
    
    static bool create_child(ConfigObject* config, const char* key);
    
    static void parse_config_line(ConfigObject* config, char* readed_line);
    
    static char* parse_inner_section_line(char* key);
    
    static void check_string(char* section_name);

public:

    static bool create(ConfigObject* config);
    
    static bool remove(ConfigObject* config);
    
    static bool parse_argv(ConfigObject* config_obj, int argc, const char** argv);
    
    static bool load(ConfigObject* config, char* config_file_path, bool flag);
    
    static bool save(ConfigObject* config, char* config_file_path, bool flag);
    

    static bool get_string(ConfigObject* config, const char* section, const char* key, char** value);

    static bool set_string(ConfigObject* config, const char* section, const char* key, const char* value);
    
    static bool get_int(ConfigObject* config, const char* section, const char* key, uint32_t* value);
    
    static bool set_int(ConfigObject* config, const char* section, const char* key, int value);
    
    static bool get_float(ConfigObject* config, const char* section, const char* key, float* value);
    
    static bool set_float(ConfigObject* config, const char* section, const char* key, float value);
    
    static bool get_double(ConfigObject* config, char* section, char* key, double* result);
    
    static bool set_double(ConfigObject* config, const char* section, const char* key, double value);
    
};

#endif /* defined(__f_decomp__game_config__) */
