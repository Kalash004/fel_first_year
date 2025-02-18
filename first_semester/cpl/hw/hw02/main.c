#include <stdio.h>
#include <stdlib.h>

// ------------------ Error codes and messaged definitions ------------------------

#define UNKNOWN_ERROR_CODE 1
#define MSG_UNKNOWN_ERROR "Error: unknown error"
#define WRONG_OPERATOR_CODE 2
#define MSG_WRONG_OPERATOR_ERROR "Error: wrong operator"
#define WRONG_OPERAND 3
#define MSG_WRONG_OPERAND "Error: wrong operands"

#define OK 0
#define TRUE 1
#define FALSE 0
// ------ Parameter handling functions -------------

/// @brief Checks if there is only one parameter
/// @param argc int Parameter count
/// @return Ok if there is only one parameter, otherwise UNKNOWN_ERROR_CODE
int check_parameters_length_is_one(int argc);
char read_parameter(char **argv);
int check_parameter(char type_of_output);

// ----- Operand handling -----------

/// @brief Reads input, checks for errors, sends to operand pointer
/// @param pOperand
void get_operand_in_int(long long int *pOperand);
int read_operand_in_char(char **pBuffer);
char get_operand_type(char arr[], char *pType);
int save_to_int(char *str, long long int *num, char type);
int check_operand(char arr[], char type);
int check_operand_decimal(char arr[]);
int check_operand_hex(char arr[]);
int check_operand_octal(char arr[]);
void is_negative(char buffer[], int *pB_negative_flag);
void cut_minus(char *buffer[], int buffer_size);

// ---- Operation handling ----------

void get_operation(char *pOperation);

// ------ Error handling functions ----------------

/// @brief Same as handle_non_fatal_error but exits the program at the end
/// @param code Error code number
void handle_fatal_error(int code);
/// @brief Finds error code, and prints it
/// @param code Error code number
void handle_non_fatal_error(int code);
void print_error_message(int code);
void get_error_code_to_message(int code, char buffer[], unsigned int buffer_size);

// ------- Math -------------------------------

void do_math(long long int *math_result, long long int operand_1, char operation, long long int operand_2);
long long int math_sum(long long int operand1, long long int operand2);
long long int math_subtraction(long long int operand1, long long int operand2);
long long int math_multiply(long long int operand1, long long int operand2);
long long int math_division(long long int operand1, long long int operand2);

// ------- Print output ------------------------

void print_result(long long int output, char type_of_output);
void print_hex_negative(long long int output);
void print_hex(long long int output);
void print_decimal(long long int output);
void print_octal(long long int output);
void print_octal_negative(long long int output);

// ----- Utils -----------------------

void print_with_newline(char msg[]);
/// @brief Checks if character is in the array
/// @param c char : character to look for
/// @param arr_size int : length of array to look in
/// @param arr char[] : Array to look in
/// @return int : 1 if is in array; 0 if not in array
int is_char_in_array(char c, int arr_size, char arr[]);
/// @brief Read char by char input with unknown size
/// @param pBuffer **char : pointer to where this returns allocated memory to
/// @return int : length of allocated memory
int read_unknown_size_input(char **pBuffer);
/// @brief Shifts array to the left ([1,2,3], [2,3])
/// @param shift_index int : from which index to shift
/// @param arr_size int : length of the array
/// @param buffer  **char : pointer to array that this shifts
void shift_char_array_left(int shift_index, int arr_size, char *buffer[]);

// ============================= Program =================================

int main(int argc, char **argv)
{
    // --------- obtain and check parameter --------

    int result_param_size_check = check_parameters_length_is_one(argc);
    if (result_param_size_check != OK)
    {
        handle_fatal_error(result_param_size_check);
    }
    char type_of_output = read_parameter(argv);
    int result_param_check = check_parameter(type_of_output);
    if (result_param_check != OK)
    {
        handle_fatal_error(result_param_check);
    }

    // --------- obtain and check operand_1 --------

    long long int operand_1;
    get_operand_in_int(&operand_1);

    // --------- obtain and check operation ---------

    char operation;
    get_operation(&operation);

    // --------- obtain and check operand_2 --------

    long long int operand_2;
    get_operand_in_int(&operand_2);

    // --------- do math ------------------

    long long int math_result;
    do_math(&math_result, operand_1, operation, operand_2);

    // --------- print result -------------

    print_result(math_result, type_of_output);
}

// ---------------- Parameter Handling ---------------------------------

int check_parameters_length_is_one(int argc)
{
    if (argc != 2)
        return UNKNOWN_ERROR_CODE;
    return OK;
}

