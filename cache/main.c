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

int main()
{
	int i1, i2, i3, i4, i5, i6, i7;

	int in1 = 10;
	int in2 = 20;
	int in3 = 30;
	int in4 = 40;
	int in5 = 50;
	int in6 = 60;
	int in7 = 70;

	CACHE *cache = cache_create();

	    cache_list(cache);
	i1 = slot_get(cache, &in1);
	    cache_list(cache);
	i2 = slot_get(cache, &in2);
	    cache_list(cache);
	i3 = slot_get(cache, &in3);
	    cache_list(cache);
	item_ref(i1, cache);
	    cache_list(cache);
	item_unref(i2, cache);
	    cache_list(cache);
	i4 = slot_get(cache, &in4);
	    cache_list(cache);
	item_unref(i1, cache);
	    cache_list(cache);
	item_unref(i1, cache);
	    cache_list(cache);
	i5 = slot_get(cache, &in5);
	    cache_list(cache);
	i6 = slot_get(cache, &in6);
	    cache_list(cache);
	i7 = slot_get(cache, &in7);
	    cache_list(cache);

	cache_free(cache);

	return 0;
}
