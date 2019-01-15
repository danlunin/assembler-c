#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mainFunctions.h"
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)



int main(int argc, char *argv[]) {
    /*if (argc != 3) {
        exit(1);
    }
    FILE *f_in;
    FILE *f_out;
    f_in = fopen( argv[1], "r" );
    f_out = fopen( argv[2], "w" );*/
    //Temp
    FILE *f_in = fopen("in4.txt", "r" );
    FILE *f_out = fopen( "out.txt", "w" );
    //Temp
    encode_commands_from_file(f_in, f_out);
    fclose(f_in);
    fclose(f_out);
    return 0;
}