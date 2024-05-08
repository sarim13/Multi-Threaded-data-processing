CC = gcc
CFLAGS = -Wall -Wextra -pthread
TARGET = Multi

all: $(TARGET)

$(TARGET): assignment4.c
	$(CC) $(CFLAGS) -o $(TARGET) Multi.c

clean:
	rm -f $(TARGET)
