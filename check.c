#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mainFunctions.h"


void extract_constant_test(char* constant, int size, int correct) {
    int answer = extract_constant(constant, size);
    if (answer != correct) {
        printf("Our  answer: %d \n", answer);
        printf("Correct  answer: %d \n", correct);
        exit(1);
    }

}

void check_pattern_test(char* string, char* pattern, unsigned int correct) {
    int answer = check_pattern(string, pattern);
    if (answer != correct) {
        printf("String: %s \n", string);
        printf("Pattern: %s \n", pattern);
        exit(1);
    }
}

void parse_line_into_command_test(char* line, struct rule* correct_command) {
    struct rule command;
    parse_line_into_command(line, &command);
    if (command.argument1.value != correct_command->argument1.value ||
        command.argument2.value != correct_command->argument2.value ||
        command.contains_label != correct_command->contains_label ||
        command.command != correct_command->command) {
        printf("Line: %s \n", line);
        exit(1);
    }
}

void create_command(struct rule* command, enum Command com, int arg1, int arg2, int contains_label) {
    command->command = com;
    command->argument1.value = arg1;
    command->argument2.value = arg2;
    command->contains_label = contains_label;
}


void RunUnitTests() {
    extract_constant_test("#12", 4, 12);
    extract_constant_test("#12", 8, 12);
    extract_constant_test("#300", 4, -200);
    extract_constant_test("#17", 4, -200);
    extract_constant_test("#17", 8, 17);


    check_pattern_test("R7X", "R*X", 1);
    check_pattern_test("R9X", "R*X", 0);
    check_pattern_test("R7H", "R*#", 1);
    check_pattern_test("R4L", "R*#", 1);
    check_pattern_test("R4L", "R*X", 0);

    struct rule com;
    create_command(&com, Mov, 1, 2, 0);
    //char line[] =" mov R1X R2X";
    //parse_line_into_command_test(line, &com);
}

int main() {
    RunUnitTests();
    printf("All tests are passed.");
    return 0;
}

