#include <stdio.h>

typedef struct ngx_array_s ngx_array_t;

struct ngx_array_s{
	void *elts;
	ngx_uint_t nelts;
	size_t size;
	ngx_uint_t nalloc;
	ngx_pool_t *pool;
};