int is_char_in_array(char c, int arr_size, char arr[])
{
    for (int i = 0; i < arr_size; ++i)
    {
        if (c == arr[i])
            return TRUE;
    }
    return FALSE;
}

char read_parameter(char **argv)
{
    return *argv[1];
}

int check_parameter(char parameter)
{
    if (!is_char_in_array(parameter, 3, (char[]){'d', 'x', 'o'}))
        return UNKNOWN_ERROR_CODE;
    return OK;
}

// ---------------- Operands Handling ----------------------------------

void get_operand_in_int(long long int *pOperand)
{
    char *buffer;
    int b_negative = FALSE;
    int buffer_size = read_operand_in_char(&buffer);
    is_negative(buffer, &b_negative);
    if (b_negative)
    {
        cut_minus(&buffer, buffer_size);
    }
    if (buffer_size <= 1)
    {

        free(buffer);
        handle_fatal_error(WRONG_OPERAND);
    }
    char type;
    int operand_type_check = get_operand_type(buffer, &type);
    if (operand_type_check != OK)
    {

        free(buffer);
        handle_fatal_error(operand_type_check);
    }
    int operand_check = check_operand(buffer, type);
    if (operand_check != OK)
    {
        free(buffer);
        handle_fatal_error(operand_check);
    }
    save_to_int(buffer, pOperand, type);
    if (b_negative)
        *pOperand = -1 * *pOperand;
    free(buffer);
}

int read_operand_in_char(char **pBuffer)
{
    return read_unknown_size_input(pBuffer);
}

int read_unknown_size_input(char **pBuffer)
{
    int allocated = 5;
    int used = 0;
    char *memory = malloc(allocated);
    char temp_char;
    do
    {
        temp_char = getchar();
        if (allocated - 2 == used)
        {
            allocated *= 2;
            memory = realloc(memory, allocated);
        }
        if (temp_char == '\n')
            break;
        memory[used] = temp_char;
        ++used;
    } while (temp_char != '\n' || temp_char != EOF);
    memory[used] = '\0';
    *pBuffer = memory;
    return used;
}

char get_operand_type(char arr[], char *pType)
{
    if (arr[1] == 'x')
    {
        *pType = 'x';
        return OK;
    }
    if (arr[0] != '0')
    {

        *pType = 'd';
        return OK;
    }
    if (arr[0] == '0')
    {
        *pType = 'o';
        return OK;
    }
    if (!is_char_in_array(*pType, 3, (char[]){'d', 'o', 'x'}))
    {
        return UNKNOWN_ERROR_CODE;
    }
    return OK;
}

int save_to_int(char *str, long long int *num, char type)
{
    switch (type)
    {
    case 'x':
        sscanf(str, "%llx", (long long unsigned int *)num);
        break;
    case 'o':
        sscanf(str, "%llo", (long long unsigned int *)num);
        break;
    case 'd':
        sscanf(str, "%lli", num);
        break;
    default:
        return UNKNOWN_ERROR_CODE;
        break;
    }
    return OK;
}

int check_operand(char arr[], char type)
{
    switch (type)
    {
    case 'o':
        return check_operand_octal(arr);
        break;
    case 'x':
        return check_operand_hex(arr);
        break;
    case 'd':
        return check_operand_decimal(arr);
        break;
    default:
        return UNKNOWN_ERROR_CODE;
        break;
    }
}

