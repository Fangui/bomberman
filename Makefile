CC=gcc
CPPFLAGS=
CFLAGS= -Wall -Wextra -std=c99 -O2
LDFLAGS=
LDLIBS=

SRC= map.c player.c vector.c queue.c queue2.c

OBJ= ${SRC:.c=.o}
all: bomber

bomber: ${OBJ}

clean:
	rm -f *.o *.d .*.swp
	rm bomber
	clear
