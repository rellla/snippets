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

#define INITIAL_SIZE 16

static struct
{
	int ref[HT_LENGTH];
	void **data;
	int size;
} ht;

int handle_create(void *data, int handletype)
{
	int index;

	if (!data)
		return -1;

	for (index = 0; index < ht.size; index++)
		if (ht.data[index] == NULL)
			break;

	if (index >= ht.size)
	{
		int new_size = ht.size ? ht.size * 2 : INITIAL_SIZE;
		void **new_data = realloc(ht.data, new_size * sizeof(void *));
		if (!new_data)
			return -1;

		memset(new_data + ht.size, 0, (new_size - ht.size) * sizeof(void *));
		ht.data = new_data;
		ht.size = new_size;
	}

	ht.ref[handletype] = 0;
	ht.data[index] = data;
	printf("[CREATE] Handle: %d, Address %x, ", index, ht.data[index]);

	switch (handletype) {
	case HT_DECODER:
		printf("Type: Decoder\n");
		break;
	case HT_DEVICE:
		printf("Type: Device\n");
		break;
	default:
		printf("Type: not specified\n");
		break;
	}

	return index + 1;
}

void *handle_get(int handle, int handletype)
{
	if (handle == INVALID_HANDLE)
		return NULL;

	int index = handle - 1;
	if (index < ht.size) {
		ht.ref[handletype]++;
		printf("[GET] Handle: %d, Address %x, Ref: %d\n", index, ht.data[index], ht.ref[handletype]);
		return ht.data[index];
	}

	return NULL;
}

int handle_put(int handle, int handletype)
{
	if (handle == INVALID_HANDLE)
		return 1;

	int index = handle - 1;
	if (index < ht.size) {
		ht.ref[handletype]--;
		printf("[PUT] Handle: %d, Address %x, Ref: %d\n", index, ht.data[index], ht.ref[handletype]);
	}

	return 0;
}

int handle_destroy(int handle, int handletype)
{
	int index = handle - 1;
	signed int timeout = 1000;

	if (index < ht.size) {
		ht.ref[handletype]--;

		while (ht.ref[handletype] > 0 && timeout > 0) {
			usleep(2);
			timeout--;
		}

		if (!(timeout > 0)) {
			printf("[DESTROY] Handle: %d, Time exeeded, %d references in use\n", index, ht.ref[handletype]);
			return 1;
		}

		printf("[DESTROY] Handle: %d, Address %x\n", index, ht.data[index]);
		ht.data[index] = NULL;
	}

	return 0;
}
