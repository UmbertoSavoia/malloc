#include "../include/libmalloc.h"

void    free_chunk(t_pages **list, void *ptr)
{
    t_pages *prev = *list;
    t_pages *cur = 0;
    t_pages *tmp = 0;

    if (ptr == prev->chunks[0].data) {
        *list = prev->next;
        munmap(prev, prev->chunks[0].size +
            sizeof(t_pages) + sizeof(t_pages));
    } else {
        cur = prev->next;
        while (cur) {
            if (ptr == cur->chunks[0].data) {
                tmp = cur->next;
                munmap(cur, cur->chunks[0].size +
                    sizeof(t_pages) + sizeof(t_pages));
                prev->next = tmp;
                cur = tmp;
                return ;
            }
            prev = cur;
            cur = cur->next;
        }
    }
}

void    set_unused_chunk(t_pages *list, void *ptr)
{
    while (list) {
        for (size_t i = 0; i < list->tot_chunks; ++i) {
            if (ptr == list->chunks[i].data) {
                list->chunks[i].size = 0;
                return ;
            }
        }
        list = list->next;
    }
}

void    free(void *ptr)
{
    t_pages **list = 0;

    if (!ptr)
        return ;
    pthread_mutex_lock(&mutex);
    if (!(list = get_list_from_ptr_data(ptr)))
        return ;
    if (*list == allocations.large)
        free_chunk(list, ptr);
    else
        set_unused_chunk(*list, ptr);
    pthread_mutex_unlock(&mutex);
}