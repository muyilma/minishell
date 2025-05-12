typedef struct s_pro
{
	char	**str;
	char	*outfile;
	char	*infile;
	char	*heradock;
	char	*append_outfile;
}			t_pro;

typedef struct s_input
{
	char	*input;
	int		after_str;
	char	qut;
	int		isprint;
	int		pipe;
	int		isalpha;
	int		operator;
	int		error;
	int		dollar;
	int		quotes;
	int 	exit_code;
	char **env;
	t_pro	**arg;
}			t_input;

int	execute_pipe(t_input *pro, int start_idx, int cmd_idx);
void		arg_convert(t_input *ipt, char *str, int k);
int			quotes_skip(char *str, int j);
void		ft_parser(t_input *input);
void		dollar_parse(t_input *input);
void		arg_parse(t_input *ipt, int len, int k);
void		handle_redirections(t_pro *pro);
int	built_in(char **args, t_input *pro);
char		*pathc(char *cmd, char **envp);
char	**ft_setenv(char **env, char *variable, int e);
char *ft_getenv(char **env, char *name);
void	ft_export(char **args, t_input *pro , int envflag);
void	ft_print_sorted_env(char **env);
void ft_cd(char **args, t_input *pro);
void ft_pwd(void);
void ft_exit(char **args);