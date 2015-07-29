/*
 * Copyright (c) 2013 Jens Kuske <jenskuske@gmail.com>
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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "handles.h"

#define INITIAL_SIZE 1

typedef struct{
	unsigned int refcount;
	void *handledata;
	callback cb_func;
} dataobject;

static struct
{
	dataobject **data;
	int size;
} ht;

void *handle_create(size_t size, int *handle, void(*callback)(void*))
{
	*handle = INVALID_HANDLE;

	unsigned int index;

	dataobject *data = calloc(1, sizeof(dataobject));

	void *h_data = NULL;
	h_data = calloc(1, size);
	if (!h_data)
		return data;

	for (index = 0; index < ht.size; index++)
		if (ht.data[index] == NULL)
			break;

	if (index >= ht.size)
	{
		int new_size = ht.size ? ht.size * 2 : INITIAL_SIZE;
		dataobject **new_data = realloc(ht.data, new_size * (sizeof(dataobject)));
		if (!new_data)
			return data;

		memset(new_data + ht.size, 0, (new_size - ht.size) * (sizeof(dataobject)));
		ht.data = new_data;
		ht.size = new_size;
		printf("Table Resized to %d\n", ht.size);
	}

	data->refcount = 1;
	data->handledata = h_data;
	data->cb_func = callback;

	ht.data[index] = data;
	*handle = index + 1;

	printf("[CREATE] Handle: %d, Data %x, HTData %x, Ref %d\n", index, ht.data[index]->handledata, ht.data[index], ht.data[index]->refcount);

	return h_data;
}

void *handle_get(int handle)
{
	if (handle == INVALID_HANDLE)
		return NULL;

	unsigned int index = handle - 1;

	dataobject *data;
	void *h_data = NULL;

	if (index < ht.size) {
		data = ht.data[index];
		h_data = data->handledata;
		data->refcount++;
	}

	printf("[GET] Handle: %d, Data %x, HTData %x, Ref %d\n", index, data->handledata, data, data->refcount);
	return h_data;
}

int handle_put(int handle)
{
	if (handle == INVALID_HANDLE)
		return 1;

	unsigned int index = handle - 1;
	dataobject *data = NULL;

	if (index < ht.size) {
		data = ht.data[index];
		data->refcount--;

		printf("[PUT] Handle: %d, Data %x, HTData %x, Ref %d\n", index, data->handledata, data, data->refcount);
		if (data->refcount == 0) {
			data->cb_func(data->handledata);
			free(data->handledata);
			free(data);
		}

		return 0;
	}

	return 1;
}

int handle_destroy(int handle)
{
	if (handle == INVALID_HANDLE)
		return 1;

	unsigned int index = handle - 1;
	dataobject *data = NULL;

	if (index < ht.size) {
		data = ht.data[index];
		data->refcount--;
		printf("[DESTROY] Handle: %d, Data %x, HTData %x, Ref %d\n", index, data->handledata, data, data->refcount);
		return handle_put(handle);
	}

	return 1;
}
