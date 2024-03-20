#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <bits/getopt_core.h>

const char* option_string = "asr";

struct Option {
    char a, s, r;
};

void print_dir(DIR* dir_ptr) {
    struct dirent* dirent_ptr;

    dirent_ptr = readdir(dir_ptr);

    while (dirent_ptr != 0) {
        printf("%s\n", dirent_ptr->d_name);
        dirent_ptr = readdir(dir_ptr);
    }
}

void do_ls(char* dir_name, struct Option option) {
    DIR* dir_ptr = opendir(dir_name);

    if (dir_ptr == 0) {
        perror(dir_name);
    } else {
        print_dir(dir_ptr);
        closedir(dir_ptr);
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