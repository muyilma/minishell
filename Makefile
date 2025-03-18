NAME=minishell

SRCS= minishell.c
CFLAGS=-Wall -Wextra -Werror


$(NAME): $(SRCS) 
	cc  -o  $(NAME)  $(CFLAGS) $(SRCS) -lreadline -lncurses

all: $(NAME)

clean:
	

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re