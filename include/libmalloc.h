#ifndef LIBMALLOC_H

#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>

typedef struct  s_meta
{
    size_t size;
    void *data;
}               t_meta;

typedef struct  s_pages
{
    struct s_pages *next;
    size_t tot_chunks;
    t_meta chunks[];
}               t_pages;

typedef struct  s_allocations
{
    t_pages *tiny;
    t_pages *small;
    t_pages *large;
}               t_allocations;

extern t_allocations    allocations;
extern pthread_mutex_t  mutex;

#define ALIGN(x)        (((((x) - 1) >> 2) << 2) + 4)
#define TINY            (getpagesize() / 16)
#define SMALL           (getpagesize() / 4)
#define DEFAULT_BLOCK   100

#define MMAP(x) mmap(0, x, PROT_READ|PROT_WRITE, \
                    MAP_PRIVATE|MAP_ANONYMOUS, -1, 0)

/* realloc.c */
void    *realloc(void *ptr, size_t size);

/* free.c */
void    free(void *ptr);
void    set_unused_chunk(t_pages *list, void *ptr);
void    free_chunk(t_pages **list, void *ptr);

/* show_alloc_mem.c */
void    show_alloc_mem(void);

/* malloc.c */
void    *malloc(size_t size);
void    *check_space(t_pages **list, size_t size, size_t type);

/* utils.c */
t_pages **get_list_from_size(size_t size);
size_t  get_type_from_size(size_t size);
t_pages **get_list_from_ptr_data(void *ptr);
size_t  get_size_from_ptr(t_pages *list, void *ptr);
void    *add_pages(t_pages **list, size_t size, size_t type);
void    *add_large_pages(t_pages **list, size_t size);
t_meta  *inizialize_new_pages(t_pages *list, size_t type);

/* libc.c */
size_t  ft_strlen(const char *s);
void    ft_itoa(long num, long base_len, char *str);
void    ft_putstr(char *s);
void    ft_putchar(char c);
void    *ft_memcpy(void *dest, const void *src, size_t n);
void    *ft_memset(void *s, int c, size_t n);
void     small_printf(const char *fmt, ...);

#endif