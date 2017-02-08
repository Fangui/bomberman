CC=gcc
CPPFLAGS=
CFLAGS= -Wall -Wextra -std=c99 -O2 -o bomber
LDFLAGS=
LDLIBS=
 
SRC= map.c player.c vector.c

OBJ= ${SRC:.c=.o} 
all: bomber

bomber: ${OBJ} 
 
clean:
	rm -f ${OBJ}
	rm -f *.o
	rm -f .*.swp
	rm bomber
	clear
