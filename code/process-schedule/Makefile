CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11
TARGET = process_schedule

.PHONY: all run clean

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
