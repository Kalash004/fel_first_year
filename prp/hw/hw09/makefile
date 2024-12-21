CC = gcc-11
CFLAGS = -g -pedantic -Wall -Werror -std=gnu99 
LDFLAGS = -lm

# Target: build a.out from graph.c
a.out: graph.o
	$(CC) graph.o -o a.out $(LDFLAGS)

# Target: compile graph.c into graph.o
graph.o: graph.c graph.h
	$(CC) $(CFLAGS) -c graph.c -o graph.o

# Optional clean target to remove build artifacts
clean:
	rm -f a.out graph.o