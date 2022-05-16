#include "../include/libmalloc.h"

void    *realloc(void *ptr, size_t size)
{
    t_pages **list = 0;
    void    *ret = 0;
    size_t  type = 0;

    if (!ptr)
        return malloc(size);
    pthread_mutex_lock(&mutex);
    if (!(list = get_list_from_ptr_data(ptr)))
        return 0;
    type = get_type_from_size(size);
    if (!(ret = check_space(list, ALIGN(size), type)))
        return 0;
    ft_memcpy(ret, ptr, get_size_from_ptr(*list, ptr));
    if (*list == allocations.large)
        free_chunk(list, ptr);
    else
        set_unused_chunk(*list, ptr);
    pthread_mutex_unlock(&mutex);
    return ret;
}