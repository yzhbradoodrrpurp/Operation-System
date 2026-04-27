CC = gcc
CFLAGS = -Wall -Wextra -g

.PHONY: all run hello_stages clean

all: myapp

myapp: myapp.o greeting.o
	$(CC) -o $@ $^

myapp.o: myapp.c greeting.h
	$(CC) $(CFLAGS) -c myapp.c -o myapp.o

greeting.o: greeting.c greeting.h
	$(CC) $(CFLAGS) -c greeting.c -o greeting.o

run: myapp
	./myapp

hello_stages: hello.c
	$(CC) -E hello.c -o hello.i
	$(CC) -S hello.c -o hello.s
	$(CC) -c hello.c -o hello.o
	$(CC) hello.c -o hello
	$(CC) -g hello.c -o hello_g

clean:
	rm -f *.o myapp hello hello_g hello.i hello.s
