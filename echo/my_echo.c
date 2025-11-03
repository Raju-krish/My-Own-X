#include <stdint.h>
#include <stdio.h>
#define SET     1
#define UNSET   0

uint8_t disable_new_line        = 0;
uint8_t interpret_escape_seq    = 0;
uint8_t end_of_options          = 0;
uint8_t option_is_string        = 0;

void reset_globals()
{
    disable_new_line = UNSET;
    interpret_escape_seq = UNSET;
    end_of_options = UNSET;
}
void handle_options(char *argv) 
{
    char *s = argv;
    while(*s != '\0') {
        if(option_is_string == 1) break;
        switch(*s) {
            case '-': // if -- is given treat everyting after this as a plain text
                end_of_options += SET;
                break;
            case 'n': // Disables appending new line character
                disable_new_line = SET;
                break;
            case 'e': // Interpret escape seq
                interpret_escape_seq = SET;
                break;
            case 'E': // Don't interpret escape seq
                interpret_escape_seq = UNSET;
                break;
            default: // Invalid Option hence reset all options, and treat it as a string
                option_is_string = SET;
                reset_globals();
                break;
        }
        s++;
    }
}

int main (int argc, char *argv[])
{
    for(int i = 1; i < argc; i++) {
        if(end_of_options != 2 && argv[i][0] == '-'){ 
            handle_options(argv[i]);
            if(option_is_string == 0)
                continue;
        }
        printf("%s", argv[i]);
        if(i != argc-1) printf(" ");
    }
    
    if(disable_new_line != SET)
        printf("\n");
}
