
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef __BOOK_H__
#define __BOOK_H__
 
#include <stdio.h>
 
#define MAX_NAME_SIZE 19
 
struct Book {
    int year;
    char title[MAX_NAME_SIZE];
    char* abstract;
};
typedef struct Book Book;
 
 
struct BookList {
    Book** books;
    size_t size;
};
typedef struct BookList BookList;
 
Book* create_book(int year, const char* title, const char* abstract);
void destroy_book(Book* b);
void print_book(Book* b);
 
BookList* create_book_list();
void destroy_book_list(BookList* bl);
void add_book(BookList* bl, Book* b);
void print_book_list(BookList* bl);
 
 
#endif 


int main(void) {

}
 
Book* create_book(int year, const char* title, const char* abstract) {
    Book* p = (Book*)malloc(sizeof(Book));
    if (p == NULL) {
        printf("Error: malloc failed in create_book\n");
        return NULL;
    }
    p->year = year;
 
    for (size_t i = 0; i < MAX_NAME_SIZE; i++) {
        p->title[i] = '\0'; // erase "dangerous" leftover data
    }
    for (size_t i = 0; i < MAX_NAME_SIZE-1; i++) {
        p->title[i] = title[i];
        if (title[i] == '\0') {
            break;
        }
    }
 
    if (abstract == NULL) {
        p->abstract = NULL;
    } else {
        p->abstract = (char*)malloc(strlen(abstract) + 1);
        if (p->abstract == NULL) {
            printf("Error: malloc abstract failed in create_book\n");
            free(p);
            return NULL;
        }
        strcpy(p->abstract, abstract);
    }
    return p;
}
 
void destroy_book(Book* p) {
    free(p->abstract);
    free(p);
}
 
void print_book(Book* p) {
    printf("Title: %s, Publication year: %d, Abstract: %s\n", p->title, p->year, p->abstract);
}
 
BookList* create_book_list() {
    BookList* pl = (BookList*)malloc(sizeof(BookList));
    if (pl == NULL) {
        printf("Error: malloc failed in create_book_list\n");
        return NULL;
    }
    pl->books = NULL;
    pl->size = 0;
    return pl;
}
 
void add_book(BookList* pl, Book* p) {
    Book** people_ = (Book**) realloc(pl->books, (pl->size + 1) * sizeof(Book*));
    if (people_ == NULL) {
        printf("Warning: Adding failed, no modification is being done.\n");
        return;
    }
    pl->books = people_;
    (pl->books)[pl->size] = p;
    pl->size++;
}
 
void destroy_book_list(BookList* pl) {
    for (size_t i = 0; i < pl->size; i++) {
        destroy_book(pl->books[i]);
        pl->books[i] = NULL;
    }
    free(pl->books);
    free(pl);
}
 
void print_book_list(BookList* pl) {
    for (size_t i = 0; i < pl->size; i++) {
        printf("Book %zu:\n", i);
        print_book(pl->books[i]);
    }
}