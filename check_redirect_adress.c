#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
/**
 * Dosya yönlendirme için erişim kontrolü yapar
 * @param filename Dosya adı
 * @param mode Açılış modu (0: okuma, 1: yazma, 2: ekleme)
 * @param error_msg Hata mesajını alacak pointer
 * @return Erişim sağlanabilir ise 0, değilse -1 döner
 */
int check_redirect_access(char *filename, int mode, char **error_msg)
{
    int fd;
    
    if (mode == 0) // Okuma
    {
        if (file_exists(filename) == 0)
        {
            *error_msg = ": No such file or directory";
            return (-1);
        }
        
        if (check_access(filename, R_OK) == 0)
        {
            *error_msg = ": Permission denied";
            return (-1);
        }
        
        fd = open(filename, O_RDONLY);
        if (fd == -1)
        {
            *error_msg = ": No such file or directory";
            return (-1);
        }
        close(fd);
        return (0);
    }
    else if (mode == 1 || mode == 2) // Yazma veya ekleme
    {
        if (file_exists(filename) == 1 && check_access(filename, W_OK) == 0)
        {
            *error_msg = ": Permission denied";
            return (-1);
        }
        
        // Dosyayı açmayı dene
        if (mode == 1)
            fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else
            fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        
        if (fd == -1)
        {
            *error_msg = ": Cannot open file";
            return (-1);
        }
        
        close(fd);
        return (0);
    }
    
    *error_msg = ": Invalid redirect mode";
    return (-1);
}
