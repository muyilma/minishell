#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

/**
 * İsmi verilen dosyanın varlığını kontrol eder
 * @param path Kontrol edilecek dosya/dizin yolu
 * @return Dosya varsa 1, yoksa 0 döner
 */
int file_exists(char *path)
{
    struct stat buffer;
    int result;
    
    result = stat(path, &buffer);
    if (result == 0)
        return (1);
    return (0);
}

/**
 * İsmi verilen yolun bir dizin olup olmadığını kontrol eder
 * @param path Kontrol edilecek yol
 * @return Dizin ise 1, değilse 0 döner
 */
int is_directory(char *path)
{
    struct stat path_stat;
    int result;
    
    result = stat(path, &path_stat);
    if (result != 0)
        return (0);
    
    if (S_ISDIR(path_stat.st_mode))
        return (1);
    return (0);
}

/**
 * Bir dosya üzerindeki izinleri kontrol eder
 * @param path Kontrol edilecek dosya yolu
 * @param mode İzin modu (F_OK: var mı, R_OK: okunabilir mi, W_OK: yazılabilir mi, X_OK: çalıştırılabilir mi)
 * @return İstenen izin varsa 1, yoksa 0 döner
 */
int check_access(char *path, int mode)
{
    int result;
    
    result = access(path, mode);
    if (result == 0)
        return (1);
    return (0);
}

/**
 * CD komutu için yolun uygun olup olmadığını kontrol eder
 * @param path Kontrol edilecek yol
 * @param error_msg Hata mesajını alacak pointer
 * @return Uygun ise 1, değilse 0 döner ve error_msg doldurulur
 */
int check_directory_access(char *path, char **error_msg)
{
    if (file_exists(path) == 0)
    {
        *error_msg = ": No such file or directory";
        return (0);
    }
    
    if (is_directory(path) == 0)
    {
        *error_msg = ": Not a directory";
        return (0);
    }
    
    if (check_access(path, R_OK) == 0 || check_access(path, X_OK) == 0)
    {
        *error_msg = ": Permission denied";
        return (0);
    }
    
    return (1);
}

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
            *error_msg = ": Cannot open file";
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
