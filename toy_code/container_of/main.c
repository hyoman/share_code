#include<stdio.h>
#include <stdlib.h>

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})


struct list_head{
	struct list_head *prev,*net;
};

struct sdata{
	unsigned int id;
	unsigned int age;
	char name[16];
	struct list_head list;
};

void container_1(void) {
	struct sdata tmp;
	printf("---------------------------------\n");
	printf("address of &tmp is= %p\n", &tmp);
	printf("address of &tmp.id is= %p\n", &tmp.id);
	printf("address of &tmp.age is= %p\n", &tmp.age);
	printf("address of &tmp.name is= %p\n", &tmp.name);
	printf("address of &tmp.list is= %p\n", &tmp.list);
	printf("---------------------------------\n");
	printf("container of id= %p \t offset of tmp->id= %lu\n", container_of(&tmp.id, struct sdata, id), offsetof(struct sdata, id));
	printf("container of age= %p \t offset of tmp->age= %lu\n", container_of(&tmp.age, struct sdata, age), offsetof(struct sdata, age));
	printf("container of name= %p \t offset of tmp->name= %lu\n", container_of(&tmp.name, struct sdata, name), offsetof(struct sdata, name));
	printf("container of list= %p \t offset of tmp->list= %lu\n", container_of(&tmp.list, struct sdata, list), offsetof(struct sdata, list));
}
void offset_1(void) {
	struct sdata tmp;

	printf("address of &tmp is= %p\n\n", &tmp);
	printf("address of tmp->id= %p \t offset of tmp->id= %lu\n", &tmp.id, offsetof(struct sdata, id));
	printf("address of tmp->age= %p \t offset of tmp->id= %lu\n", &tmp.age, offsetof(struct sdata, age));
	printf("address of tmp->name= %p \t offset of tmp->id= %lu\n", &tmp.age, offsetof(struct sdata, name));
	printf("address of tmp->list= %p \t offset of tmp->id= %lu\n", &tmp.list, offsetof(struct sdata, list));

	printf("computed address of &tmp using:\n");
	printf("\taddress and offset of tmp->id= %p\n",
			(struct sdata *) (((char *) &tmp.id) - ((unsigned long) &((struct sdata *)0)->id)));
	printf("\taddress and offset of tmp->age= %p\n",
			(struct sdata *) (((char *) &tmp.age) - ((unsigned long) &((struct sdata *)0)->age)));
	printf("\taddress and offset of tmp->name= %p\n",
			(struct sdata *) (((char *) &tmp.name) - ((unsigned long) &((struct sdata *)0)->name)));
	printf("\taddress and offset of tmp->list= %p\n",
			(struct sdata *) (((char *) &tmp.list) - ((unsigned long) &((struct sdata *)0)->list)));

}

void raw1(void) {
	struct sdata tmp;

	printf("---------------------------------\n");
	printf("address of &tmp is= %p\n", &tmp);
	printf("address of tmp->id= %p \t offset of tmp->id= %lu\n", &tmp.id, (unsigned long) &((struct sdata *)0)->id);
	printf("address of tmp->age= %p \t offset of tmp->age= %lu\n", &tmp.age, (unsigned long) &((struct sdata *)0)->age);
	printf("address of tmp->name= %p \t offset of tmp->name= %lu\n", &tmp.name, (unsigned long) &((struct sdata *)0)->name);
	printf("address of tmp->list= %p \t offset of tmp->name= %lu\n", &tmp.list, (unsigned long) &((struct sdata *)0)->list);
	printf("---------------------------------\n");

	printf("computed address of &tmp using:\n");
	printf("\taddress and contanier of tmp->id= %p\n",
			(struct sdata *) (((char *) &tmp.id) - ((unsigned long) &((struct sdata *)0)->id)));
	printf("\taddress and container of tmp->age= %p\n",
			(struct sdata *) (((char *) &tmp.age) - ((unsigned long) &((struct sdata *)0)->age)));
	printf("\taddress and container of tmp->name= %p\n",
			(struct sdata *) (((char *) &tmp.name) - ((unsigned long) &((struct sdata *)0)->name)));
	printf("\taddress and conainer of tmp->list= %p\n",
			(struct sdata *) (((char *) &tmp.list) - ((unsigned long) &((struct sdata *)0)->list)));

}

int main(int argc, char** argv)
{

//	raw1();
//	offset_1();
	container_1();

	return 0;
}


