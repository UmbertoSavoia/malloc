#include "../include/libmalloc.h"

size_t  ft_strlen(const char *s)
{
    size_t ret = 0;

    while (s && *s++)
        ret++;
    return (ret);
}

long    nbrlen(long num, long base_len)
{
    long count = 0;
    long u_num = num < 0 ? -(long)num : (long)num;

    if(num == 0)
        return 1;
    while(u_num > 0) {
        count++;
        u_num = u_num / base_len;
    }
    return count;
}

void    ft_itoa(long num, long base_len, char *str)
{
    long len_num = nbrlen(num, base_len);
    char *base = (base_len == 10) ? "0123456789" : "0123456789abcdef";
    long u_num = num < 0 ? -(long)num : (long)num;

    if (num == 0 && len_num == 1) {
        str[0] = '0';
        str[1] = 0;
    } else {
        if (num < 0)
            str[0] = '-';
        for (long i = num < 0 ? len_num : len_num - 1; 
                u_num != 0; --i, u_num /= base_len) {
            str[i] = base[u_num % base_len];
        }
        str[num < 0 ? len_num + 1 : len_num] = 0;
    }
}

void    ft_putstr(char *s)
{
    write(1, s, ft_strlen(s));
}

void    ft_putchar(char c)
{
    write(1, &c, 1);
}

void    *ft_memcpy(void *dest, const void *src, size_t n)
{
    char        *temp;
    const char  *s;

    if (!dest && !src)
        return (0);
    s = src;
    temp = dest;
    while (n--)
        *temp++ = *s++;
    return (dest);
}

void    *ft_memset(void *s, int c, size_t n)
{
    char *temp;

    temp = s;
    while (n--)
        *temp++ = (unsigned char)c;
    return (s);
}

void     small_printf(const char *fmt, ...)
{
    char    tmp[4096] = {0};
    size_t  num = 0;
    va_list ap;

    va_start(ap, fmt);
    while (*fmt) {
        if (*fmt == '%') {
            ++fmt;
            if (*fmt == 'x') {
                num = va_arg(ap, size_t);
                ft_itoa(num, 16, tmp);
                ft_putstr(tmp);
                ft_memset(tmp, 0, sizeof(tmp));
            } else if (*fmt == 'd') {
                num = va_arg(ap, size_t);
                ft_itoa(num, 10, tmp);
                ft_putstr(tmp);
                ft_memset(tmp, 0, sizeof(tmp));
            }
        } else {
            ft_putchar(*fmt);
        }
        ++fmt;
    }

    va_end(ap);
}