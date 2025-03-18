#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include "minishell.h"


void read_line(t_input *input)
{
    input->input = NULL;
    input->isdigit = 0;
    input->isalpha = 0;
    input->backslash = 0;
    input->pipe = 0;
    input->error = 0;
    input->dot = 0;
    input->quotes = 0;
    input->input = readline("yaz:");
    add_history(input->input);
}

void empty_line(t_input *input)
{
    int i;

    i = 0;
    while (input->input[i])
    {
        if (input->input[i] != ' ')
            input->isdigit++;
        i++;
    }
}

void dollar_parse(t_input *input)
{
    
}

void quotes(t_input *input)
{
    int i;

    i = -1;
    empty_line(input);
    if (input->isdigit == 0)
    {
        input->error = 1;
        return;
    }
    while (input->input[++i])
    {
        if (input->input[i] == 34 || input->input[i] == 39)
        {
            input->quotes++;
            input->qut=input->input[i];
            i++;
            while (input->input[i] != input->qut && input->input[i] != '\0')
                i++;
            if (input->input[i] == input->qut)
                input->quotes++;
        }
    }
    if (input->quotes%2!=0)
        input->error=1;
    dollar_parse(input);
}


int main()
{
    t_input *input;

    while (1)
    {
        input = malloc(sizeof(t_input));
        read_line(input);

        quotes(input);
        if (input->error == 0)
        {
            printf("%s  ", input->input);
            printf("%d\n", input->quotes);
            free(input->input);
            free(input);
        }
        else
        {
            printf("CAlistin\n");
            free(input->input);
            free(input);
        }
    }
}
