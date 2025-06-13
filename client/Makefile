# Compiler
CC = gcc

# Compiler flags
CFLAGS = -I../common

# Source files
SRCS = main.c client.c ../common/utils.c ../common/user.c

# Object files
OBJS = $(SRCS:.c=.o)

# Output file name
TARGET = client

# Default target
all: $(TARGET)

# How to build the target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to build object files from source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(TARGET) $(OBJS)