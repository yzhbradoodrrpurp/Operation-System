CC = gcc
CFLAGS = -Wall -Wextra -g

.PHONY: all run_fork_wait run_exec_ls clean

all: fork_wait_demo fork_exec_ls

fork_wait_demo: fork_wait_demo.c
	$(CC) $(CFLAGS) -o $@ $<

fork_exec_ls: fork_exec_ls.c
	$(CC) $(CFLAGS) -o $@ $<

run_fork_wait: fork_wait_demo
	./fork_wait_demo

run_exec_ls: fork_exec_ls
	./fork_exec_ls

clean:
	rm -f fork_wait_demo fork_exec_ls
