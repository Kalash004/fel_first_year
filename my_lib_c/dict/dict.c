# include <stdlib.h>

typedef struct {
    char *str_pntr; // maybe use void pointer ?
    size_t str_len;
} Value;

typedef struct {
    Value* key;
    Value* value;
} Cell;

typedef struct {
    Cell *cell_pntr;
    size_t dict_len;
} Dict;


char *key = "my_key";
char *value = "my_val";

Dict *create_dict(char *keys, char *values);


