a.out : main.o simage.o
	gcc-11 -Wall -Werror -Wextra -std=c99 -pedantic -g main.o simage.o -C -lm -o a.out

main.o : main.c
	gcc-11 -Wall -Werror -Wextra -std=c99 -pedantic -g -c main.c -o main.o

simage.o : simage.c
	gcc-11 -Wall -Werror -Wextra -std=c99 -pedantic -g -c simage.c -o simage.o