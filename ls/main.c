#include <stdint.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "my_list.h"

// Print colors & formats
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define WHITE   "\033[37m"
#define BOLD      "\033[1m"
#define UNDERLINE "\033[4m"
#define RESET     "\033[0m"

// Readability MACROS
#define NO    0
#define YES   1

// gloabal variables
uint8_t g_list_hidden = NO;


struct entry_info {
    struct dirent meta;
    struct stat detail_info;
};


uint8_t is_entry_hidden(const char *name)
{
    if(name != NULL && name[0] == '.')
        return YES;
    return NO;
}

void print_list(void *data)
{
    struct entry_info *info = (struct entry_info *)data;
    printf("This is from callback func: file is %s\n", info->meta.d_name);
}

void list_dir(DIR *dirp, char *parent) 
{
    struct dirent* entry = NULL;
    struct stat info;
    char path[512] = {0};

    while((entry = readdir(dirp)) != NULL)
    {
        if(is_entry_hidden(entry->d_name) == YES && g_list_hidden == NO)
            continue;
        
        struct entry_info *p_info = malloc(sizeof(struct entry_info));
    
        snprintf(path, sizeof(path), "%s/%s", parent, entry->d_name);
        stat(path, &info);

        memcpy(&(p_info->meta), entry, sizeof(struct dirent));
        memcpy(&(p_info->detail_info), &info, sizeof(struct stat));
        
        insert_list((void*)p_info);

        printf("%*ld%*ld%*o", -8,info.st_size, 0,info.st_ino, 2,info.st_mode & 0777);

        switch(entry->d_type) {
            case DT_DIR:
                printf(BOLD BLUE "%s\n" RESET, entry->d_name);
                break;

            case DT_REG:
                printf(WHITE "%s\n" RESET, entry->d_name);
                break;

            case DT_FIFO:
                printf(YELLOW "%s\n" RESET, entry->d_name);
                break;

            default:
                printf("%s\n", entry->d_name);
                break;
        }
    }
    closedir(dirp);
}

int main(int argc, char *argv[])
{
    DIR *dirp = NULL;

    /* If command line argumnets provided */
    if(argc >= 2) {
        int i = 1;
        while(argv[i]) {
            printf(UNDERLINE "Contents of [%s] :\n" RESET, argv[i]);
            dirp = opendir(argv[i]);
            if(dirp == NULL) {
                printf("my_ls: cannot access '%s' %s\n", argv[i], strerror(errno));
                i++;
                continue;
            }
            list_dir(dirp, argv[i]);
            printf("\n");
            i++;
        }
    }
    /* If no cmd line arg, list cur directory */
    else {
        dirp = opendir(".");
        if(dirp == NULL) {
            printf("my_ls: cannot access cur dir %s\n", strerror(errno));
        }
        list_dir(dirp, ".");
    }
    for_each_entry(print_list);
    clear_list();
}
