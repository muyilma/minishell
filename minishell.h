typedef struct s_input
{
    char *input;
    char qut;
    int isdigit;
    int isalpha;
    int backslash;
    int pipe;
    int error;
    int dot;
    int quotes;
} t_input;