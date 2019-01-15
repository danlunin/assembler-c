#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mainFunctions.h"
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)


int extract_constant(char* constant, unsigned  int size) {
    if (constant[0] != '#')
        return -200;
    char subbuff[size+1];
    memcpy( subbuff, constant+1, size);
    subbuff[size] = '\0';
    int num = atoi(subbuff);
    printf("%d\n", num);
    if ((size == 4 && num < 16 && num > -1) || (size == 8 && num < 256 && num > -1)) {
        return num;
    }
    return -200;
}


unsigned int parse_line_into_tokens (char* line, char* label, char* cmd, char* op1, char* op2) {
    char seps[]   = " \n";
    char *token;
    if (line[0] == ' '){
        token = strtok( line, seps );
        if (token != NULL) {
            strcpy(cmd, token);
        }
        token = strtok( NULL, seps );
        if (token != NULL) {
            strcpy(op1, token);
        }
        token = strtok( NULL, seps );
        if (token != NULL) {
            strcpy(op2, token);
        }
        return 0;
    } else {
        token = strtok( line, seps );
        if (token != NULL) {
            strcpy(label, token);
        }
        token = strtok( NULL, seps );
        if (token != NULL) {
            strcpy(cmd, token);
        }
        token = strtok( NULL, seps );
        if (token != NULL) {
            strcpy(op1, token);
        }
        token = strtok( NULL, seps );
        if (token != NULL) {
            strcpy(op2, token);
        }

        return 1;
    }
}

void add_one_argument (struct rule* command, int value, enum Command command1,
                       int multiply_by_two) {
    command->command = command1;
    if (multiply_by_two) command->argument1.value = 2*value;
    else {
        command->argument1.value = 2*value + 1;
    }
}

void add_two_arguments (struct rule* command, enum Command command1, int value1, int value2) {
    command->command = command1;
    command->argument1.value = value1;
    command->argument2.value = value2;
}

unsigned int check_is_number(char numeral) {
    return (numeral >= '0' && numeral <= '8');
}

unsigned int check_pattern(char* string, char* pattern) {
    unsigned int  no_mistake = 1;
    for (size_t i =0; pattern[i] != '\0'; ++i) {
        if (pattern[i] == '*') {
            if (!check_is_number(string[i])) {
                no_mistake = 0;
            }
        } else if (pattern[i] == '#') {
            if (string[i] != 'H' && string[i] != 'L') {
                no_mistake = 0;
            }
        } else {
            if (pattern[i] != string[i])
                no_mistake = 0;
        }
    }
    return no_mistake;
}

void operate_with_two_simple_regs(struct rule* command, char* op1, char* op2, enum Command command1) {
    if (check_pattern(op1, "R*X") && check_pattern(op2, "R*X")) {
        command->command = command1;
        add_two_arguments(command, command1, op1[1]-'0', op2[1]-'0');
    }
}

