/** 
 * ngx_pool_t test, to test ngx_palloc, ngx_palloc_block, ngx_palloc_large 
 */  
  
#include <stdio.h>   
#include "ngx_config.h"   
#include "ngx_conf_file.h"   
#include "nginx.h"   
#include "ngx_core.h"   
#include "ngx_string.h"   
#include "ngx_palloc.h"   
  
volatile ngx_cycle_t  *ngx_cycle;  
  
void ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,  
            const char *fmt, ...)  
{  
}  
  
void dump_pool(ngx_pool_t* pool)  
{  
    while (pool)  
    {  
        printf("pool = 0x%x\n", pool);  
        printf("  .d\n");  
        printf("    .last = 0x%x\n", pool->d.last);  
        printf("    .end = 0x%x\n", pool->d.end);  
        printf("    .next = 0x%x\n", pool->d.next);  
        printf("    .failed = %d\n", pool->d.failed);  
        printf("  .max = %d\n", pool->max);  
        printf("  .current = 0x%x\n", pool->current);  
        printf("  .chain = 0x%x\n", pool->chain);  
        printf("  .large = 0x%x\n", pool->large);  
        printf("  .cleanup = 0x%x\n", pool->cleanup);  
        printf("  .log = 0x%x\n", pool->log);  
        printf("available pool memory = %d\n\n", pool->d.end - pool->d.last);  
        pool = pool->d.next;  
    }  
}  
  
int main()  
{  
    ngx_pool_t *pool;  
  
    printf("--------------------------------\n");  
    printf("create a new pool:\n");  
    printf("--------------------------------\n");  
    pool = ngx_create_pool(1024, NULL);  
    dump_pool(pool);  
  
    printf("--------------------------------\n");  
    printf("alloc block 1 from the pool:\n");  
    printf("--------------------------------\n");  
    void *p1 = ngx_palloc(pool, 512);  
    dump_pool(pool);  
  
    printf("--------------------------------\n");  
    printf("alloc block 2 from the pool:\n");  
    printf("--------------------------------\n");  
    void *p2 = ngx_palloc(pool, 512);  
    dump_pool(pool);  
  
    printf("--------------------------------\n");  
    printf("alloc block 3 from the pool :\n");  
    printf("--------------------------------\n");  
    void *p3 = ngx_palloc(pool, 512);  
    dump_pool(pool);  

    printf("--------------------------------\n");  
    printf("**************ngx_pfree*******:\n");  
    printf("--------------------------------\n");  

    if(ngx_pfree(pool, p1) < 0)
    {
	printf("ngx_pfree error 1\n");
    }

    if(ngx_pfree(pool, p2) < 0)
    {
	printf("ngx_pfree error 2\n");
    }

    if(ngx_pfree(pool, p3) < 0)
    {
	printf("ngx_pfree error 3\n");
    }
    printf("--------------------------------\n");  
    printf("alloc block 4 from the pool :\n");  
    printf("--------------------------------\n");  

    dump_pool(pool);  
/*********************ngx malloc more ******************/
    void *p4 = ngx_palloc(pool, 1024 * 1024);  
    dump_pool(pool);  
    if(ngx_pfree(pool, p4) < 0)
    {
	printf("ngx_pfree error 4\n");
    }
    else
    {
	printf("ngx_pfree succsess 4\n");
    }

    void *p5 = ngx_pnalloc(pool,1024);

    ngx_reset_pool(pool);
  
    ngx_destroy_pool(pool);  
    return 0;  
}  
