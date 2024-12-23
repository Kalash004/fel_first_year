CC = gcc-11
FLAGS_BASIC = -Wall -Werror -std=c99
FLAGS_PED = $(FLAGS_BASIC) -pedantic -g  
FLAGS_NON_PED = -std=c99 -g
FLAGS_PROD = $(FLAGS_BASIC) -pedantic
OUT = -o a.out

a.out : main.c
	$(CC) main.c $(FLAGS_PED) $(OUT)