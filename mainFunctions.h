#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef ASSEMBLER_ASM_H
#define ASSEMBLER_ASM_H
enum Command{Mov, Mov1, Mov2, Mov3, Push, Pop, Call, Ret, Add, Sub, Mul, Div, And, Or, Xor, Not, Shl, Shr, Reset,
    Nop, Jmp, Je, Jne, In, Out};


struct argument
{
    unsigned int value;
};

struct rule
{
    enum Command command;
    struct argument argument1;
    struct argument argument2;
    char label[20];
    int contains_label;
};

int extract_constant(char* constant, unsigned  int size);
unsigned int parse_line_into_tokens (char* line, char* label, char* cmd, char* op1, char* op2);
void add_one_argument (struct rule* command, int value, enum Command command1,
                       int multiply_by_two);
void add_two_arguments (struct rule* command, enum Command command1, int value1, int value2);
unsigned int check_is_number(char numeral);
unsigned int check_pattern(char* string, char* pattern);
void operate_with_two_simple_regs(struct rule* command, char* op1, char* op2, enum Command command1);
void parse_line_into_command(char* line, struct rule* command);
void get_binary_with_current_length(unsigned int length, unsigned int value, char* finalString);
int get_jump_offset(struct rule all_commands[], struct rule* command, unsigned int current_position,
        unsigned int commands_size);
void encode_command(unsigned int size1, unsigned int value1, unsigned int size2, unsigned int value2, unsigned int size3,
                    unsigned int value3, char* binary_string);
void parse_command_into_binary(struct rule* command, char* binary_string, struct rule all_commands[],
                               unsigned int current_position, unsigned int commands_size);
void write_to_file(char* binary_string, FILE *f_out);
        void encode_commands_from_file (FILE *f_in, FILE *f_out);

#endif //ASSEMBLER_ASM_H
