#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef ASSEMBLER_ASM_H
#define ASSEMBLER_ASM_H
enum Command{Mov, Mov1, Mov2, Mov3, Push, Pop, Call, Ret, Add, Sub, Mul, Div, And, Or, Xor, Not, Shl, Shr, Reset,
    Nop, Jmp, Je, Jne, In, Out, Cmpxchg};


struct argument
{
    int value;
};

struct rule
{
    enum Command command;
    struct argument argument1;
    struct argument argument2;
    struct argument argument3;
    char label[20];
    int contains_label;
};

int extract_constant(char* constant, unsigned  int size);
unsigned int parse_line_into_tokens (char* line, char* label, char* cmd, char* op1, char* op2, char *op3);
void add_one_argument (struct rule* command, int value, enum Command command1,
                       int multiply_by_two, int small_register);
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
void parse_command_into_binary(struct rule* command, char* binary_string, struct rule* all_commands,
                               unsigned int current_position, unsigned int commands_size);
void write_to_file(char* binary_string, FILE *f_out);
        void encode_commands_from_file (FILE *f_in, FILE *f_out);
void get_substring(unsigned int size, unsigned int offset, char* string, char * substr);
void decode_command(struct rule *com, char * line);
void decode_whole_rule(struct rule *com, int firstSize, int firstOffset, int secondSize, int secondOffset,
                       enum Command command, char *line);
int get_substr_and_convert_to_int(unsigned int size, unsigned int offset, char* string);
void execute_operation(int *registers, struct rule* com, char operation);
int calculate(int *registers, struct rule* com, char operation);
void extract_whole_register_value(int *registers, int reg_num, int result);
void initialize_registers(int *registers);
void print_registers_states(int *registers);
#endif //ASSEMBLER_ASM_H
