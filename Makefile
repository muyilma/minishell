NAME=minishell

SRCS= minishell.c parser/parser.c parser/dollar_expand.c parser/token_find.c parser/operator_redirect.c parser/token_create.c parser/memory_utils.c  \
executer/execute.c  executer/file_redirect.c executer/env.c executer/file_control.c executer/check_redirect_access.c  executer/heredoc.c\
built_in/export.c built_in/cd.c built_in/echo_and_unset.c built_in/exit.c built_in/env_and_pwd.c built_in/built_in.c

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