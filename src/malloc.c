#include "../include/libmalloc.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

t_allocations   allocations = {
    .tiny = 0,
    .small = 0,
    .large = 0
};

void    *check_space(t_pages **list, size_t size, size_t type)
{
    t_pages *tmp = *list;

    if (!tmp)
        return add_pages(list, size, type);
    while (tmp) {
        if (tmp->tot_chunks > 0) {
            for (size_t i = 0; i < tmp->tot_chunks; ++i) {
                if (tmp->chunks[i].size == 0 ) {
                    tmp->chunks[i].size = size;
                    return tmp->chunks[i].data;
                }
            }
        }
        tmp = tmp->next;
    }
    return add_pages(list, size, type);
}

void    *malloc(size_t size)
{
    t_pages **list = 0;
    size_t  type = 0;
    void    *ret = 0;

    if (!size)
        return 0;
    pthread_mutex_lock(&mutex);
    list = get_list_from_size(size);
    type = get_type_from_size(size);
    if (!(ret = check_space(list, ALIGN(size), type)))
        return 0;
    pthread_mutex_unlock(&mutex);
    return ret;
}