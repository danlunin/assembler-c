#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <math.h>
#include "mainFunctions.h"

void read_source_from_file(char *buffer, FILE *fp, long lSize) {
    if( 1!=fread( buffer , lSize, 1 , fp) )
        fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);
}

long get_file_size(FILE *fp) {
    long lSize;

    fseek( fp , 0L , SEEK_END);
    lSize = ftell( fp );
    rewind( fp );
    return  lSize;
}

/*int get_register_value(int *registers, int reg_num) {
    return registers[2*reg_num]*256 + registers[2*reg_num+1];
}*/



int calculate_one_register(int *registers, struct rule* com, char operation, int i, int is_ip) {
    int value1;
    if (is_ip) {
        value1 = i;
    } else {
        value1 = registers[2*com->argument1.value]*256 + registers[2*com->argument1.value+1];
    }
    switch (operation) {
        case '<':
            return value1 << com->argument2.value;
        case '>':
            return value1 >> com->argument2.value;
        case '`':
            return ~value1;
        case '|':
            return value1 + com->argument1.value;
    }

    return 0;
}

int apply_instruction(struct rule *com, int *registers, char* memory, int i, int *finished, int process_number) {
    //printf("Proccess: %d ", process_number);
    //print_registers_states(registers);
    char one_byte;
    int result;
    switch (com->command){
        case Je:
            //printf("JE to %d\n", com->argument1.value);
            if (registers[2*com->argument2.value] == 0 && registers[2*com->argument2.value + 1] == 0) {
                //JMP
                i = calculate_one_register(registers, com, '|', i, 1) - 1;
                //printf("JE JMP to %d\n", com->argument1.value);
            }
            break;
        case Jne:
            //printf("JNE to %d\n", com->argument1.value);
            if (registers[2*com->argument2.value] != 0 && registers[2*com->argument2.value + 1] != 0) {
                i = calculate_one_register(registers, com, '|', i, 1) - 1;
                //JMP
                //printf("JNE JMP to %d\n", com->argument1.value);
            }
            break;
        case Mov:
            registers[2*com->argument1.value] = registers[2*com->argument2.value];
            registers[2*com->argument1.value + 1] = registers[2*com->argument2.value + 1];
            //printf("MOV to %d from %d\n", com->argument1.value, com->argument2.value);
            break;
        case Mov1:
            memcpy(&registers[com->argument1.value], &memory[com->argument2.value], 2);
            //move_value_from_second_address(registers, com->argument1.value, com->argument2.value, memory);
            //printf("MOV1 to %d from %d\n", com->argument2.value, com->argument1.value);
            break;
        case Mov2:
            memcpy(&memory[com->argument1.value], &registers[com->argument1.value], 2);
            //printf("MOV2 to %d from %d\n", com->argument2.value, com->argument1.value);
            break;
        case Mov3:
            registers[com->argument1.value] = com->argument2.value;
           //printf("MOV3 imm %d to %d\n", com->argument2.value, com->argument1.value);
            break;
        case Push:
            result = 256*registers[7*2] + registers[7*2+1] - 2;
            extract_whole_register_value(registers, 7, result);
            memory[result] = registers[2*com->argument1.value];
            memory[result + 1] = registers[2*com->argument1.value + 1];
            //("PUSH: %d, Stack pointer: %d\n", com->argument1.value, result);
            break;
        case Pop:
            result = 256*registers[7*2] + registers[7*2 + 1];
            registers[2*com->argument1.value] = memory[result];
            registers[2*com->argument1.value + 1] = memory[result + 1];
            result += 2;
            extract_whole_register_value(registers, 7, result);
           //("POP: %d, Stack pointer: %d\n", com->argument1.value, result);
            break;
        case Call:
            //Extract stack pointer
            result = 256*registers[7*2] + registers[7*2 + 1] - 2;
            //Update stack pointer
            extract_whole_register_value(registers, 7, result);
            //Push ip value on stack
            div_t res_div = div(i, 256);
            memory[result] = res_div.quot;
            memory[result + 1] = res_div.rem;
            //printf("Pushing ip value: %d %d\n", result, result+1);
            //Increase ip by x
            i += com->argument1.value - 1;
            //printf("Call: %d, Stack pointer: %d\n", com->argument1.value, result);
            return i;
        case Ret:
            //Extract ip
            result = 256*registers[7*2] + registers[7*2 + 1];

            //printf("Popping ip value: %d %d\n", result, result+1);
            i = 256*memory[result] + memory[result + 1];
            //Increase sp
            result += 2;
            //Update sp
            extract_whole_register_value(registers, 7, result);
            //printf("RET: %d Stack pointer: %d To: %d\n", com->argument1.value, result, i);
            return i;
        case Add:
            execute_operation(registers, com, '+');
            //printf("ADD to %d from %d\n", com->argument2.value, com->argument1.value);
            break;
        case Sub:
            execute_operation(registers, com, '-');
            //printf("SUB to %d from %d\n", com->argument2.value, com->argument1.value);
            break;
        case Mul:
            execute_operation(registers, com, '*');
            //printf("MUL to %d from %d\n", com->argument2.value, com->argument1.value);
            break;
        case Div:
            if (registers[com->argument2.value*2] * 256 + registers[com->argument2.value*2 +1 ] == 0) {
                fprintf(stderr, "Division by zero. Process: %d", process_number);
                finished[process_number] = 1;
                return i;
            }
            execute_operation(registers, com, '/');
            //printf("DIV to %d from %d\n", com->argument2.value, com->argument1.value);
            break;
        case And:
            execute_operation(registers, com, '&');
            //printf("AND to %d from %d\n", com->argument2.value, com->argument1.value);
            break;
        case Or:
            execute_operation(registers, com, '|');
            //printf("OR to %d from %d\n", com->argument2.value, com->argument1.value);
            break;
        case Xor:
            execute_operation(registers, com, 'X');
            //printf("XOR to %d from %d\n", com->argument2.value, com->argument1.value);
            break;
        case In:
            //printf("In block Register: %d\n", com->argument1.value);
            //fgets(&one_byte, 1, stdin);
            read(STDIN_FILENO, &one_byte, 1);
            registers[com->argument1.value] = one_byte;
            //printf("STDIN: %d to: %d \n", one_byte, com->argument1.value);
            break;
        case Out:
            printf("%d \n", registers[com->argument1.value]);
            //printf("STDOUT: %d \n", registers[com->argument1.value]);
            break;
        case Reset:
            //printf("%d\n", registers[1]);
            fprintf(stderr, "%d", registers[1]);
            finished[process_number] = 1;
            break;
        case Not:
            result = calculate_one_register(registers, com, '~', i, 0);
            extract_whole_register_value(registers, com->argument1.value, result);
            //printf("BITWISE NO to %d \n", com->argument1.value);
            break;
        case Shl:
            result = calculate_one_register(registers, com, '<', i, 0);
            extract_whole_register_value(registers, com->argument1.value, result);
            //printf("SHL to %d from %d\n", com->argument1.value, com->argument2.value);
            break;
        case Shr:
            result = calculate_one_register(registers, com, '>', i, 0);
            extract_whole_register_value(registers, com->argument1.value, result);
            //printf("SHR to %d from %d\n", com->argument1.value, com->argument2.value);
            break;
        case Jmp:
            i = calculate_one_register(registers, com, '|', i, 1) - 1;
            //printf("JMP to %d\n", com->argument1.value);
        case Nop:
            break;
        case Cmpxchg:
            result = 256*registers[2*com->argument1.value] + registers[2*com->argument1.value + 1];
            int second_register = 256*registers[2*com->argument2.value] + registers[2*com->argument2.value + 1];
            if (memory[result] == second_register) {
                memcpy(&memory[result], &registers[com->argument3.value], 2);
                memcpy(&registers[com->argument3.value], &registers[com->argument2.value], 2);
            }
            //printf("CMPXCHG  R(%dX) R%dX R%dX\n", com->argument1.value, com->argument2.value, com->argument3.value);
    }
    return i;
}

