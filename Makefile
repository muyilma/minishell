NAME=minishell

SRCS= minishell.c parser.c dollar_parse.c arg_parse.c arg_parse2.c execute.c built_in.c file_redirect.c
CFLAGS=-g
LIBFT=libft/libft.a

all: $(NAME)

$(NAME): $(SRCS) 
	make -C ./libft -s
	gcc -g   -o  $(NAME)  $(CFLAGS) $(SRCS) -lreadline -lncurses $(LIBFT)

clean:
	make clean -C ./libft -s

fclean: clean
	make fclean -C ./libft -s
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re