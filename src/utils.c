#include "../include/libmalloc.h"

t_pages  **get_list_from_size(size_t size)
{
    if (size <= (size_t)TINY)
        return &(allocations.tiny);
    else if (size <= (size_t)SMALL)
        return &(allocations.small);
    return &(allocations.large);
}

size_t  get_type_from_size(size_t size)
{
    if (size <= (size_t)TINY)
        return TINY;
    else if (size <= (size_t)SMALL)
        return SMALL;
    return size;
}

t_pages **get_list_from_ptr_data(void *ptr)
{
    t_pages *tmp = 0;

    for (int i = 0; i < 3; ++i) {
        if (i == 0)
            tmp = allocations.tiny;
        else if (i == 1)
            tmp = allocations.small;
        else
            tmp = allocations.large;
        while (tmp) {
            for (size_t j = 0; j < tmp->tot_chunks; ++j) {
                if (ptr == tmp->chunks[j].data) {
                    if (i == 0)
                        return &(allocations.tiny);
                    else if (i == 1)
                        return &(allocations.small);
                    else
                        return &(allocations.large);
                }
            }
            tmp = tmp->next;
        }
    }
    return 0;
}

size_t  get_size_from_ptr(t_pages *list, void *ptr)
{
    while (list) {
        for (size_t i = 0; i < list->tot_chunks; ++i) {
            if (list->chunks[i].data == ptr)
                return list->chunks[i].size;
        }
        list = list->next;
    }
    return 0;
}

t_meta  *inizialize_new_pages(t_pages *list, size_t type)
{
    list->tot_chunks = DEFAULT_BLOCK;
    for (size_t i = 0; i < list->tot_chunks; ++i) {
        list->chunks[i].size = 0;
        list->chunks[i].data = (char *)list + sizeof(t_pages) + 
            (sizeof(t_meta) * DEFAULT_BLOCK) + i * type;
    }
    list->next = 0;
    return &(list->chunks[0]);
}

void    *add_large_pages(t_pages **list, size_t size)
{
    t_pages *tmp = *list;
    size_t tot_size = size + sizeof(t_pages) + sizeof(t_meta);

    if (!tmp) {
        if ((*list = MMAP(tot_size)) == MAP_FAILED)
            return 0;
        (*list)->tot_chunks = 1;
        (*list)->next = 0;
        (*list)->chunks[0].size = size;
        (*list)->chunks[0].data = (char *)(*list) + sizeof(t_pages) + 
            sizeof(t_meta);
        return (*list)->chunks[0].data;
    } else {
        for (; tmp->next; tmp = tmp->next);
        if ((tmp->next = MMAP(tot_size)) == MAP_FAILED)
            return 0;
        tmp->next->tot_chunks = 1;
        tmp->next->next = 0;
        tmp->next->chunks[0].size = size;
        tmp->next->chunks[0].data = (char *)tmp->next + sizeof(t_pages) + 
            sizeof(t_meta);
        return tmp->next->chunks[0].data;
    }
}

void    *add_pages(t_pages **list, size_t size, size_t type)
{
    t_pages *tmp = *list;
    t_meta *ret = 0;
    size_t tot_size = 0;
    size_t page_size = 0;

    if (type > (size_t)SMALL)
        return add_large_pages(list, size);
    page_size = getpagesize();
    tot_size = ((type + sizeof(t_meta)) * DEFAULT_BLOCK) + sizeof(t_pages);
    tot_size = ((tot_size / page_size) + 1) * page_size;
    if (!tmp) {
        if ((*list = MMAP(tot_size)) == MAP_FAILED)
            return 0;
        ret = inizialize_new_pages(*list, type);
        ret->size = size;
        return ret->data;
    } else {
        for (; tmp->next; tmp = tmp->next);
        if ((tmp->next = MMAP(tot_size)) == MAP_FAILED)
            return 0;
        ret = inizialize_new_pages(tmp->next, type);
        ret->size = size;
        return ret->data;
    }
}