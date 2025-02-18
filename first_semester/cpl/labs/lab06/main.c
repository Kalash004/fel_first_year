# include <stdlib.h>
# include <stdio.h>
#include <string.h>

typedef struct {
    char *name; // to not play with memalloc
    int age;
} Person;

void get_x_people(FILE *f, int count, Person **ret_to);

Person *get_person(FILE *f);

void get_name(FILE *f, char *return_pointr);

unsigned int get_age(FILE *f);

void print_people(Person **people, int len);

void print_person(Person p);


int main(void) {
    char *path = "./somedata.txt";
    FILE *f = fopen(path, "r");
    Person *people[9];
    int len = 9;
    get_x_people(f, 8, people);
    print_people(people, len);
}

void get_x_people(FILE *f, int count, Person **ret_to) {
    for (int i = 0; i < count; ++i) {
        if (feof(f)) 
            break;
        ret_to[i] = get_person(f);
    }
}

Person *get_person(FILE *f) {
    char name_surname[100]; // to not play with memalloc
    get_name(f, name_surname);
    unsigned int age = get_age(f);
    Person *person = malloc(sizeof(Person));
    person->name = (char *)malloc(100);
    memcpy(person->name,name_surname,100);
    person->age = age;
    print_person(*person);
    return person;
}

void get_name(FILE *f,char *return_pointr) {
    fgets(return_pointr, 99, f);
}

unsigned int get_age(FILE *f) {
    int *age;
    fscanf(f, "%i\n", age);
    return *age;
}

void print_people(Person **people, int len) {
    for (int i = 0; i < len; ++i) {
        Person p = *people[i];
        print_person(p);
    }
}

void print_person(Person p) {
    printf("%s %i \n", p.name, p.age);
}