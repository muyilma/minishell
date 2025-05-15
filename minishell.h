typedef struct s_pro
{
	char **str;
	char *outfile;
	char *infile;
	char *heradock;
	char *append_outfile;
} t_pro;

typedef struct s_input
{
	char *input;
	int after_str;
	char qut;
	int isprint;
	int pipe;
	int isalpha;
	int operator;
	int error;
	int dollar;
	int quotes;
	int exit_code;
	int redirect_error;
	char **env;
	t_pro **arg;
} t_input;


void	arg_convert(t_input *ipt, char *str, int k);
int	quotes_skip(char *str, int j);
void	ft_parser(t_input *input);
void	dollar_parse(t_input *input);
void	arg_parse(t_input *ipt, int len, int k);
void	ft_print_error(char *base, char *str, char **arg, int flag);
void	quotes_control(t_input *input);
void	ft_executer_free(t_input *input);



void	handle_redirections(t_pro *pro);
void	built_in(char **args, t_input *pro);
char	*pathc(char *cmd, char **envp, int i);;
int	execute_pipe(t_input *pro, int start_idx, int cmd_idx);
void	ft_pwd(void);
int	built_in2(char **args, t_input *pro, t_pro *arg);
void ft_echo(char **args, int i, int newline);
int	ft_unset(char **args, t_input *pro);
void	ft_env(char **env, char **args, t_input *pro);
int ft_exit(char **args);
int	ft_cd(char **args, t_input *pro);
int	ft_export(char **args, t_input *pro, int envflag);
char	**ft_setenv(char **env, char *variable, int e);
char	*ft_getenv(char **env, char *name);
int check_directory_access(char *path, char **error_msg);
char *check_command_access(char *cmd, char **env, char **error_msg);
int check_redirect_access(char *filename, int mode, char **error_msg);