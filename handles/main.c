/*
 * Copyright (c) 2015 Andreas Baierl <ichgeh@imkreisrum.de>
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
#include "handles.h"

void device_cb(void* param)
{
	device_ctx_t *dev = (device_ctx_t *)param;
	printf("Callback function! Set device = 0 and free private data\n");
	dev->device = 0;
	free(dev->somedata);
	printf("Device: %d\n", dev->device);
}

void decoder_cb(void* param)
{
	decoder_ctx_t *dec = (decoder_ctx_t *)param;
	printf("Callback function! Set decoder = 0 and free private data\n");
	dec->decoder = 0;
	free(dec->somedata);
	printf("Decoder: %d\n", dec->decoder);
}

int device_create(int *device)
{
	device_ctx_t *dev = handle_create(sizeof(*dev), device, device_cb);

	if (!dev)
		goto err;

	void *somedata = calloc(1, sizeof(somedata));

	dev->device = 5;
	dev->somedata = somedata;

	return 0;
err:
	printf("Create Device failed");
	return 1;
}

int device_destroy(int device)
{
	decoder_ctx_t *dev = handle_get(device);
	if (!dev)
		return 1;

	handle_destroy(device);

	return 0;
}

int decoder_create(int *decoder)
{
	decoder_ctx_t *dec = handle_create(sizeof(decoder_ctx_t *), decoder, decoder_cb);

	if (!dec)
		goto err;

	void *somedata = calloc(1, sizeof(somedata));

	dec->decoder = 5;
	dec->somedata = somedata;

	return 0;
err:
	printf("Create Decoder failed");
	return 1;
}

int decoder_destroy(int decoder)
{
	decoder_ctx_t *dec = handle_get(decoder);
	if (!dec)
		return 1;

	handle_destroy(decoder);

	return 0;
}


int decoder_print(int decoder)
{
	decoder_ctx_t *dec = handle_get(decoder);
	if (!dec)
		return 1;

	printf("Decoder: %d\n", dec->decoder);

	handle_put(decoder);

	return 0;
}

int decoder_inc(int decoder)
{
	decoder_ctx_t *dec = handle_get(decoder);
	if (!dec)
		return 1;

	dec->decoder++;

	handle_put(decoder);

	return 0;
}

int decoder_dec(int decoder)
{
	decoder_ctx_t *dec = handle_get(decoder);
	if (!dec)
		return 1;

	dec->decoder--;

	handle_put(decoder);

	return 0;
}

int device_print(int device)
{
	device_ctx_t *dev = handle_get(device);
	if (!dev)
		return 1;

	printf("Device: %d\n", dev->device);

	handle_put(device);

	return 0;
}

int device_inc(int device)
{
	device_ctx_t *dev = handle_get(device);
	if (!dev)
		return 1;

	dev->device++;

	handle_put(device);

	return 0;
}

int device_inc2(int device)
{
	device_ctx_t *dev = handle_get(device);
	if (!dev)
		return 1;

	dev->device++;

//	handle_put(device);

	return 0;
}

int device_dec(int device)
{
	device_ctx_t *dev = handle_get(device);
	if (!dev)
		return 1;

	dev->device--;

	handle_put(device);

	return 0;
}


int main()
{
	int device, device2, decoder;

	device_create(&device);
//	decoder_create(&decoder);
//	device_create(&device2);

	device_print(device);
//	decoder_print(decoder);
//	device_print(device2);

	device_inc(device);
	device_print(device);
//	device_inc(device);
//	device_print(device);
//	device_inc(device);
//	device_print(device);
//	device_inc(device);
//	decoder_inc(decoder);
//	device_inc(device2);
//	device_inc2(device2);

//	device_print(device);
//	decoder_print(decoder);
//	device_print(device2);

	device_destroy(device);
//	decoder_destroy(decoder);
//	decoder_destroy(device2);

	return 0;
}
