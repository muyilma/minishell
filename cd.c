#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

// pwd komutu - mevcut çalışma dizinini yazdırır
void ft_pwd(void)
{
    char    cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("%s\n", cwd);
    else
        perror("pwd");
}

// cd komutu - dizini değiştirir
void ft_cd(char **args, t_input *pro)
{
    char    *path;
    char    cwd[1024];
    char    old_cwd[1024];
    char    *new_pwd;
    char    *new_oldpwd;

    // Mevcut dizini sakla
    if (getcwd(old_cwd, sizeof(old_cwd)) == NULL)
    {
        perror("cd");
        return;
    }

    // Argüman yoksa HOME dizinine git
    if (!args || !args[0])
    {
        path = ft_getenv(pro->env, "HOME");
        if (!path)
        {
            printf("cd: HOME not set\n");
            return;
        }
    }
    // "-" argümanı varsa OLDPWD dizinine git
    else if (ft_strncmp(args[0], "-", 2) == 0)
    {
        path = ft_getenv(pro->env, "OLDPWD");
        if (!path)
        {
            printf("cd: OLDPWD not set\n");
            return;
        }
        printf("%s\n", path); // cd - komutu dizini yazdırmalı
    }
    // Diğer durumlarda belirtilen dizine git
    else
        path = args[0];

    // Dizin değiştir
    if (chdir(path) != 0)
    {
        perror("cd");
        return;
    }

    // Yeni dizini al
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("cd");
        return;
    }

    // PWD ve OLDPWD ortam değişkenlerini güncelle
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
// exit komutu - shell'den çıkış yapar
void ft_exit(char **args)
{
    int exit_status;

    printf("exit\n"); // exit komutunu görünür yapar

    // Argüman yoksa 0 ile çık
    if (!args || !args[0])
    {
        exit(0);
    }
    
    // İlk argümanı sayıya çevir
    exit_status = ft_atoi(args[0]);
    
    // Birden fazla argüman varsa hata ver
    if (args[1])
    {
        printf("exit: too many arguments\n");
        return; // Çıkmadan devam et
    }
    
    // Sayı değilse hata ver
    int i = 0;
    while (args[0][i])
    {
        if (!ft_isdigit(args[0][i]) && !(i == 0 && (args[0][i] == '-' || args[0][i] == '+')))
        {
            printf("exit: %s: numeric argument required\n", args[0]);
            exit(255); // Sayısal olmayan argüman için 255 ile çık
        }
        i++;
    }
    
    // Exit status 8 bit ile sınırlı (0-255)
    exit(exit_status & 0xFF);
}