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
	t_pro	**arg;
}			t_input;

void	execute_pipe(t_pro **input, int s, int start, int i);
void		arg_convert(t_input *ipt, char *str, int k);
int			quotes_skip(char *str, int j);
void		ft_parser(t_input *input);
void		dollar_parse(t_input *input);
void		arg_parse(t_input *ipt, int len, int k);
void	ft_echo(char **args);
void	handle_redirections(t_pro *pro);
