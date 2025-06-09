# Compiler
CC = gcc

# Compiler flags
CFLAGS = -I../common -Wall -Wextra -Wunused-macros

# Source files
SRCS = main.c server.c ../common/utils.c command_parser.c hashing.c ../common/user_handler.c client_handler.c message.c 

# Object files
OBJS = $(SRCS:.c=.o)

# Output file name
TARGET = server

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