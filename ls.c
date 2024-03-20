#include <stdio.h>
#include <dirent.h>

void do_ls(char* dir_name) {
    DIR* dir_ptr;
    struct dirent* dirent_ptr;
    dir_ptr = opendir(dir_name);

    if(dir_ptr == 0) {
        perror(dir_name);
    } else {
        dirent_ptr = readdir(dir_ptr);

        while(dirent_ptr != 0) {
            printf("%s\n", dirent_ptr->d_name);

            dirent_ptr = readdir(dir_ptr);
        }

        closedir(dir_ptr);
    }
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        do_ls(".");
    } else {
        do_ls(argv[1]);
    }

    return 0;
}