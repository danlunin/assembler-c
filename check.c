#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mainFunctions.h"


void extract_constant_test(char* constant, int size, int correct) {
    int answer = extract_constant(constant, size);
    if (answer != correct) {
        printf("Ectract constant test. Our  answer: %d \n", answer);
        printf("Correct  answer: %d \n", correct);
        exit(1);
    }

}

void check_pattern_test(char* string, char* pattern, unsigned int correct) {
    int answer = check_pattern(string, pattern);
    if (answer != correct) {
        printf("Pattern test. String: %s \n", string);
        printf("Pattern: %s \n", pattern);
        exit(1);
    }
}

void check_command_equality(struct rule* command, struct rule* correct_command, int number_of_arguments) {
    if ((number_of_arguments > 0 && command->argument1.value != correct_command->argument1.value) ||
        (number_of_arguments > 1 && command->argument2.value != correct_command->argument2.value) ||
        command->contains_label != correct_command->contains_label ||
        command->command != correct_command->command ||
        (number_of_arguments > 2 && command->argument3.value != correct_command->argument3.value)) {
        //printf("Line: %s \n", line);
        printf("Values: %d %d %d %d Number of arguments: %d\n", command->argument1.value, command->argument2.value,
               command->argument3.value, command->command, number_of_arguments);
        printf("Expected: %d %d %d %d\n", correct_command->argument1.value, correct_command->argument2.value,
               correct_command->argument3.value, correct_command->command);
        exit(1);
    }
}

void parse_line_into_command_test(char* line, struct rule* correct_command, int number_of_arguments) {
    struct rule *com = malloc(50);
    parse_line_into_command(line, com);
    check_command_equality(com, correct_command, number_of_arguments);
}

void create_command(struct rule* command, enum Command com, int arg1, int arg2, int arg3, int contains_label) {
    command->command = com;
    command->argument1.value = arg1;
    command->argument2.value = arg2;
    command->argument3.value = arg3;
    command->contains_label = contains_label;
}

void test_parsing_lines() {
    struct rule *com = malloc(50);
    com->contains_label = 0;
    char line[100];
    //line = (char *) malloc(20);
    strcpy(line, " mov R1X R2X");
    printf("LINE: %s\n", line);
    create_command(com, Mov, 1, 2, 0, 0);
    parse_line_into_command_test(line, com, 2);

    strcpy(line, " mov R5X (R2X)");
    create_command(com, Mov1, 5, 2, 0, 0);
    parse_line_into_command_test(line, com, 2);

    strcpy(line, " cmpxchg (R1X) R2X R5X");
    create_command(com, Cmpxchg, 1, 2, 5, 0);
    parse_line_into_command_test(line, com, 3);

    strcpy(line, " mov (R5X) R7X");
    create_command(com, Mov2, 5, 7, 0, 0);
    parse_line_into_command_test(line, com, 2);

    strcpy(line, " mov R5L #10");
    create_command(com, Mov3, 11, 10, 0, 0);
    parse_line_into_command_test(line, com, 2);

    strcpy(line, " push R3X");
    create_command(com, Push, 3, 0, 0, 0);
    parse_line_into_command_test(line, com, 1);

    strcpy(line, " pop R5X");
    create_command(com, Pop, 5, 0, 0, 0);
    parse_line_into_command_test(line, com, 1);

    strcpy(line, " ret");
    create_command(com, Ret, 0, 0, 0, 0);
    parse_line_into_command_test(line, com, 0);

    strcpy(line, " add R1X R2X");
    create_command(com, Add, 1, 2, 0, 0);
    parse_line_into_command_test(line, com, 2);

    strcpy(line, " sub R3X R4X");
    create_command(com, Sub, 3, 4, 0, 0);
    parse_line_into_command_test(line, com, 2);

    strcpy(line, " mul R1X R2X");
    create_command(com, Mul, 1, 2, 0, 0);
    parse_line_into_command_test(line, com, 2);

    strcpy(line, " div R1X R2X");
    create_command(com, Div, 1, 2, 0, 0);
    parse_line_into_command_test(line, com, 2);


    strcpy(line, " and R1X R2X");
    create_command(com, And, 1, 2, 0, 0);
    parse_line_into_command_test(line, com, 2);

    strcpy(line, " xor R1X R2X");
    create_command(com, Xor, 1, 2, 0, 0);
    parse_line_into_command_test(line, com, 2);

    strcpy(line, " or R1X R2X");
    create_command(com, Or, 1, 2, 0, 0);
    parse_line_into_command_test(line, com, 2);

    strcpy(line, " not R5X");
    create_command(com, Not, 5, 0, 0, 0);
    parse_line_into_command_test(line, com, 1);

    strcpy(line, " shl R1X #12");
    create_command(com, Shl, 1, 12, 0, 0);
    parse_line_into_command_test(line, com, 2);

    strcpy(line, " shr R1X #13");
    create_command(com, Shr, 1, 13, 0, 0);
    parse_line_into_command_test(line, com, 2);

    strcpy(line, " reset");
    create_command(com, Reset, 1, 0, 0, 0);
    parse_line_into_command_test(line, com, 0);

    strcpy(line, " nop");
    create_command(com, Nop, 1, 0, 0, 0);
    parse_line_into_command_test(line, com, 0);

    strcpy(line, " jmp");
    create_command(com, Jmp, 1, 0, 0, 0);
    parse_line_into_command_test(line, com, 0);

    strcpy(line, "label: mov R2X R3X");
    create_command(com, Mov, 2, 3, 0, 1);
    parse_line_into_command_test(line, com, 2);

    //strcpy(line, " je label R3X");
    //create_command(com, Je, 2, 3, 0, 0);
    //parse_line_into_command_test(line, com, 1);

    //strcpy(line, " jne label R3X");
    //create_command(com, Jne, 2, 3, 0, 0);
    //parse_line_into_command_test(line, com, 1);

    strcpy(line, " in R3L");
    create_command(com, In, 7, 0, 0, 0);
    parse_line_into_command_test(line, com, 1);

    strcpy(line, " out R3H");
    create_command(com, Out, 6, 0, 0, 0);
    parse_line_into_command_test(line, com, 1);
}

