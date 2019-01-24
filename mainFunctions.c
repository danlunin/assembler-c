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
    if ((size == 4 && num < 16 && num > -1) || (size == 8 && num < 256 && num > -1)) {
        return num;
    }
    return -200;
}


unsigned int parse_line_into_tokens (char* line, char* label, char* cmd, char* op1, char* op2, char *op3) {
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
        if (strcmp(cmd, "cmpxchg") == 0) {
            token = strtok( NULL, seps );
            if (token != NULL) {
                //printf("%s\n", token);
                strcpy(op3, token);
            }
        }
        //printf("Doesn't contain label: -%d-\n", line[0]);
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
        //printf("Contains label %s %s %s %s\n", label, cmd, op1, op2);
        return 1;
    }
}

void add_one_argument (struct rule* command, int value, enum Command command1,
                       int multiply_by_two, int small_register) {
    command->command = command1;
    if (multiply_by_two) command->argument1.value = 2*value;
    else if (small_register){
        command->argument1.value = 2*value + 1;
    } else {
        command->argument1.value = value;
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
                return 0;
            }
        } else if (pattern[i] == '#') {

            if (string[i] != 'H' && string[i] != 'L') {
                return 0;
            }
        } else {
            if (pattern[i] != string[i]) {
                return 0;
            }

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
    char op3[20];
    if (line[0] == 10) return;
    unsigned int conatins_label = parse_line_into_tokens(line, label, cmd, op1, op2, op3);
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
        } else if ((check_pattern(op1, "R*#")) && op2[0] == '#') {
            if (op1[2] == 'H') {
                command->command = Mov3;
                command->argument1.value = 2*(op1[1] - '0');
                command->argument2.value = extract_constant(op2, 8);
            } else {
                command->command = Mov3;
                command->argument1.value = 2*(op1[1] - '0') + 1;
                command->argument2.value = extract_constant(op2, 8);
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
    else if (strcmp(cmd, "div") == 0) {
        operate_with_two_simple_regs(command, op1, op2, Div);
    }
    else if (strcmp(cmd, "push") == 0 && check_pattern(op1, "R*X")) {
        add_one_argument(command, op1[1] - '0', Push, 0, 0);
    }
    else if (strcmp(cmd, "pop") == 0 && check_pattern(op1, "R*X")) {
        add_one_argument(command, op1[1] - '0', Pop, 0, 0);
    }
    else if (strcmp(cmd, "call") == 0 && extract_constant(op1, 8) != -200) {
        add_one_argument(command, op1[1] - '0', Call, 0, 0);
        command->argument1.value = extract_constant(op1, 8);
    }
    else if (strcmp(cmd, "not") == 0 && check_pattern(op1, "R*X")) {
        add_one_argument(command, op1[1] - '0', Not, 0, 0);
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
            add_one_argument(command, op1[1] - '0', In, 1, 1);
        } else {
            add_one_argument(command, op1[1] - '0', In, 0, 1);
        }
    }
    else if (strcmp(cmd, "out") == 0 && check_pattern(op1, "R*#")) {
        if (op1[2] == 'H') {
            add_one_argument(command, op1[1] - '0', Out, 1, 1);
        } else {
            add_one_argument(command, op1[1] - '0', Out, 0, 1);
        }
    }
    else if (strcmp(cmd, "shl") == 0 && check_pattern(op1, "R*X")) {
        add_one_argument(command, op1[1] - '0', Shl, 0, 0);
        int res = extract_constant(op2, 4);
        if (res!=-200){
            command->argument2.value = res;
        }
    }
    else if (strcmp(cmd, "shr") == 0 && check_pattern(op1, "R*X")) {
        add_one_argument(command, op1[1] - '0', Shr, 0, 0);
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
    } else if (strcmp(cmd, "cmpxchg") == 0 && check_pattern(op1, "(R*X)") &&
        check_pattern(op2, "R*X") && check_pattern(op3, "R*X")) {
        add_two_arguments(command, Cmpxchg, op1[2]-'0', op2[1]-'0');
        command->argument3.value = op3[1] - '0';
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
    strcpy(search_label, command->label);
    search_label[strlen(command->label)] = ':';
    search_label[strlen(command->label) + 1] = '\0';
    //printf("Result: %s Start: %s\n", search_label, command->label);
    for (size_t i=1; i < 128 && i + current_position < commands_size; ++i) {
        if (all_commands[current_position + i].contains_label &&
            strcmp(search_label, all_commands[current_position + i].label) == 0) {
            return i;
        }
    }
    for (int i=-1; i > -128 && i + current_position > 0; --i) {
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

void parse_command_into_binary(struct rule* command, char* binary_string, struct rule *all_commands,
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
                command->argument1.value = 128 + get_jump_offset(all_commands, command, current_position, commands_size);
                //printf("Jump to: %d \n", command->argument1.value);
            }
            //printf("CALL: %d \n", command->argument1.value);
            encode_command(8, command->command, 8, command->argument1.value, 0, 0,
                           binary_string);
            break;
        case Mov3:
            encode_command(4, command->command, 4, command->argument1.value, 8, command->argument2.value,
                           binary_string);
           //("IMM: %d", command->argument2.value);
            binary_string[0] = '1';
            binary_string[1] = '0';
            binary_string[2] = '0';
            break;
        case Je:
        case Jne:
            command->argument1.value = 128 + get_jump_offset(all_commands, command, current_position, commands_size);
            encode_command(4, command->command, 8, command->argument1.value, 4, command->argument2.value,
                           binary_string);
            //printf("Jump to: %d \n", command->argument1.value);
            if (command->command == Je) {
                binary_string[0] = '1';
            } else {
                binary_string[0] = '0';
            }
            binary_string[1] = '1';
            binary_string[2] = '0';
            break;
        case Cmpxchg:
            encode_command(10, command->argument1.value, 3, command->argument2.value, 3, command->argument3.value,
                           binary_string);
            binary_string[0] = '1';
            binary_string[1] = '1';
            binary_string[2] = '1';
            //printf("Cmpxchg first: %d second: %d third: %d \n", command->argument1.value,
            //        command->argument2.value, command->argument3.value);
            //printf("Exit");
    }
}

void get_substring(unsigned int size, unsigned int offset, char* string, char * substr) {
    int c = 0 ;
    while (c < size) {
        substr[c] = string[offset+c];
        c++;
    }
    substr[c] = '\0';
}

void convert_to_binary(char* binary_string, char* substr, char *data, int i, int offset){
    char *ptr;
    get_substring(8, offset, binary_string, substr);
    //printf("For converting: %s\n", substr);
    int gettt = (int)strtol(substr, &ptr, 2);
    //printf("STRTOL: %d\n", gettt);
    data[i] =  gettt;
}

void write_to_file(char* binary_string, FILE *f_out) {
    char substr[10 + 8];
    char data[3];
    convert_to_binary(binary_string, substr, data, 0, 0);
    convert_to_binary(binary_string, substr, data, 1, 8);
    //printf("DATA: %d %d\n", data[0], data[1]);
    //printf("Data: %s Size: %d\n", data, sizeof(data)*strlen(data));
    data[2] = '\0';
    //printf("Size of char: %d\n", sizeof(char));
    fwrite(&data[0], 1, 1, f_out);
    fwrite(&data[1], 1, 1, f_out);
    //fputc(data[0], f_out);
    //fputc(0, f_out);
    //fputc(data[1], f_out);
}

void encode_commands_from_file(FILE *f_in, FILE *f_out) {
    char line[100];
    char binary_str[17];
    struct rule all_commands[1000];
    unsigned int counter = 0;
    while (fgets(line, sizeof(line), f_in)) {


        if (line[0] != 10) {
            if (line[0] != ' ') {
                char line2[100];
                fgets(line2, sizeof(line2), f_in);


                strcat(line, line2);
                //printf("Line Label: %s\n", line);
                parse_line_into_command(line, &all_commands[counter]);
            } else {

                //printf("Line: %s\n", line);
                parse_line_into_command(line, &all_commands[counter]);

            }
            ++counter;


        }

    }
    //printf("Counted %d\n", counter);
    for (size_t i =0; i < counter; ++i) {
        parse_command_into_binary(&all_commands[i], binary_str, all_commands, i, counter);
        //printf("Command id: %d\n", all_commands[i].command);
        //printf("Binary_str: %s\n", binary_str);
        write_to_file(binary_str, f_out);
    }
}

void decode_whole_rule(struct rule *com, int firstSize, int firstOffset, int secondSize, int secondOffset,
                       enum Command command, char *line) {
    com->argument1.value = get_substr_and_convert_to_int(firstSize, firstOffset, line);
    com->command = command;
    com->argument2.value = get_substr_and_convert_to_int(secondSize, secondOffset, line);
    //printf("Command: %d, Arg1: %d, Arg2: %d\n", com->command, com->argument1.value, com->argument2.value);

}

void decode_command(struct rule *com, char * line) {
    //printf("Line: %s\n", line);
    if (line[0] == '1' && line[1] == '1' && line[2] == '1') {
        decode_whole_rule(com, 3, 7, 3, 10, Cmpxchg, line);
        com->argument3.value = get_substr_and_convert_to_int(3, 13, line);
    } else if (line[0] == '1' && line[1] == '1') {
        decode_whole_rule(com, 8, 4, 4, 12, Je, line);
        com->argument1.value -= 128;
    } else if (line[0] == '0' && line[1] == '1') {
        decode_whole_rule(com, 8, 4, 4, 12, Jne, line);
        com->argument1.value -= 128;
    } else if (line[0] == '1' && line[1] == '0') {
        decode_whole_rule(com, 4, 4, 8, 8, Mov3, line);
    } else {
        switch (get_substr_and_convert_to_int(8, 0, line)) {
            case 0:
                decode_whole_rule(com, 4, 8, 4, 12, Mov, line);
                break;
            case 1:
                decode_whole_rule(com, 4, 8, 4, 12, Mov1, line);
                break;
            case 2:
                decode_whole_rule(com, 4, 8, 4, 12, Mov2, line);
                break;
            case 4:
                decode_whole_rule(com, 4, 8, 0, 12, Push, line);
                break;
            case 5:
                decode_whole_rule(com, 4, 8, 0, 12, Pop, line);
                break;
            case 6:
                decode_whole_rule(com, 8, 8, 0, 0, Call, line);
                break;
            case 7:
                decode_whole_rule(com, 0, 0, 0, 0, Ret, line);
                break;
            case 8:
                decode_whole_rule(com, 4, 8, 4, 12, Add, line);
                break;
            case 9:
                decode_whole_rule(com, 4, 8, 4, 12, Sub, line);
                break;
            case 10:
                decode_whole_rule(com, 4, 8, 4, 12, Mul, line);
                break;
            case 11:
                decode_whole_rule(com, 4, 8, 4, 12, Div, line);
                break;
            case 12:
                decode_whole_rule(com, 4, 8, 4, 12, And, line);
                break;
            case 13:
                decode_whole_rule(com, 4, 8, 4, 12, Or, line);
                break;
            case 14:
                decode_whole_rule(com, 4, 8, 4, 12, Xor, line);
                break;
            case 15:
                decode_whole_rule(com, 4, 8, 0, 12, Not, line);
                break;
            case 16:
                decode_whole_rule(com, 4, 8, 4, 12, Shl, line);
                break;
            case 17:
                decode_whole_rule(com, 4, 8, 4, 12, Shr, line);
                break;
            case 18:decode_whole_rule(com, 0, 0, 0, 0, Reset, line);
                break;
            case 19:
                decode_whole_rule(com, 0, 0, 0, 0, Nop, line);
                break;
            case 20:
                decode_whole_rule(com, 8, 8, 0, 0, Jmp, line);
                com->argument1.value -= 128;
                break;
                /*case 21:
                    decode_whole_rule(com, 8, 4, 4, 12, Je, line);
                    com->argument1.value -= 128;
                    break;
                case 22:
                    decode_whole_rule(com, 8, 4, 4, 12, Jne, line);
                    com->argument1.value -= 128;
                    break;*/
            case 23:
                decode_whole_rule(com, 5, 8, 0, 0, In, line);
                break;
            case 24:
                decode_whole_rule(com, 5, 8, 0, 0, Out, line);
                break;
        }
    }
}

int get_substr_and_convert_to_int(unsigned int size, unsigned int offset, char* string) {
    char substr[10 + size];
    get_substring(size, offset, string, substr);
    char *ptr;
    long gettt = strtol(substr, &ptr, 2);
    return gettt;
}


void extract_whole_register_value(int *registers, int reg_num, int result) {
    div_t res_div = div(result, 256);
    registers[2*reg_num] = res_div.quot;
    registers[2*reg_num+ 1] = res_div.rem;
}

void execute_operation(int *registers, struct rule* com, char operation) {
    int result = calculate(registers, com, operation);
    extract_whole_register_value(registers, com->argument1.value, result);
}

int calculate(int *registers, struct rule* com, char operation) {
    int value1;
    int value2;
    value1 = registers[2*com->argument1.value]*256 + registers[2*com->argument1.value+1];
    value2 = registers[2*com->argument2.value]*256 + registers[2*com->argument2.value+1];
    switch (operation) {
        case '+':
            return value1 + value2;
        case '-':
            return value1 - value2;
        case '*':
            return value1 * value2;
        case '/':
            return value1 / value2;
        case '&':
            return value1 & value2;
        case '|':
            return value1 | value2;
        case 'X':
            return value1 ^ value2;
    }

    return 0;
}

void initialize_registers(int *registers) {
    for (size_t id = 0; id < 18; ++id) {
        registers[id] = 0;
    }
}

void print_registers_states(int *registers) {
    for (int i=0; i< 8; ++i) {
        printf("R%dH: %d ", i, registers[2*i]);
        printf("R%dL: %d ", i, registers[2*i+1]);

    }
    printf("\n");
}