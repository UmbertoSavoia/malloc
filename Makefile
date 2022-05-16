ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

TARGET	=	libft_malloc_$(HOSTTYPE).so
LINK	=	libft_malloc.so
CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror -fPIC -g
RM		=	rm -rf
SRC		=	$(wildcard ./src/*.c)
OBJ		=	$(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(TARGET)

$(TARGET) : $(OBJ)
	$(CC) $(CFLAGS) -shared $(OBJ) -o $(TARGET)
	ln -s $(TARGET) $(LINK)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(TARGET) $(LINK)

re: fclean all

.PHONY: all clean fclean re