//
//  game_config.cpp
//  f_decomp
//
//  Created by Mikhail Kochegarov on 5/24/13.
//  Copyright (c) 2013 Mikhail Kochegarov. All rights reserved.
//

#include "game_config.h"

// DGROUP:004FE8FC
char GameConfig::char_table[256] = {
    0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0xA,  0xC,  0xC,  0xC,  0xC,  0xC,  0xC,  0xC,  0xC,  0xC, 0xC,  0xC,  0xC,  0xC,  0xC, 0xC,
    0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0xC,  0xC,  0xC,  0xC,  0xC, 0xC,
    0xC, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48,0x48,
    0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0xC,  0xC,  0xC,  0xC,  0xC,
    0xC, -0x68,-0x68,-0x68,-0x68,-0x68,-0x68,-0x78,-0x78,-0x78,-0x78,-0x78,-0x78,-0x78,-0x78,-0x78,
    -0x78,-0x78,-0x78,-0x78,-0x78,-0x78,-0x78,-0x78,-0x78,-0x78,-0x78, 0xC, 0xC,  0xC,  0xC,   1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// DGROUP:00504C28
char GameConfig::section_name[256] = "unknown";


// BEGTEXT:00426540
bool GameConfig::create(ConfigObject* config) {
    if (config) {
        return Config::create_config(config, 10, (sizeof(ConfigObject)), 0);
    }
    return false;
}

// BEGTEXT:0042656C
// TODO: not tested yet
bool GameConfig::remove(ConfigObject* config) {
    if (config) {
        
        for(int i=0; i < config->children_count; i++) {
            ConfigObject* inner = (ConfigObject*)(config->children[i*2 + 1]);
            
            for(int j=0; j < inner->children_count*2; j++) {
                Memory::run_delete_object_func(inner->children[j]);
            }
            Config::free_config(inner);
        }
        
        Config::free_config(config);
    }
    return false;
}

// BEGTEXT:004265D0
bool GameConfig::parse_argv(ConfigObject* config_obj, int argc, const char** argv) {
    //TODO: code here, not necessary now parse each line of argv as like as from config file
    return true;
}

// BEGTEXT:004266E0
bool GameConfig::get_string(ConfigObject* config, const char* section, const char* key, char** val) {
    if (!config || !section || !key) return nullptr;
    int index = -1;
    if (Config::find_child(config, section, &index)) {
        ConfigObject* child = (ConfigObject*)(config->children)[index * 2 + 1];
        if (child && Config::find_child(child, key, &index)) {
            *val = (char*)(child->children)[index * 2 + 1];
            return true;
        }
    }
    return false;
}

// BEGTEXT:00426728
bool GameConfig::set_string(ConfigObject* config, const char* section, const char* key, const char* value) {
    if (!config || !section || !key || !value) return false;
    
    int id;
    if (!Config::find_child(config, section, &id)) { //if not found
        if (!GameConfig::create_child(config, section)) {
            return false;
        }
        
        if (!Config::find_child(config, section, &id)) return false;
    }
    
    ConfigObject* child_config = (ConfigObject*)((config->children)[id * 2 + 1]);
    
    if (Config::find_child(child_config, key, &id)) {
        //Memory::run_delete_object_func( child_config->children[id * 2 + 1]);
        
        Config::delete_child(child_config, key);
    }
    
    void* value_obj = Memory::create_object_with_value(value);
    if (value_obj) {
        if (Config::create_child(child_config, key, value_obj)) {
            return true;
        }
        Memory::run_delete_object_func(value_obj);
    }
    
    return false;
}

// BEGTEXT:004267DC
bool GameConfig::get_int(ConfigObject* config, const char* section, const char* key, uint32_t* result) {
    char** value = nullptr;
    if (GameConfig::get_string(config, section, key, value)) {
        *result = atoi(*value);
        return true;
    }
    return false;
}

// BEGTEXT:00426810
bool GameConfig::get_float(ConfigObject* config, const char* section, const char* key, float* result) {
    char** value = nullptr;
    if (GameConfig::get_string(config, section, key, value)) {
        *result = atof(*value);
        return true;
    }
    return false;
}

// BEGTEXT:004268E0
bool GameConfig::set_int(ConfigObject* config, const char* section, const char* key, int value) {
    char val[10];
    snprintf(val, 10, "%i", value);
    return set_string(config, section, key, val);
}

// BEGTEXT:00426910
bool GameConfig::set_float(ConfigObject* config, const char* section, const char* key, float value) {
    char val[10];
    snprintf(val, 10, "%f", value);
    return set_string(config, section, key, val);
}

// BEGTEXT:00426A00
bool GameConfig::load(ConfigObject* config, char* config_path, bool flag) {
    if (!config || ! config_path) return false;
    
    FILE* fp;
    char readed_line[256];
    
    if (flag) {
        //TODO: code here
        return true;
    }
    
    fp = fopen(config_path, "rt");
    if (fp != 0) {
        while(fgets(readed_line, 256, fp)) {
            GameConfig::parse_config_line(config, readed_line);
        }
        
        fclose(fp);
        return true;
    }
    
    return false;
}

// BEGTEXT:00426AA4
// not tested yet
bool GameConfig::save(ConfigObject* config, char* config_path, bool flag) {
    if (!config || ! config_path) return false;
    
    if (!flag) {
        
        FILE *fp = fopen(config_path, "wt");
        
        for(int i=0; i < config->children_count; i++) {
            fprintf(fp, "[%s]\n", (char*)(config->children[i*2]));
            ConfigObject* inner = (ConfigObject*)(config->children[i*2]);
            for (int j=0; j< inner->children_count; j++) {
                fprintf(fp, "%s=%s\n", (char*)(inner->children[j*2]), (char*)(inner->children[j*2 + 1]));
            }
            fprintf(fp, "\n");
        }
        fclose(fp);

        return true;
    }
    
    return true;
}

// BEGTEXT:00426C3C
void GameConfig::parse_config_line(ConfigObject* config, char* readed_line) {
    
    char* parsed_line;
    
    parsed_line = strchr(readed_line, ';');
    if (parsed_line) parsed_line[0] = '\0';
    
    parsed_line = strchr(readed_line, '[');
    if (parsed_line) {
        readed_line = parsed_line+1;
        parsed_line = strchr(readed_line, ']');
        if (!parsed_line) {
            return;
        }
        parsed_line[0] = '\0';
        strcpy(GameConfig::section_name, readed_line);
        
        GameConfig::check_string(section_name);
    } else {
        char* value = parse_inner_section_line(readed_line);
        if (value != nullptr) {
            GameConfig::set_string(config, GameConfig::section_name, readed_line, value);
        }
    }
}

// BEGTEXT:00426D14
char* GameConfig::parse_inner_section_line(char* key) {
    char* value = strchr(key, '=');
    if (value) {
        value[0] = '\0';
        value++;
        
        GameConfig::check_string(key);
        GameConfig::check_string(value);
        return value;
    }
    return nullptr;
}

// BEGTEXT:00426DB8
bool GameConfig::create_child(ConfigObject* config, const char* key) {
    int id;
    
    if (Config::find_child(config, key, &id)) return true;
    
    ConfigObject* children_obj = new ConfigObject();
    
    if (Config::create_config(children_obj, 10, sizeof(void*), 0)) {
        Config::create_child(config, key, (void*)children_obj);
        return true;
    }
    return false;
}

// BEGTEXT:00426E18
void GameConfig::check_string(char* section_name) {
    char* str = section_name;
    int length = (int)strlen((char*)str);
    if (length > 0) {
        int i = length;
        int start = 0;
        while(i > 0) {
            if (GameConfig::char_table[str[i]+1] & CHAR_SPACE) {
                start++;
            } else {
                break;
            }
            i++;
        }
        i = length-1;
        while (i > 0) {
            if (GameConfig::char_table[str[i]+1] & CHAR_SPACE) {
                str[i] = '\0';
            } else {
                break;
            }
            i--;
        }
        
        section_name += start;
    }
}

// BEGTEXT:00426E98
bool GameConfig::get_double(ConfigObject* config, char* section, char* key, double* result) {
    char** value = nullptr;
    if (GameConfig::get_string(config, section, key, value)) {
        *result = strtod(*value, nullptr);
        return true;
    }
    return false;
}

// BEGTEXT:00426ECC
bool GameConfig::set_double(ConfigObject* config, const char* section, const char* key, double value) {
    char val[10];
    snprintf(val, 10, "%.6f", value);
    return GameConfig::set_string(config, section, key, val);
}

