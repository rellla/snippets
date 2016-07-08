/*
 * Copyright (c) 2016 Andreas Baierl <ichgeh@imkreisrum.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "cache.h"

/*
 * This is our struct we want to push into the cache
 * including creation and destroying function
 */
typedef struct {
	int id;
	int *data;
} rgba_surface;


rgba_surface *rgba_create(int id, int data) {
	printf("Create surface id %d data %d\n", id, data);
	rgba_surface *rgba = calloc(1, sizeof(rgba_surface));
	int *r_data = calloc(1, sizeof(data));

	*r_data = data;

	rgba->id = id;
	rgba->data = r_data;
}

void *rgba_destroy(rgba_surface *rgba) {
	printf("Destroy surface id %d data %d\n", rgba->id, *rgba->data);
	free(rgba->data);
	free(rgba);
}

/* printf callback function */
void print_value(void *itemdata)
{
    printf("INT id %d INT data %d", ((rgba_surface *)itemdata)->id, *((rgba_surface *)itemdata)->data);
}

/* main procedure */
int main()
{
	/* slot ids */
	int i1, i2, i3, i4, i5, i6, i7;

	CACHE *cache = cache_create();

	/* create surfaces */
	rgba_surface *rgba1 = rgba_create(10, 111);
	rgba_surface *rgba2 = rgba_create(20, 222);
	rgba_surface *rgba3 = rgba_create(30, 333);
	rgba_surface *rgba4 = rgba_create(40, 444);
	rgba_surface *rgba5 = rgba_create(50, 555);
	rgba_surface *rgba6 = rgba_create(60, 666);
	rgba_surface *rgba7 = rgba_create(70, 777);

	/* get free slots and do sth with the surfaces */
	    cache_list(cache, print_value);
	i1 = slot_get(cache, rgba1);
	    cache_list(cache, print_value);
	i2 = slot_get(cache, rgba2);
	    cache_list(cache, print_value);
	i3 = slot_get(cache, rgba3);
	    cache_list(cache, print_value);
	item_ref(i1, cache);
	    cache_list(cache, print_value);
	item_unref(i2, cache);
	    cache_list(cache, print_value);
	i4 = slot_get(cache, rgba4);
	    cache_list(cache, print_value);
	item_unref(i1, cache);
	    cache_list(cache, print_value);
	item_unref(i1, cache);
	    cache_list(cache, print_value);
	i5 = slot_get(cache, rgba5);
	    cache_list(cache, print_value);
	i6 = slot_get(cache, rgba6);
	    cache_list(cache, print_value);
	i7 = slot_get(cache, rgba7);
	    cache_list(cache, print_value);

	/* destroy surfaces */
	rgba_destroy(rgba1);
	rgba_destroy(rgba2);
	rgba_destroy(rgba3);
	rgba_destroy(rgba4);
	rgba_destroy(rgba5);
	rgba_destroy(rgba6);
	rgba_destroy(rgba7);

	cache_free(cache);

	return 0;
}
