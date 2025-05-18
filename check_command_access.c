#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
/**
 * Komut çalıştırma için erişim kontrolü yapar
 * @param cmd Çalıştırılacak komut
 * @param env Çevre değişkenleri
 * @param error_msg Hata mesajını alacak pointer
 * @return Komut çalıştırılabilir ise komutun tam yolu, değilse NULL döner
 */
char *check_command_cmd_access(char *cmd, char **env)
{
    struct stat cmd_stat;
    int stat_result;

    if (file_exists(cmd) == 0)
        return (": No such file or directory");
    stat_result = stat(cmd, &cmd_stat);
    if (stat_result == 0)
    {
        if (S_ISDIR(cmd_stat.st_mode))
             return (": Is a directory");
            
        if (check_access(cmd, X_OK) == 0)
        {
             return (": Permission denied");
        }
        return (NULL);
    }
}
char *check_command_access(char *cmd, char **env, char **error_msg)
{
    char *cmd_path;
    
    if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/') || 
        (cmd[0] == '.' && cmd[1] == '.' && cmd[2] == '/'))
    {
        *error_msg = check_command_cmd_access(cmd, env);
        if(*error_msg == NULL)
            return (ft_strdup(cmd));
        return (NULL);
    }
    else
    {
        cmd_path = pathc(cmd, env, -1);
        if (cmd_path == NULL)
        {
            *error_msg = ": command not found";
            return (NULL);
        }
        return (cmd_path);
    }
    
    *error_msg = ": Unknown error";
    return (NULL);
}
