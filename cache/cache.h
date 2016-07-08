#ifndef __CACHE_H__
#define __CACHE_H__

typedef int ITEM;

typedef struct{
	unsigned int refcount;
	void *itemdata;
} item;

typedef struct
{
	item **data;
	int size;
} CACHE;

CACHE *cache_create(void);
void item_ref(ITEM item, CACHE *cache);
void item_unref(ITEM item, CACHE *cache);
int slot_get(CACHE *cache, void *item_p);

#endif
