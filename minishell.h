extern int g_signal_exit;

typedef struct s_cmd
{
	char **str;
	char *outfile;
	char *infile;
	char *heradock;
	char *append_outfile;
} t_cmd;

typedef struct s_shell
{
	char *input;
	char **new_env;
	char *temp;
	int original_stdin;
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
	t_cmd **arg;
} t_shell;


void	arg_convert(t_shell *ipt, char *str, int k);
int	quotes_skip(char *str, int j,int flag,int *quotes);
void	ft_parser(t_shell *input);
void	dollar_expand(t_shell *input);
void	token_create(t_shell *ipt, int len, int k,int flag);
void	quotes_control(t_shell *input);
char	*redirect_find(char **redirect, char *str, int *i, int *flag);

void	ft_print_error(char *base, char *str, char **arg, int flag);
void	ft_executer_free(t_shell *input);
char **copy_env(char **env, int b);


void	handle_redirections(t_shell *shell,t_cmd *pro);
void	built_in(char **args, t_shell *pro);
char	*pathc(char *cmd, char **envp, int i);;
int	execute_pipe(t_shell *pro, int start_idx);
void	ft_pwd(void);
int	built_in2(char **args, t_shell *pro, t_cmd *arg);
void ft_echo(char **args);
int	ft_unset(char **args, t_shell *pro);
void	ft_env(char **args, t_shell *pro);
int ft_exit(char **args, t_shell *pro);
int	ft_cd(char **args, t_shell *pro);
int	ft_export(char **args, t_shell *pro);
char	**ft_setenv(char **env, char *variable);
char	*ft_getenv(char **env, char *name);
int check_directory_access(char *path, char **error_msg);
char *check_command_access(char *cmd, char **env, char **error_msg);
int check_redirect_access(char *filename, char **error_msg);
int check_access(char *path, int mode);
int file_exists(char *path);
int redirect_heredoc_to_stdin(char *delimiter);
int heredoc_control(t_cmd *pro);
int check_redirect_access_input(char *filename, char **error_msg);
int redirect_heredoc_write(int *fd ,char *delimiter, int  heredoc_status);
void error_and_allocate(t_shell *pro, int exit_code);
void	ft_free(char **str);