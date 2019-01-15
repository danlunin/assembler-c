#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>

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


void get_substring(unsigned int size, unsigned int offset, char* string, char * substr) {
    int c = 0 ;
    while (c < size) {
        substr[c] = string[offset+c];
        c++;
    }
    substr[c] = '\0';
}

int get_substr_and_convert_to_int(unsigned int size, unsigned int offset, char* string) {
    char substr[10 + size];
    get_substring(size, offset, string, substr);
    char *ptr;
    long gettt = strtol(substr, &ptr, 2);
    return gettt;
}

void decode_whole_rule(struct rule *com, int firstSize, int firstOffset, int secondSize, int secondOffset,
                       enum Command command, char *line) {
    com->argument1.value = get_substr_and_convert_to_int(firstSize, firstOffset, line);
    com->command = command;
    com->argument2.value = get_substr_and_convert_to_int(secondSize, secondOffset, line);
    //printf("Command: %d, Arg1: %d, Arg2: %d\n", com->command, com->argument1.value, com->argument2.value);

}

void decode_command(struct rule *com, char * line) {
    printf("Line: %s\n", line);
    if (line[0] == '1' && line[1] == '1') {
        decode_whole_rule(com, 8, 4, 4, 12, Je, line);
    } else if (line[0] == '0' && line[1] == '1') {
        decode_whole_rule(com, 8, 4, 4, 12, Jne, line);
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
                //decode_whole_rule(com, 8, 8, 0, 0, Call, line);
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
                //decode_whole_rule(com, 4, 8, 0, 12, Not, line);
                break;
            case 17:
                //decode_whole_rule(com, 4, 8, 0, 12, Not, line);
                break;
            case 18:decode_whole_rule(com, 0, 0, 0, 0, Reset, line);
                break;
            case 19:
                //decode_whole_rule(com, 0, 0, 0, 0, Reset, line);
                break;
            case 20:
                //decode_whole_rule(com, 4, 8, 0, 12, Not, line);
                break;
            case 21:
                //decode_whole_rule(com, 4, 8, 0, 12, Not, line);
                break;
            case 22:
                //decode_whole_rule(com, 4, 8, 0, 12, Not, line);
                break;
            case 23:
                decode_whole_rule(com, 5, 8, 0, 0, In, line);
                break;
            case 24:
                decode_whole_rule(com, 5, 8, 0, 0, Out, line);
                break;
        }
    }
}

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

void extract_whole_register_value(int *registers, struct rule* com, int result) {
    registers[2*com->argument1.value] = result / 256;
    registers[2*com->argument1.value + 1] = result - registers[2*com->argument1.value];
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
            return value1 + value2;
        case '&':
            return value1 & value2;
        case '|':
            return value1 | value2;
        case 'X':
            return value1 ^ value2;
    }

    return 0;
}

void execute_operation(int *registers, struct rule* com, char operation) {
    int result = calculate(registers, com, operation);
    extract_whole_register_value(registers, com, result);
}

void move_value_from_second_address(int *registers, int value1, int value2) {
    //printf(" XXX ");
    //printf(" %d ", registers[2*value1]*256 + 2*value1 + 1);
    //char p = *(char*)0xff73000;
    //char *p = registers[2*value1]*256 + 2*value1 + 1;
    registers[2*value2] = 1;//p[0];
    registers[2*value2 + 1] =1;// p[1];
}

void print_registers_states(int *registers) {
    for (int i=0; i< 8; ++i) {
        printf("R%dH: %d ", i, registers[2*i]);
        printf("R%dL: %d ", i, registers[2*i+1]);

    }
    printf("\n");
}

