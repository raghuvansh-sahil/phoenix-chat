CC = gcc
CFLAGS = -Wall -Wextra  -Icommon -Iserver
LDFLAGS =

COMMON_DIR = common
SERVER_DIR = server

COMMON_SRCS = $(COMMON_DIR)/user.c \
			  $(COMMON_DIR)/utils.c
SERVER_SRCS = $(SERVER_DIR)/command.c \
			  $(SERVER_DIR)/hashing.c \
			  $(SERVER_DIR)/input_parser.c \
			  $(SERVER_DIR)/main.c \
			  $(SERVER_DIR)/message.c \
			  $(SERVER_DIR)/server.c \
			  $(SERVER_DIR)/user_handler.c 

SRCS = $(COMMON_SRCS) $(SERVER_SRCS)
OBJS = $(SRCS:.c=.o)

TARGET = server_app

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)