int all_proccesses_are_finished(int *finished, int number_of_proccesses) {
    for (int i = 0; i < number_of_proccesses; ++i) {
        if (!finished[i])
            return 0;
    }
    //printf("\nAll proccesses are finished");
    return 1;
}

void launch_interpretation(char **file_paths, long *offsets, long number_of_steps, int number_of_proccesses,
        int max_per_proccess) {
    int registers[number_of_proccesses][16];

    int proccesses_indicies[number_of_proccesses];
    int counters[number_of_proccesses];
    int finished[number_of_proccesses];
    int sizes[number_of_proccesses];
    char memory[65536];
    FILE *f_in[number_of_proccesses];
    struct rule com;
    char command_binary_high[8+1];
    char command_binary_low[8+1];
    for (int i = 0; i < number_of_proccesses; ++i) {
        initialize_registers(registers[i]);
        f_in[i] = fopen( file_paths[i], "rb" );
        sizes[i] = get_file_size(f_in[i]);
        //printf("Init address %d: %ld\n", i, offsets[i]);
        //printf("File size %d \n", sizes[i]);
        //printf("File name %s \n", file_paths[i]);
        read_source_from_file( &memory[offsets[i]], f_in[i], sizes[i]);
        finished[i] = 0;
        proccesses_indicies[i] = 0;
        counters[i] = 0;

        extract_whole_register_value(registers[i], 7, offsets[i]);
    }
    while (!all_proccesses_are_finished(finished, number_of_proccesses)) {
        for (int proccess = 0; proccess < number_of_proccesses; ++proccess) {
            int k = 0;
            while (proccesses_indicies[proccess] <= (sizes[proccess]/2) && counters[proccess] < number_of_steps
                && k < max_per_proccess && !finished[proccess]) {
                ++k;
                //printf("Ip: %d, Proccess: %d Counter: %d Size of instructions: %d\n", proccesses_indicies[proccess],
                //        proccess, counters[proccess], sizes[proccess]);
                if (number_of_steps != 0 && counters[proccess] >= number_of_steps) {
                    finished[proccess] = 1;
                    break;
                }
                get_binary_with_current_length(8, memory[offsets[proccess]+2*proccesses_indicies[proccess]],
                        command_binary_high);
                get_binary_with_current_length(8, memory[offsets[proccess]+2*proccesses_indicies[proccess] + 1],
                        command_binary_low);
                //printf("Command binary high: %s\n", command_binary_high);
                //printf("Command binary low: %s\n", command_binary_low);
                char *result = malloc(strlen(command_binary_high) + strlen(command_binary_low) + 1);
                strcpy(result, command_binary_high);
                strcat(result, command_binary_low);
                decode_command(&com, result);
                proccesses_indicies[proccess] = apply_instruction(&com, registers[proccess], memory,
                        proccesses_indicies[proccess], finished, proccess);
                ++counters[proccess];
                proccesses_indicies[proccess] += 1;
            }
            if (finished[proccess])
                break;
            if (proccesses_indicies[proccess] >= (sizes[proccess]/2)) {
                finished[proccess] = 1;
                //printf("Instruction pointer reached the end of the program. Process: %d\n", proccess);
            }
            if ( counters[proccess] >= number_of_steps && number_of_steps != 0){
                finished[proccess] = 1;
                //printf("Limit of steps is reached. Limit: %ld Process: %d\n", number_of_steps, proccess);
            }


        }
    }

    for (int i = 0; i < number_of_proccesses; ++i) {
        fclose(f_in[i]);
    }
}

int main(int argc, char *argv[]) {

    char *p;
    long number_of_comands = strtol(argv[1], &p, 10);
    long offsets[(argc-2)/2];
    char** filePaths;
    filePaths = malloc(argc * sizeof *filePaths);
    //printf("Numbers of pr: %d\n", (argc-2)/2);
    for (int i = 0; i < (argc-2)/2; ++i) {
        offsets[i] = strtol(argv[3+2*i], &p, 10);
        //printf("Offset of pr: %ld\n", offsets[i]);
        filePaths[i] = malloc(strlen(argv[2 + 2*i])+1);
        strcpy(filePaths[i], argv[2 + 2*i]);
        //printf("Path of pr: %s\n", filePaths[i]);
    }
    launch_interpretation(filePaths, offsets, number_of_comands, (argc-2)/2, 10);
    return 0;
}