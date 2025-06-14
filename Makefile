CC = gcc
CFLAGS = -Wall -Wextra  -Icommon -Iserver -Iclient -Ilibbcrypt
LDFLAGS =

COMMON_SRCS = common/user.c common/utils.c

# Server
SERVER_SRCS = server/command.c server/file_handler.c server/hashing.c server/input_parser.c \
              server/main.c server/message.c server/server.c server/user_handler.c \
			  $(COMMON_SRCS)
SERVER_OBJS = $(SERVER_SRCS:.c=.o)
SERVER_EXEC = server_app

# Client
CLIENT_SRCS = client/client.c client/main.c \
              $(COMMON_SRCS)
CLIENT_OBJS = $(CLIENT_SRCS:.c=.o)
CLIENT_EXEC = client_app

.PHONY: all server client clean

all: libbcrypt server client

libbcrypt: libbcrypt/bcrypt.a

libbcrypt/bcrypt.a:
	@$(MAKE) -s -C libbcrypt

server: $(SERVER_EXEC)

$(SERVER_EXEC): $(SERVER_OBJS) libbcrypt/bcrypt.a
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) 

client: $(CLIENT_EXEC)

$(CLIENT_EXEC): $(CLIENT_OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(SERVER_OBJS) $(SERVER_EXEC) $(CLIENT_OBJS) $(CLIENT_EXEC)
	@$(MAKE) -s -C libbcrypt 