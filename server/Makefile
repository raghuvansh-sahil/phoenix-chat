# Compiler
CC = gcc

# Compiler flags
CFLAGS = 

# Source files
SRCS = main.c server.c ../common/utils.c 

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