void parse_line_into_command(char* line, struct rule* command) {
    char label[20];
    char cmd[10];
    char op1[20];
    char op2[20];
    unsigned int conatins_label = parse_line_into_tokens(line, label, cmd, op1, op2);
    printf("%s %s %s\n", cmd, op1, op2);
    if (conatins_label) {
        strcpy(command->label, label);
        command->contains_label = 1;
    } else {
        command->contains_label = 0;
    };
    if (strcmp(cmd, "mov") == 0) {
        if (check_pattern(op1, "R*X") && check_pattern(op2, "R*X")) {
            add_two_arguments(command, Mov, op1[1]-'0', op2[1]-'0');
        } else if (check_pattern(op1, "R*X") && check_pattern(op2, "(R*X)")) {
            command->command = Mov1;
            add_two_arguments(command, Mov1, op1[1]-'0', op2[2]-'0');
        } else if (check_pattern(op1, "(R*X)") && check_pattern(op2, "R*X")) {
            add_two_arguments(command, Mov2, op1[2]-'0', op2[1]-'0');
        }else if (check_pattern(op1, "R*H") || (check_pattern(op1, "R*L") && line[8] == '#')) {
            if (op1[2] == 'H') {
                command->command = Mov3;
                command->argument1.value = 2*op1[1];
                command->argument2.value = extract_constant(op2, 8);
                //add_two_arguments(command, Mov3, op1[1], extract_constant(op2, 8), 1);
            } else {
                command->command = Mov3;
                command->argument1.value = 2*op1[1] + 1;
                command->argument2.value = extract_constant(op2, 8);
                //add_two_arguments(command, Mov3, op1[1], extract_constant(op2, 8), 0);
            }

        }
    } else if (strcmp(cmd, "mul") == 0) {
        operate_with_two_simple_regs(command, op1, op2, Mul);
    } else if (strcmp(cmd, "add") == 0) {
        operate_with_two_simple_regs(command, op1, op2, Add);
    }
    else if (strcmp(cmd, "sub") == 0) {
        operate_with_two_simple_regs(command, op1, op2, Sub);
    }
    else if (strcmp(cmd, "and") == 0) {
        operate_with_two_simple_regs(command, op1, op2, And);
    }
    else if (strcmp(cmd, "or") == 0) {
        operate_with_two_simple_regs(command, op1, op2, Or);
    }
    else if (strcmp(cmd, "xor") == 0) {
        operate_with_two_simple_regs(command, op1, op2, Xor);
    }
    else if (strcmp(cmd, "push") == 0 && check_pattern(op1, "R*X")) {
        add_one_argument(command, op1[1] - '0', Push, 0);
    }
    else if (strcmp(cmd, "pop") == 0 && check_pattern(op1, "R*X")) {
        add_one_argument(command, op1[1] - '0', Pop, 0);
    }
    else if (strcmp(cmd, "call") == 0 && extract_constant(op1, 8) != -200) {
        add_one_argument(command, op1[1] - '0', Call, 0);
        command->argument1.value = extract_constant(op1, 8);
    }
    else if (strcmp(cmd, "not") == 0 && check_pattern(op1, "R*X")) {
        add_one_argument(command, op1[1] - '0', Not, 0);
    }
    else if (strcmp(cmd, "reset") == 0) {
        command->command = Reset;
    }
    else if (strcmp(cmd, "ret") == 0) {
        command->command = Ret;
    }
    else if (strcmp(cmd, "nop") == 0) {
        command->command = Nop;
    }
    else if (strcmp(cmd, "in") == 0 && check_pattern(op1, "R*#")) {
        if (op1[2] == 'H') {
            add_one_argument(command, op1[1] - '0', In, 1);
        } else {
            add_one_argument(command, op1[1] - '0', In, 0);
        }
    }
    else if (strcmp(cmd, "out") == 0 && check_pattern(op1, "R*#")) {
        if (op1[2] == 'H') {
            add_one_argument(command, op1[1] - '0', Out, 1);
        } else {
            add_one_argument(command, op1[1] - '0', Out, 0);
        }
    }
    else if (strcmp(cmd, "shl") == 0 && check_pattern(op1, "R*X")) {
        add_one_argument(command, op1[1] - '0', Shl, 0);
        int res = extract_constant(op2, 4);
        if (res!=-200){
            command->argument2.value = res;
        }
    }
    else if (strcmp(cmd, "shr") == 0 && check_pattern(op1, "R*X")) {
        add_one_argument(command, op1[1] - '0', Shr, 0);
        int res = extract_constant(op2, 4);
        if (res!=-200){
            command->argument2.value = res;
        }
    }
    else if (strcmp(cmd, "jmp") == 0) {
        command->command = Jmp;
        strcpy(command->label, op1);
    }
    else if (strcmp(cmd, "je") == 0 && check_pattern(op2, "R*X")) {
        command->command = Je;
        command->argument2.value = op2[1] - '0';
        strcpy(command->label, op1);
    }
    else if (strcmp(cmd, "jne") == 0 && check_pattern(op2, "R*X")) {
        command->command = Jne;
        command->argument2.value = op2[1] - '0';
        strcpy(command->label, op1);
    }

}

void get_binary_with_current_length(unsigned int length, unsigned int value, char* finalString){
    unsigned int i = 0;
    while ( length-- )
        finalString[ i++ ] = bitRead( value, length ) + '0';
    finalString[ i ] = '\0';
}

