#include <param_dealing.h>

void deal_with_arguments(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        printf("Argument #%i: %s", i, argv[i]);
    }
}