NAME=minishell

SRCS= minishell.c parser.c dollar_parse.c 
CFLAGS=-Wall -Wextra -Werror
LIBFT=libft/libft.a

all: $(NAME)

$(NAME): $(SRCS) 
	make -C ./libft -s
	cc  -o  $(NAME)  $(CFLAGS) $(SRCS) -lreadline -lncurses $(LIBFT)

clean:
	make clean -C ./libft -s

fclean: clean
	make fclean -C ./libft -s
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re