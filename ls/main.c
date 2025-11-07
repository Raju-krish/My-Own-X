#include <stdint.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>


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

uint8_t is_entry_hidden(const char *name)
{
    if(name != NULL && name[0] == '.')
        return YES;
    return NO;
}

void list_dir(DIR *dirp) 
{
    struct dirent* entry = NULL;
    while((entry = readdir(dirp)) != NULL)
    {
        if(is_entry_hidden(entry->d_name) == YES && g_list_hidden == NO)
            continue;

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
            list_dir(dirp);
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
        list_dir(dirp);
    }
}
