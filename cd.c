#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void ft_pwd(void)
{
    char    cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("%s\n", cwd);
    else
        perror("pwd");
}
void ft_cd4(t_input *pro, char    *old_cwd, char  *cwd)
{
    char    *new_pwd;
    char    *new_oldpwd;

    new_pwd = malloc(ft_strlen(cwd) + 5);
    if (!new_pwd)
        return;
    ft_strlcpy(new_pwd, "PWD=", 5);
    ft_strlcat(new_pwd, cwd, ft_strlen(cwd) + 5);
    pro->env = ft_setenv(pro->env, new_pwd, 0);
    free(new_pwd);

    new_oldpwd = malloc(ft_strlen(old_cwd) + 8);
    if (!new_oldpwd)
        return;
    ft_strlcpy(new_oldpwd, "OLDPWD=", 8);
    ft_strlcat(new_oldpwd, old_cwd, ft_strlen(old_cwd) + 8);
    pro->env = ft_setenv(pro->env, new_oldpwd, 0);
    free(new_oldpwd);
}

void ft_cd3(t_input *pro, char *old_cwd, char  *cwd, char *path)
{
    if (chdir(path) != 0)
        return;
    if (getcwd(cwd, 1024) == NULL)
    {
        perror("cd");
        return;
    }
    ft_cd4(pro, old_cwd, cwd);
}

void ft_cd2(t_input *pro , char    *path)
{
    path = ft_getenv(pro->env, "OLDPWD");
    if (!path)
    {
        printf("cd: OLDPWD not set\n");
        return;
    }
    printf("%s\n", path);
}

void ft_cd(char **args, t_input *pro)
{
    char    *path;
    char    cwd[1024];
    char    old_cwd[1024];

    if (getcwd(old_cwd, sizeof(old_cwd)) == NULL)
    {
        perror("cd");
        return;
    }
    if (!args || !args[0])
    {
        path = ft_getenv(pro->env, "HOME");
        if (!path)
        {
            printf("cd: HOME not set\n");
            return;
        }
    }
    else if (ft_strncmp(args[0], "-", 2) == 0)
        ft_cd2(pro, path);
    else
        path = args[0];
    ft_cd3(pro, old_cwd, cwd, path);
}