int check_operand_decimal(char arr[])
{
    int i = 0;
    char temp_char;
    do
    {
        temp_char = arr[i];
        if (temp_char == '\0')
            break;
        char template[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
        int template_length = sizeof(template) / sizeof(char);
        if (!is_char_in_array(temp_char, template_length, template))
        {
            return WRONG_OPERAND;
        }
        ++i;
    } while (temp_char != '\0');
    return OK;
}

int check_operand_hex(char arr[])
{
    int i = 2;
    char temp_char;
    do
    {
        temp_char = arr[i];
        if (temp_char == '\0')
            break;
        char template[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'A', 'B', 'C', 'D', 'E', 'F'};
        int template_length = sizeof(template) / sizeof(char);
        if (!is_char_in_array(temp_char, template_length, template))
        {
            return WRONG_OPERAND; // TODO: Change to right error code
        }
        ++i;
    } while (temp_char != '\0');
    return OK;
}

int check_operand_octal(char arr[])
{
    int i = 1;
    char temp_char;
    do
    {
        temp_char = arr[i];
        if (temp_char == '\0')
            break;
        char template[] = {'1', '2', '3', '4', '5', '6', '7', '0'};
        int template_length = sizeof(template) / sizeof(char);
        if (!is_char_in_array(temp_char, template_length, template))
        {
            return WRONG_OPERAND;
        }
        ++i;
    } while (temp_char != '\0');
    return OK;
}

void cut_minus(char **buffer, int buffer_size)
{
    shift_char_array_left(1, buffer_size, buffer);
}

void shift_char_array_left(int shift_index, int arr_size, char **buffer)
{
    int used = 0;
    for (int i = 0; i < arr_size; ++i)
    {
        if (i < shift_index)
        {
            continue;
        }
        char temp_char = (*buffer)[i];
        (*buffer)[used] = temp_char;
        ++used;
    }
    (*buffer)[used] = '\0';
}

void is_negative(char buffer[], int *pB_negative_flag)
{
    if (buffer[0] == '-')
    {
        *pB_negative_flag = TRUE;
        return;
    }
    *pB_negative_flag = FALSE;
}

// ---------------- Operation Handling --------------------------------

void get_operation(char *pOperation)
{
    char operation;
    scanf("%c", &operation);
    if (!is_char_in_array(operation, 4, (char[]){'-', '+', '*', '/'}))
    {
        handle_fatal_error(WRONG_OPERATOR_CODE);
    }
    *pOperation = operation;
    char c = getchar();
    if (c != '\n')
        handle_fatal_error(WRONG_OPERATOR_CODE);
}

// ---------------- Math ----------------------------------------------

void do_math(long long int *math_result, long long int operand_1, char operation, long long int operand_2)
{
    switch (operation)
    {
    case '+':
        *math_result = math_sum(operand_1, operand_2);
        break;
    case '*':
        *math_result = math_multiply(operand_1, operand_2);
        break;
    case '-':
        *math_result = math_subtraction(operand_1, operand_2);
        break;
    case '/':
        *math_result = math_division(operand_1, operand_2);
        break;
    default:
        break;
    }
}
long long int math_sum(long long int operand1, long long int operand2)
{
    return operand1 + operand2;
}
long long int math_subtraction(long long int operand1, long long int operand2)
{
    return operand1 - operand2;
}
long long int math_multiply(long long int operand1, long long int operand2)
{
    return operand1 * operand2;
}
long long int math_division(long long int operand1, long long int operand2)
{
    return operand1 / operand2;
}

// ---------------- Error Handling -------------------------------------

void handle_fatal_error(int code)
{
    print_error_message(code);
    exit(code);
}

void handle_non_fatal_error(int code)
{
    print_error_message(code);
}

void print_error_message(int code)
{
    char buffer[100];
    get_error_code_to_message(code, &buffer[0], 100);
    print_with_newline(buffer);
}

void get_error_code_to_message(int code, char buffer[], unsigned int buffer_size)
{
    switch (code)
    {
    case UNKNOWN_ERROR_CODE:
        snprintf(buffer, buffer_size, MSG_UNKNOWN_ERROR);
        break;
    case WRONG_OPERATOR_CODE:
        snprintf(buffer, buffer_size, MSG_WRONG_OPERATOR_ERROR);
        break;
    case WRONG_OPERAND:
        snprintf(buffer, buffer_size, MSG_WRONG_OPERAND);
        break;
    default:
        snprintf(buffer, buffer_size, "No error message for error code: %d\n", code);
    }
    return;
}

// ---------------- Printing -------------------------------------------

void print_with_newline(char msg[])
{
    int str_elem_id = 0;

    do
    {
        putchar(msg[str_elem_id]);
        ++str_elem_id;
    } while (msg[str_elem_id] != '\0');
    putchar('\n');
}

void print_result(long long int output, char type_of_output)
{
    switch (type_of_output)
    {
    case 'o':
        if (output < 0)
        {
            print_octal_negative(output * -1);
            break;
        }
        print_octal(output);
        break;
    case 'd':
        print_decimal(output);
        break;
    case 'x':
        if (output < 0)
        {
            print_hex_negative(output * -1);
            break;
        }
        print_hex(output);
        break;
    default:
        handle_fatal_error(UNKNOWN_ERROR_CODE);
        break;
    }
}
void print_hex(long long int output)
{
    printf("0x%llX\n", output);
}
void print_hex_negative(long long int output)
{
    printf("-0x%llX\n", output);
}
void print_decimal(long long int output)
{
    printf("%lli\n", output);
}
void print_octal(long long int output)
{
    printf("0%llo\n", output);
}
void print_octal_negative(long long int output)
{
    printf("-0%llo\n", output);
}
