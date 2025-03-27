typedef struct s_input
{
	char	*input;
	char	**env;
	int		after_str;
	char	qut;
	int		isprint;
	int		isalpha;
	// int		backslash;
	// int		pipe;
	int		operator;
	int		error;
	int		dollar;
	// int		dot;
	int		quotes;
}			t_input;

void		ft_parser(t_input *input);
void		dollar_parse(t_input *input);
