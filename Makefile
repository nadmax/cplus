CC   =  gcc

SRC  =  lib/int.c    \
		lib/list.c   \
		lib/new.c    \
		lib/node.c   \
		lib/delete.c \
		lib/point.c  \
		lib/vertex.c

OBJ  =  $(SRC:.c=.o)

AR = ar rc

CFLAGS = -I./include/ -Wall -Wextra -Wpedantic

LIB =  lib.a

all: $(LIB)

$(LIB): 
	make -C . build

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

build: $(OBJ)
	$(AR) $(LIB) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(LIB)

re: fclean all

.PHONY: all build clean fclean re
