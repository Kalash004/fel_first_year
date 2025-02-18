test.x: testing.o my_str.o
	gcc testing.o my_str.o -Wall -Werror -pedantic -std=c99 -lm  -o test.x

testing.o: testing.c
	gcc -c testing.c -Wall -Werror -pedantic -std=c99 -lm -o testing.o

my_str.o: my_str.c
	gcc -c my_str.c -Wall -Werror -pedantic -std=c99 -lm -o my_str.o
