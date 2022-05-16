#define _GNU_SOURCE
#include <stddef.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>

void* (*_malloc)(size_t size);
void (*show_alloc_mem)(void);
void (*_free)(void *);
void* (*_realloc)(void *, size_t);

int	main(void)
{
	void *handle = dlopen("./libft_malloc_x86_64.so", RTLD_LAZY);
	if (!handle) return 1;
	_malloc = dlsym(handle, "malloc");
	show_alloc_mem = dlsym(handle, "show_alloc_mem");
	_free = dlsym(handle, "free");
	_realloc = dlsym(handle, "realloc");

	char *a = _malloc(128);
	a[100] = 'A';

	char *b = _malloc(4096);
	b[1024] = 'B';

	char *c = _malloc(5000);
	b[128] = 'C';

	char *d = _malloc(1024);
	d[1000] = 'D';

	char *e = _malloc(1024);
	d[128] = 'E';

	show_alloc_mem();
	_free(b);
	_free(a);
	_free(e);
	puts("-----------------------");
	show_alloc_mem();

}
