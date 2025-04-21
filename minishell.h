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
	int		isalpha;
	int		operator;
	int		error;
	int		dollar;
	int		quotes;
	t_pro	**arg;
}			t_input;



void		ft_parser(t_input *input);
void		dollar_parse(t_input *input);
void	arg_parse(t_input *ipt,int len,int k);