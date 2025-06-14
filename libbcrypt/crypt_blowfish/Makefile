CC = gcc
AS = $(CC)
LD = $(CC)
RM = rm -f
CFLAGS = 
ASFLAGS = -c
LDFLAGS = -s

BLOWFISH_OBJS = \
	crypt_blowfish.o x86.o

CRYPT_OBJS = \
	$(BLOWFISH_OBJS) crypt_gensalt.o wrapper.o

all: $(CRYPT_OBJS) 

crypt_blowfish.o: crypt_blowfish.h
crypt_gensalt.o: crypt_gensalt.h
wrapper.o: crypt.h ow-crypt.h crypt_blowfish.h crypt_gensalt.h

.c.o:
	$(CC) -c $(CFLAGS) $*.c

.S.o:
	$(AS) $(ASFLAGS) $*.S

clean:
	$(RM) *.o 