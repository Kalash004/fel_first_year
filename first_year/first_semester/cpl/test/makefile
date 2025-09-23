program.out : main.o translations.o
	gcc-11 -Wall -Werror -Wextra -std=c99 -pedantic -lm main.o translations.o -o program.out

main.o: main.c
	gcc-11 -Wall -Werror -Wextra -std=c99 -pedantic -lm -c main.c -o main.o

translations.o: translation_operations.c
	gcc-11 -Wall -Werror -Wextra -std=c99 -pedantic -lm -c translation_operations.c -o translations.o