CC = gcc-11
FLAGS_BASIC =  -std=c99
FLAGS_PED = $(FLAGS_BASIC) -pedantic -g  
FLAGS_NON_PED = -std=c99 -g
FLAGS_PROD = $(FLAGS_BASIC) -pedantic

multi.out : more_threads.c queue.o
	$(CC) more_threads.c $(FLAGS_PED) -C queue.o -o multi.out

one.out : one_thread.c
	$(CC) one_thread.c $(FLAGS_PED) -C queue.o -o one.out

queue.o : queue.h queue.c
	$(CC) queue.c $(FLAGS_PED) -c -o queue.o