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
int check_parameter(char parameter);

// ----- Operand handling -----------

void get_operand_in_int(int *pOperand);
int read_operand_in_char(char **pBuffer);
char get_operand_type(char arr[], char *pType);
int save_to_int(char *str, int *num, char type);
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
void handle_non_fatal_error(int code);
void print_error_message(int code);
void get_error_code_to_message(int code, char buffer[], unsigned int buffer_size);

// ----- Utils -----------------------

void print_with_newline(char msg[]);
int is_char_in_array(char c, int arr_size, char arr[]);
int read_unknown_size_input(char **pBuffer);
void shift_char_array_left(int shift_index, int arr_size, char *buffer[]);

// ============================= Program =================================

// int main(int argc, char **argv)
// {
//     // --------- obtain and check parameter --------

//     int result_param_size_check = check_parameters_length_is_one(argc);
//     if (result_param_size_check != OK)
//     {
//         handle_fatal_error(result_param_size_check);
//     }
//     char parameter = read_parameter(argv);
//     int result_param_check = check_parameter(parameter);
//     if (result_param_check != OK)
//     {
//         handle_fatal_error(result_param_check);
//     }

//     // --------- obtain and check operand_1 --------

//     int operand_1;
//     get_operand_in_int(&operand_1);

//     // --------- obtain and check operation ---------

//     char operation;
//     get_operation(&operation);

//     // --------- obtain and check operand_2 --------

//     int operand_2;
//     get_operand_in_int(&operand_2);

//     // --------- do math ------------------
// }

int main(void)
{
    int operand_1;
    get_operand_in_int(&operand_1);
    printf("%d", operand_1);
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

void get_operand_in_int(int *pOperand)
{
    char *buffer;
    int b_negative = FALSE;
    int buffer_size = read_operand_in_char(&buffer);
    is_negative(buffer, &b_negative);
    if (b_negative)
    {
        cut_minus(&buffer, buffer_size);
    }
    if (buffer_size < 1)
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
    } while (temp_char != '\n');
    memory[used + 1] = '\0';
    *pBuffer = memory;
    return used + 1;
}

char get_operand_type(char arr[], char *pType)
{
    if (arr[1] == 'x')
        *pType = 'x';
    if (arr[0] != '0')
        *pType = 'd';
    if (arr[0] == '0')
        *pType = 'o';
    if (!is_char_in_array(*pType, 3, (char[]){'d', 'o', 'x'}))
    {
        return UNKNOWN_ERROR_CODE;
    }
    return OK;
}

int save_to_int(char *str, int *num, char type)
{
    unsigned int unsigned_num;
    switch (type)
    {
    case 'x':
        sscanf(str, "%x", &unsigned_num);
        break;
    case 'o':
        sscanf(str, "%o", &unsigned_num);
        break;
    case 'd':
        sscanf(str, "%d", num);
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
        if (!is_char_in_array(temp_char, 10, (char[]){'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'}))
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
        if (!is_char_in_array(temp_char, 10, (char[]){'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'A', 'B', 'C', 'D', 'E', 'F'}))
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
        if (!is_char_in_array(temp_char, 10, (char[]){'1', '2', '3', '4', '5', '6', '7', '0'}))
        {
            return WRONG_OPERAND;
        }
        ++i;
    } while (temp_char != '\0');
    return OK;
}

void cut_minus(char **buffer, int buffer_size)
{ // <--------
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
