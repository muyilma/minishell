NAME=minishell


SRCS= minishell.c parser/parser.c parser/dollar_expand.c parser/token_find.c parser/operator_redirect.c parser/token_create.c parser/ft_free.c  executer/execute.c executer/built_in.c executer/file_redirect.c executer/env.c executer/export.c executer/cd.c executer/echo_and_unset.c executer/exit.c executer/env_and_pwd.c executer/file_control.c executer/check_redirect_access.c  executer/heredoc.c

CFLAGS=-Wall -Wextra -Werror
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