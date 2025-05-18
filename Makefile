NAME=minishell


SRCS= minishell.c parser.c dollar_expand.c token_find.c operator_redirect.c arg_parse2.c execute.c built_in.c file_redirect.c env.c export.c cd.c ft_free.c echo_and_unset.c exit.c env_and_pwd.c file_control.c check_redirect_access.c check_command_access.c heredoc.c

CFLAGS=
LIBFT=libft/libft.a

all: $(NAME)

$(NAME): $(SRCS) 
	make -C ./libft -s
	gcc -o  $(NAME)  $(CFLAGS) $(SRCS) -lreadline -lncurses $(LIBFT)

clean:
	make clean -C ./libft -s

fclean: clean
	make fclean -C ./libft -s
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re