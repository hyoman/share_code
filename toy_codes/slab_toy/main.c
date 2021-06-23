#include<stdio.h>
#include<stdlib.h>
#include <execinfo.h>


struct test_track{
	int track_id;
	int tuple_data;
	char name[16];
};


typedef unsigned int __ttslab_index_t;
#define ttslab_index_t __ttslab_index_t

typedef struct slab_ts{
	size_t object_size; // node size ->block_track
	size_t objects;  // object cnt
	unsigned char *entry;// node start pointer
	ttslab_index_t f; /* first free block index */
}ttslab_t;


ttslab_t *g_tt_slab;

ttslab_t * ttslab_init(void *page, size_t page_size, size_t object_size)
{
	ttslab_index_t *s_nft; /* next free table */
	size_t i;
	ttslab_t *s_slab;
	
	s_slab = (ttslab_t *)page;
	s_slab->object_size = object_size;
	s_slab->objects = (page_size - sizeof(ttslab_t)) / object_size;

	while((s_slab->objects > ((size_t)0)) && ((page_size - sizeof(ttslab_t)) < ((s_slab->objects * object_size) + (s_slab->objects * sizeof(ttslab_index_t))))) {
		s_slab->objects--;
	}

	if(s_slab->objects <= ((size_t)0)) {
		return((ttslab_t *)0);
	}
	// entry means node start pointer =  header size + index mapsize( obj cnt *   indexmap member size)
	s_slab->entry = ((unsigned char *)(&s_slab[1])) + (s_slab->objects * sizeof(ttslab_index_t));
	s_slab->f = (ttslab_index_t)0U;

	// s_nft means index map start pointer
	s_nft = (ttslab_index_t *)(&s_slab[1]);
	for(i = ((size_t)0); i < s_slab->objects; i++) {
		s_nft[i] = (ttslab_index_t)( i+ ((size_t)1u));
	}

	return(s_slab);
}


void tt_init(void)
{
	int size;
	int tt_start;
	tt_start = 4096;
	void *bss_tt;
	//size = slab_expect_page_size ( sizeof(struct test_track), tt_start );
	//bss_tt = (struct test_track *)alloc_bootmem(size);

	size = tt_start;
	bss_tt=(void  *)malloc(tt_start);
	printf("bss_tt range[%p ~ %p]\n", bss_tt, bss_tt+size);
	if (!bss_tt) {
		printf("[%s] failed to block track buffer allocation!\n", __FUNCTION__);
		return;
	}
	g_tt_slab = ttslab_init(bss_tt, size, sizeof(struct test_track));
	if(g_tt_slab){
		printf(" success \n");
		printf("object size:%lu, obj_cnt:%lu \n", g_tt_slab->object_size, g_tt_slab->objects);
	}
	else{ 
		printf(" fail \n");
	}
}

static void *__ttslab_alloc(ttslab_t *s_slab)
{
	void *result;
	int old;

	ttslab_index_t *s_nft = (ttslab_index_t *)(&s_slab[1]);


	if(s_slab->f >= s_slab->objects) { /* not enough object */
		result = (void *)0;
	}
	else { /* avail */
		old =	s_slab->f;
		result = (void *)(&s_slab->entry[s_slab->f * s_slab->object_size]);
		s_slab->f = s_nft[s_slab->f];
	}

	printf("alloc index:%d ,next first pointer :%d\n",old, s_slab->f); 
	return(result);
}

static void *ttslab_alloc(void)
{
	void *m;
	m =	__ttslab_alloc(g_tt_slab);
	
	if(m)
		return (struct test_track *)m;
	return NULL;
}

static void * __ttslab_free(ttslab_t *s_slab, void *ptr)
{
	ttslab_index_t *s_nft = (ttslab_index_t *)(&s_slab[1]);
	size_t i;

	i = (size_t)(((unsigned char *)ptr) - s_slab->entry) / s_slab->object_size;

	s_nft[i] = s_slab->f;
	s_slab->f = i;

	printf("free index:%d,present first pointer  :%d\n", s_slab->f, s_slab->f ); 
	return((void *)0);
}

void ttslab_free(void *ptr)
{
	__ttslab_free(g_tt_slab, ptr);
}



int main(int argc, char **argv)
{
	struct test_track *a;
	struct test_track *b;
	struct test_track *c;
	struct test_track *d;
	
	tt_init();
	//alloc test
	a=ttslab_alloc();
	b=ttslab_alloc();
	c=ttslab_alloc();

	ttslab_free(b);

	d=ttslab_alloc();
	
	return 0;

}
