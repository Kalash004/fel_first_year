
#include <stdio.h>
/*
    This code is commented in english not because i used chatgpt
    but because i like to comment code in english
*/

#define ERR_INPUT_OUT_OF_BOUNDS 100
#define OK 0

typedef struct Stats
{
    int sum;
    int input_count;
    int positive_nums_count;
    int negative_nums_count;
    int even_nums_count;
    int non_even_nums_count;
    int minimum; // assigning this to outer limits of input range
    int maximum; // assigning this to outer limits of input range
    float positive_to_count_rate;
    float negative_to_count_rate;
    float even_to_count_rate;
    float non_even_to_count_rate;
    float avg;
} Stats;

// defining functions
#pragma region
int read_input(int *ret_num);
void calculate_stats(int input, Stats *stats);
void print_stats(Stats stats);
void print(char *msg);
#pragma endregion

int main(void)
{

// setting up stats structure
#pragma region
    Stats stats;
    // Defining variables for stats
    stats.sum = 0;
    stats.input_count = 0;
    stats.positive_nums_count = 0;
    stats.negative_nums_count = 0;
    stats.even_nums_count = 0;
    stats.non_even_nums_count = 0;
    stats.minimum = 10001;  // assigning this to outer limits of input range
    stats.maximum = -10001; // assigning this to outer limits of input range
#pragma endregion

    // Accepting and counting input
    int result = 0;          // return value from read_input - to check if any error happened
    int b_is_first_loop = 1; // bool
    while (result == 0)
    {
        int input;
        result = read_input(&input);
        if (result == ERR_INPUT_OUT_OF_BOUNDS) // catching out of bounds error and returning it
            return ERR_INPUT_OUT_OF_BOUNDS;
        if (result != 0) // end of reading
            break;
        if (!b_is_first_loop) // skips printing on first lap so we dont get output: ", 321, 12"
            printf(", ");
        printf("%i", input);
        calculate_stats(input, &stats);
        b_is_first_loop = 0;
    }
    stats.positive_to_count_rate = (float)stats.positive_nums_count / stats.input_count * 100;
    // 0 doesnt count as negative nor positive
    stats.negative_to_count_rate = (float)stats.negative_nums_count / stats.input_count * 100;
    stats.even_to_count_rate = (float)stats.even_nums_count / stats.input_count * 100;
    // 0 isnt even nor non even
    stats.non_even_to_count_rate = (float)stats.non_even_nums_count / stats.input_count * 100;
    stats.avg = (float)stats.sum / stats.input_count;
    print_stats(stats);
}

/*
    This reads input and checks if rules were not broken
    _returns:
            if out of range or input count != 1 : 100
            if ok : 0
    _prints:
            if input count != 1 : stderr, "Chyba nacitani vstupu!\n"
            if input out of range : stderr, "Error: Vstup je mimo interval!\n"
*/
int read_input(int *ret_num)
{
    int num;
    int scn_res = scanf("%d", &num);
    // if (scn_res == )
    if (scn_res != 1) // if amount of input isnt 1 return 1 to indicate the end of reading
    {
        return 1;
    }
    if (num >= 10001 || num <= -10001)
    {
        print("\nError: Vstup je mimo interval!");
        return ERR_INPUT_OUT_OF_BOUNDS;
    }
    *ret_num = num;
    return OK;
}

void calculate_stats(int input, Stats *stats)
{
    (*stats).input_count++;
    if (input > 0)
        stats->positive_nums_count++;
    if (input < 0)
        stats->negative_nums_count++;
    if (input % 2 == 0)
    {
        stats->even_nums_count++;
    }
    else
    {
        stats->non_even_nums_count++;
    }
    if (input > (*stats).maximum)
        stats->maximum = input;
    if (input < (*stats).minimum)
        stats->minimum = input;
    stats->sum += input;
}

void print_stats(Stats stats)
{
    putchar('\n'); // printing \n to after input printout
    printf("Pocet cisel: %d\n", stats.input_count);
    printf("Pocet kladnych: %d\n", stats.positive_nums_count);
    printf("Pocet zapornych: %d\n", stats.negative_nums_count);
    printf("Procento kladnych: %.2f\n", stats.positive_to_count_rate);
    printf("Procento zapornych: %.2f\n", stats.negative_to_count_rate);
    printf("Pocet sudych: %d\n", stats.even_nums_count);
    printf("Pocet lichych: %d\n", stats.non_even_nums_count);
    printf("Procento sudych: %.2f\n", stats.even_to_count_rate);
    printf("Procento lichych: %.2f\n", stats.non_even_to_count_rate);
    printf("Prumer: %.2f\n", stats.avg);
    printf("Maximum: %d\n", stats.maximum);
    printf("Minimum: %d\n", stats.minimum);
}

/**
 * This prints msg in a specific format ( with \n at the end)
 *
 * _parameters:
 * *msg - pointer to first char of the string to print
 */
void print(char *msg)
{
    int str_elem_id = 0;

    while (*(msg + str_elem_id) != '\0')
    {
        putchar(msg[str_elem_id]);
        ++str_elem_id;
    }
    putchar('\n');
}
