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
char *check_command_access(char *cmd, char **env, char **error_msg)
{
    struct stat cmd_stat;
    char *cmd_path;
    int stat_result;
    
    // Eğer komut tam yol ise
    if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/') || 
        (cmd[0] == '.' && cmd[1] == '.' && cmd[2] == '/'))
    {
        if (file_exists(cmd) == 0)
        {
            *error_msg = ": No such file or directory";
            return (NULL);
        }
        
        stat_result = stat(cmd, &cmd_stat);
        if (stat_result == 0)
        {
            if (S_ISDIR(cmd_stat.st_mode))
            {
                *error_msg = ": Is a directory";
                return (NULL);
            }
            
            if (check_access(cmd, X_OK) == 0)
            {
                *error_msg = ": Permission denied";
                return (NULL);
            }
            
            return (ft_strdup(cmd));
        }
    }
    else
    {
        // PATH üzerinden komut aranır
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
