#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char *name;
    int age;
    char *number;
} SContact;

typedef struct
{
    SContact *contacts_arr;
    int arr_length;
} SContact_arr;


typedef enum{
    ADD_CONTACT,
    VIEW_CONTACT,
    EXIT,
    END
} EActions;

char *action_name[] = {
    "Add contact - type 1",
    "View all contacts - type 2",
    "Exit the program - type 3"
};

int options[] = {
    1,
    2,
    3
};

int ask_do_display(SContact_arr *contacts);
void show_menu();
int read_menu_choice();
void action(EActions action, SContact_arr *contacts);
void add_contact(SContact_arr *contacts);
void get_contact_data(SContact *new_contact);
void print_contacts(SContact_arr *contacts);
void print_contact(SContact contact);
void exit_program();

int main(void) {
    SContact_arr contacts = { .arr_length=0 };
    int choice;
    do {
        choice = ask_do_display(&contacts);
    } while (choice != EXIT);
}

int ask_do_display(SContact_arr *contacts) {
    show_menu();
    int choice = read_menu_choice();
    // TODO: check if actions is in range
    action(choice-1, contacts);
    return choice-1;
}

void show_menu() {
    for (EActions action = ADD_CONTACT; action < END; ++action) {
        printf("%s\n", action_name[action]);
    }
}

int read_menu_choice() {
    int choice;
    scanf("%i",&choice);
    return choice;
}

void action(EActions action, SContact_arr *contacts) {
    switch (action)
    {
    case ADD_CONTACT:
        add_contact(contacts);
        break;
    case VIEW_CONTACT:
        print_contacts(contacts);
        break;
    case EXIT:
        exit_program();
        break;
    default:
        break;
    }
}

void add_contact(SContact_arr *contacts) {
    if (contacts->arr_length == 0){

        contacts->contacts_arr = malloc(contacts->arr_length + 1);
    }
    else {
        contacts->contacts_arr = realloc(contacts->contacts_arr, contacts->arr_length+1);
    }
    ++contacts->arr_length;
    SContact new_contact;
    get_contact_data(&new_contact);
    contacts->contacts_arr[contacts->arr_length] = new_contact;
}

void get_contact_data(SContact *new_contact){
    printf("Please enter a name\n");
    scanf("%s", new_contact->name);
    printf("Please enter a age\n");
    scanf("%i", &new_contact->age);
    printf("Please enter a number\n");
    scanf("%s", new_contact->number);
}

void print_contacts(SContact_arr *contacts){
    for (int i = 0; i < contacts->arr_length; ++i) {
        print_contact(contacts->contacts_arr[i]);
    }
}

void print_contact(SContact contact) {
    printf("---\nName %s\nAge %i\nNumber %s\n---\n", contact.name, contact.age, contact.number);
}

void exit_program() {
    printf("Ending");
}