void test_check_patterns() {
    check_pattern_test("R7X", "R*X", 1);
    check_pattern_test("R9X", "R*X", 0);
    check_pattern_test("R7H", "R*#", 1);
    check_pattern_test("R4L", "R*#", 1);
    check_pattern_test("R4L", "R*X", 0);
}

void test_extract_constant() {
    extract_constant_test("#12", 4, 12);
    extract_constant_test("#12", 8, 12);
    extract_constant_test("#300", 4, -200);
    extract_constant_test("#17", 4, -200);
    extract_constant_test("#17", 8, 17);
}

void compare_strings_test(char *encoded, char *correct, struct rule* com) {
    if (strcmp(encoded, correct) != 0) {
        printf("Encoding problem\n");
        printf("EncodedR: %s\n", encoded);
        printf("EncodedC: %s\n", correct);
        exit(1);
    }
}

void test_decoding(struct rule *com) {

    struct rule correct_com;

    com->contains_label = 0;
    create_command(&correct_com, Mov, 1, 3, 0, 0);
    decode_command(com, "0000000000010011");
    check_command_equality(com, &correct_com, 2);

    create_command(&correct_com, Mov3, 1, 12, 0, 0);
    decode_command(com, "1001000100001100");
    check_command_equality(com, &correct_com, 2);

    create_command(&correct_com, Cmpxchg, 1, 2, 3, 0);
    decode_command(com, "1110000001010011");
    check_command_equality(com, &correct_com, 3);
}

void check_register_values(int *registers, int register_num, int expected_value) {
    int real_value = registers[2*register_num]*256 + registers[2*register_num + 1];
    if (real_value != expected_value) {
        printf("Check register value. Error: %d %d\n", real_value, expected_value);
        exit(1);
    }
}

void test_executing(struct rule *com) {
    com->contains_label = 0;
    int registers[16];
    initialize_registers(registers);


    create_command(com, Add, 1, 2, 0, 0);
    registers[3] = 12;
    registers[5] = 12;
    //print_registers_states(registers);
    execute_operation(registers, com, '+');
    //print_registers_states(registers);
    check_register_values(registers, 1, 24);

    create_command(com, Add, 1, 2, 0, 0);
    extract_whole_register_value(registers, 1, 1300);
    extract_whole_register_value(registers, 2, 1300);
    //print_registers_states(registers);
    execute_operation(registers, com, '+');
    //print_registers_states(registers);
    check_register_values(registers, 1, 2600);

    create_command(com, Mul, 1, 2, 0, 0);
    extract_whole_register_value(registers, 1, 12);
    extract_whole_register_value(registers, 2, 12);
    //print_registers_states(registers);
    execute_operation(registers, com, '*');
    //sprint_registers_states(registers);
    check_register_values(registers, 1, 144);

    free(com);
}

void test_encoding(struct rule *com) {
    com->contains_label = 0;
    char binary_str[17];
    struct rule all_commands[1000];
    create_command(com, Mov, 1, 3, 0, 0);
    parse_command_into_binary(com, binary_str, all_commands, 0, 0);
    compare_strings_test(binary_str, "0000000000010011", com);

    create_command(com, Mov3, 1, 12, 0, 0);
    parse_command_into_binary(com, binary_str, all_commands, 0, 0);
    compare_strings_test(binary_str, "1001000100001100", com);

    create_command(com, Cmpxchg, 1, 2, 4, 0);
    parse_command_into_binary(com, binary_str, all_commands, 0, 0);
    compare_strings_test(binary_str, "1110000001010100", com);

}

void RunUnitTests() {
    struct rule *com = malloc(50);

    test_check_patterns();

    test_parsing_lines();

    test_extract_constant();

    test_encoding(com);

    test_decoding(com);

    test_executing(com);

}

int main() {
    RunUnitTests();
    printf("All tests are passed.");
    return 0;
}