void apply_instruction(struct rule *com, int *registers) {
    print_registers_states(registers);
    char one_byte;
    switch (com->command){
        case Je:
            printf("JE to %d\n", com->argument1.value);
            if (registers[2*com->argument2.value] == 0 && registers[2*com->argument2.value + 1] == 0) {
                //JMP
                printf("JMP to %d\n", com->argument1.value);
            }
            break;
        case Jne:
            printf("JE to %d\n", com->argument1.value);
            if (registers[2*com->argument2.value] != 0 && registers[2*com->argument2.value + 1] != 0) {
                //JMP
                printf("JMP to %d\n", com->argument1.value);
            }
            break;
        case Mov:
            registers[2*com->argument1.value] = registers[2*com->argument2.value];
            registers[2*com->argument1.value + 1] = registers[2*com->argument2.value + 1];
            printf("MOV to %d from %d\n", com->argument1.value, com->argument2.value);
            break;
        case Mov1:
            move_value_from_second_address(registers, com->argument2.value, com->argument1.value);
            printf("MOV1 to %d from %d\n", com->argument2.value, com->argument1.value);
            break;
        case Mov2:
            move_value_from_second_address(registers, com->argument1.value, com->argument2.value);
            printf("MOV2 to %d from %d\n", com->argument2.value, com->argument1.value);
            break;
        case Mov3:
            registers[com->argument1.value] = com->argument2.value;
            //registers[2*com->argument2.value + 1] = registers[2*com->argument1.value + 1];
            printf("MOV3 imm %d to %d\n", com->argument2.value, com->argument1.value);
            break;
        case Push:
            registers[7*2 + 1] -= 1;
            //mov (R7X) RiX.
            printf("PUSH: %d\n", com->argument1.value);
            break;
        case Pop:
            registers[7*2 + 1] ++;
            //mov RiX (R7X), R7X++
            printf("POP: %d\n", com->argument1.value);
            break;
        case Call:
            registers[7*2 + 1] -= 1;
            //mov (R7X) IP
            registers[8*2 + 1] += com->argument1.value;
            printf("CALL: %d\n", com->argument1.value);
            break;
        case Ret:
            registers[7*2 + 1] ++;
            //mov IP (R7X), R7X++
            printf("RET: %d\n", com->argument1.value);
            break;
        case Add:
            execute_operation(registers, com, '+');
            printf("ADD to %d from %d\n", com->argument2.value, com->argument1.value);
            break;
        case Sub:
            execute_operation(registers, com, '-');
            printf("SUB to %d from %d\n", com->argument2.value, com->argument1.value);
            break;
        case Mul:
            execute_operation(registers, com, '*');
            printf("MUL to %d from %d\n", com->argument2.value, com->argument1.value);
            break;
        case Div:
            execute_operation(registers, com, '/');
            printf("DIV to %d from %d\n", com->argument2.value, com->argument1.value);
            break;
        case And:
            execute_operation(registers, com, '&');
            printf("AND to %d from %d\n", com->argument2.value, com->argument1.value);
            break;
        case Or:
            execute_operation(registers, com, '|');
            printf("OR to %d from %d\n", com->argument2.value, com->argument1.value);
            break;
        case Xor:
            execute_operation(registers, com, 'X');
            printf("XOR to %d from %d\n", com->argument2.value, com->argument1.value);
            break;
        case In:
            printf("In block Register: %d\n", com->argument1.value);
            read(STDIN_FILENO, &one_byte, 1);
            registers[com->argument1.value] = one_byte;
            printf("STDIN: %d to: %d \n", one_byte, com->argument1.value);
            break;
        case Out:
            printf("%d \n", registers[com->argument1.value]);
            printf("STDOUT: %d \n", registers[com->argument1.value]);
            break;
        case Reset:
            printf("Exit code: %d\n", registers[15]);
            exit(registers[1]);
        case Not:
            //TO DO
            break;
        case Shl:
        case Shr:
        case Nop:
        case Jmp:
            break;

    }
}

void initialize_registers(int *registers) {
    for (size_t id = 0; id < 18; ++id) {
        registers[id] = 0;
    }
}

void launch_interpretation(char *file_path, long offset, long number_of_steps) {
    int registers[18];
    FILE *f_in;
    struct rule com;
    char substr[16];
    char *buffer;
    int i = 0;
    initialize_registers(registers);
    f_in = fopen( file_path, "r" );
    int size = get_file_size(f_in);
    buffer = (char*)calloc( 1, size+1 );
    //read_source_from_file( &buffer[1000], f_in, size);
    read_source_from_file( &buffer[offset], f_in, size);
    int counter = 0;
    while (size != 0) {
        if (number_of_steps !=0 && counter >= number_of_steps) {
            exit(0);
        }
        size -= 16;
        get_substring(16, i, buffer, substr);
        i += 16;
        decode_command(&com, substr);
        apply_instruction(&com, registers);
        ++counter;
    }
    fclose(f_in);
}

int main(int argc, char *argv[]) {
    /*if (argc != 4) {
        exit(1);
    }
    char *p;
    long offset = strtol(argv[3], &p, 10);
    long number_of_comands = strtol(argv[1], &p, 10);
    launch_interpretation(argv[2], offset, number_of_comands);
    return 0;*/
    char *p;
    long offset = 0;
    long number_of_comands =100;
    launch_interpretation("out.txt", offset, number_of_comands);
    return 0;
}