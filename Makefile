CC=gcc
CPPFLAGS=
CFLAGS= -Wall -Wextra -std=c99 -g -o bomber
LDFLAGS=
LDLIBS=
 
SRC= map.c

OBJ= ${SRC:.c=.o} 
all: bomber

bomber: ${OBJ} 
 
clean:
	rm -f ${OBJ}
	rm -f *.o
	rm bomber
	clear
