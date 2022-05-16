#include "../include/libmalloc.h"

void    show_alloc_mem(void)
{
    t_pages *tmp = 0;
    size_t total = 0;

    pthread_mutex_lock(&mutex);
    for (int i = 0; i < 3; ++i) {
        if (i == 0) {
            small_printf("TINY: 0x%x\n", allocations.tiny);
            tmp = allocations.tiny;
        } else if (i == 1) {
            small_printf("SMALL: 0x%x\n", allocations.small);
            tmp = allocations.small;
        } else {
            small_printf("LARGE: 0x%x\n", allocations.large);
            tmp = allocations.large;
        }
        while (tmp) {
            for (size_t j = 0; j < tmp->tot_chunks; ++j) {
                if (tmp->chunks[j].size > 0) {
                        small_printf(" 0x%x - 0x%x : %d bytes\n",
                            tmp->chunks[j].data,
                            tmp->chunks[j].data + tmp->chunks[j].size,
                            tmp->chunks[j].size);
                    total += tmp->chunks[j].size;
                }
            }
            tmp = tmp->next;
        }
    }
    small_printf("Total: %d bytes\n", total);
    pthread_mutex_unlock(&mutex);
}