int get_jump_offset(struct rule all_commands[], struct rule* command, unsigned int current_position,
        unsigned int commands_size) {
    char *search_label = malloc(strlen(command->label)+ 2);
    //for (size_t i = 0; i < strlen(command->label); ++i) {
    //    search_label[i] = command->label[i];
    //}
    strcpy(search_label, command->label);
    search_label[strlen(command->label)] = ':';
    search_label[strlen(command->label) + 1] = '\0';
    printf("Result: %s Start: %s\n", search_label, command->label);
    for (size_t i=1; i < 128 && i + current_position < commands_size; ++i) {
        if (all_commands[current_position + i].contains_label &&
            strcmp(search_label, all_commands[current_position + i].label) == 0) {
            return i;
        }
    }
    for (size_t i=-1; i > -128 && i + current_position > 0; --i) {
        if (all_commands[current_position + i].contains_label &&
            strcmp(search_label, all_commands[current_position + i].label) == 0) {
            return  i;
        }
    }
    free(search_label);
    return 0;
}

void encode_command(unsigned int size1, unsigned int value1, unsigned int size2, unsigned int value2, unsigned int size3,
                    unsigned int value3, char* binary_string) {
    char forCommand[size1+1];
    char forArgumentOne[size2+1];
    char forArgumentTwo[size3+1];
    get_binary_with_current_length(size1, value1, forCommand);
    strcpy(binary_string, forCommand);
    get_binary_with_current_length(size2, value2, forArgumentOne);
    strcat(binary_string, forArgumentOne);
    get_binary_with_current_length(size3, value3, forArgumentTwo);
    strcat(binary_string, forArgumentTwo);
}

void parse_command_into_binary(struct rule* command, char* binary_string, struct rule all_commands[],
                               unsigned int current_position, unsigned int commands_size) {
    switch (command->command) {
        case Mov:
        case Mov1:
        case Mov2:
        case Mul:
        case Add:
        case Sub:
        case Div:
        case And:
        case Or:
        case Xor:
        case Shr:
        case Shl:
            encode_command(8, command->command, 4, command->argument1.value, 4, command->argument2.value,
                           binary_string);
            break;
        case Push:
        case Pop:
        case Not:
            encode_command(8, command->command, 4, command->argument1.value, 4, 0,
                           binary_string);
            break;
        case Ret:
        case Reset:
        case Nop:
            encode_command(8, command->command, 4, 0, 4, 0,
                           binary_string);
            break;
        case In:
        case Out:
            encode_command(8, command->command, 5, command->argument1.value, 3, 0,
                           binary_string);
            break;
        case Call:
        case Jmp:
            if (command->command == Jmp) {
                command->argument1.value = get_jump_offset(all_commands, command, current_position, commands_size);
            }
            encode_command(8, command->command, 8, command->argument1.value, 0, 0,
                           binary_string);
            break;
        case Mov3:
            encode_command(4, command->command, 4, command->argument1.value, 8, command->argument2.value,
                           binary_string);
            printf("IMM: %d", command->argument2.value);
            binary_string[0] = '1';
            binary_string[1] = '0';
            break;
        case Je:
        case Jne:
            command->argument1.value = get_jump_offset(all_commands, command, current_position, commands_size);
            encode_command(4, command->command, 8, command->argument1.value, 4, command->argument2.value,
                           binary_string);
            printf("Jump to: %d \n", command->argument1.value);
            if (command->command == Je) {
                binary_string[0] = '1';
            } else {
                binary_string[0] = '0';
            }
            binary_string[1] = '1';
    }
}

void write_to_file(char* binary_string, FILE *f_out) {
    fwrite(binary_string, sizeof(char), 16, f_out);
}

void encode_commands_from_file (FILE *f_in, FILE *f_out) {
    char line[100];
    char binary_str[17];
    struct rule all_commands[1000];
    unsigned int counter = 0;
    while (fgets(line, sizeof(line), f_in)) {
        parse_line_into_command(line, &all_commands[counter]);
        ++counter;
    }
    for (size_t i =0; i < counter; ++i) {
        parse_command_into_binary(&all_commands[i], binary_str, all_commands, i, counter);
        write_to_file(binary_str, f_out);
    }
}