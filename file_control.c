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


