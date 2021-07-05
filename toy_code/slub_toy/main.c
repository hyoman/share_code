#include<stdio.h>
#include<stdlib.h>
#include <execinfo.h>

struct test_track{
	int track_id;
	int tuple_data;
	char name[16];
};

typedef struct slub_ts{
	size_t object_size; // node size ->block_track
	size_t objects;  // object cnt
	unsigned char *entry;// node start pointer
	void **fl; /*  free list */
}ttslub_t;


ttslub_t *g_tt_slub;

ttslub_t * ttslub_init(void *page, size_t page_size, size_t object_size)
{
	char *s_nsn; /* next free table */
	size_t i;
	int  offset;
	void **tmp;
	ttslub_t *s_slub;
	
	s_slub = (ttslub_t *)page;
	s_slub->object_size = object_size;
	s_slub->objects = (page_size - sizeof(ttslub_t)) / object_size;

	while((s_slub->objects > ((size_t)0)) && ((page_size - sizeof(ttslub_t)) < ((s_slub->objects * object_size) ))) {
		s_slub->objects--;
	}

	if(s_slub->objects <= ((size_t)0)) {
		return((ttslub_t *)0);
	}
	s_slub->entry = ((unsigned char *)(&s_slub[1]));

	// s_nsn means next slub node pointer
	s_nsn = (char *)(&s_slub[1]);
	offset = s_slub->object_size;
	
	for(i = ((size_t)0); i < s_slub->objects; i++) {
	//	printf("&s_nsn[%d](%p), &s_nsn[%d*s_slub_object](%p)  \n ",i, &s_nsn[i*offset] ,i+1, &s_nsn[(i+1)*offset]);
		printf("i[%lu] <-%p\n", i , &s_nsn[(i+1)*offset]);
		tmp = (void **)&s_nsn[i*offset];
		*tmp = &s_nsn[(i+1)*offset];
	//	dumpHex((void*)&s_nsn[i*offset], sizeof(struct test_track));
	}
	s_slub->fl = (void**)&s_nsn[0];

	return(s_slub);
}


void tt_init(void)
{
	int size;
	int tt_start;
	void *heap_tt;
	
	//tt_start = 4096;
	tt_start = 256;
	size = tt_start;
	heap_tt=(void  *)malloc(tt_start);

	printf("heap_tt range[%p ~ %p]\n", heap_tt, heap_tt+size);
	if (!heap_tt) {
		printf("[%s] failed to block track buffer allocation!\n", __FUNCTION__);
		return;
	}
	
	g_tt_slub = ttslub_init(heap_tt, size, sizeof(struct test_track));
	if(g_tt_slub){
		printf(" success \n");
		printf("object size:%lu, obj_cnt:%lu \n", g_tt_slub->object_size, g_tt_slub->objects);
	}
	else{ 
		printf(" fail \n");
	}
}
static void *__ttslub_alloc(ttslub_t *s_slub)
{
	void *s_result;
	int old;
	
	s_result = s_slub->fl;
	s_slub->fl = *s_slub->fl;
	printf("alloc node:%p ,next free pointer :%p\n",s_result, s_slub->fl); 

	return(s_result);
}

static void *ttslub_alloc(void)
{
	void *m;
	m =	__ttslub_alloc(g_tt_slub);
	
	if(m)
		return (struct test_track *)m;
	return NULL;
}

static void * __ttslub_free(ttslub_t *s_slub, void *s_ptr)
{
	void **tmp;
		
	// below two lines of code is similar that slub init code
	tmp = s_ptr;
	*tmp = s_slub->fl;	
	s_slub->fl =s_ptr;

	printf("free pointer:%p next first pointer  :%p\n", s_ptr, s_slub->fl ); 
	return((void *)0);
}

void ttslub_free(void *ptr)
{
	__ttslub_free(g_tt_slub, ptr);
}


int main(int argc, char **argv)
{
	struct test_track *a;
	struct test_track *b;
	struct test_track *c;
	struct test_track *d;


	tt_init();
	printf("g_tt_slub->entry:%p\n",g_tt_slub->entry);
	printf("g_tt_slub->freelist:%p\n",g_tt_slub->fl);
	//alloc test
	a=ttslub_alloc();
	b=ttslub_alloc();
	c=ttslub_alloc();

	ttslub_free(b);

	d=ttslub_alloc();
	
	return 0;

}
