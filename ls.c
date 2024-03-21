#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <bits/getopt_core.h>
#include <string.h>
#include <stdlib.h>

const char* option_string = "asr";
const int MAX_NAME_LENGTH = 256;
const int MAX_DIR_ENTS = 500;

struct Option {
    char a, s, r;
};

int cmpstr(void const *a, void const *b) { 
    char const *aa = (char const *)a;
    char const *bb = (char const *)b;

    return strcmp(aa, bb);
}

void do_ls(char* dir_name, struct Option option) {
    DIR* dir_ptr = opendir(dir_name);
    char dir_ents[MAX_DIR_ENTS][MAX_NAME_LENGTH];
    int current_ent = 0;

    if (dir_ptr == 0) {
        perror(dir_name);
    } else {
        struct dirent* dirent_ptr;
        dirent_ptr = readdir(dir_ptr);

        while (dirent_ptr != 0) {
            if (option.a || dirent_ptr->d_name[0] != '.') {
                strncpy(
                    dir_ents[current_ent], 
                    dirent_ptr->d_name, 
                    MAX_NAME_LENGTH - 1
                );
                
                dir_ents[current_ent][MAX_NAME_LENGTH - 1] = '\0';

                current_ent++;
            }

            dirent_ptr = readdir(dir_ptr);
        }
        closedir(dir_ptr);

        if (option.s)
            qsort(dir_ents, current_ent, sizeof(dir_ents[0]), cmpstr);

        for (int i = 0; i < current_ent; i++) {
            printf("%s\n", dir_ents[i]);
        }
    }
}

void parse_options(
    int argc,
    char* argv[],
    int* options,
    struct Option* option
) {
    while (*options != -1) {
        switch (*options) {
            case 'a':
                option->a = 1;
                break;
            case 's':
                option->s = 1;
                break;
            case 'r':
                option->r = 1;
                break;
            default:
                printf(
                    "?? getopt returned character code 0%o ??\n",
                    *options
                );
        }

        *options = getopt(argc, argv, option_string);
    }
}

int main(int argc, char* argv[]) {
    struct Option option = { 0 };
    int options = getopt(argc, argv, option_string);
    char* dir_name;

    parse_options(argc, argv, &options, &option);

    if (optind >= argc) {
        dir_name = ".";
    } else {
        dir_name = argv[optind];
    }
    
    do_ls(dir_name, option);

    return 0;
}