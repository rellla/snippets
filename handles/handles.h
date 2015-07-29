#ifndef __HANDLES_H__
#define __HANDLES_H__

#define INVALID_HANDLE 0xffffffffU

typedef struct
{
	int device;
	void *somedata;
} device_ctx_t;

typedef struct
{
	int decoder;
	void *somedata;
} decoder_ctx_t;

typedef void (*callback)(void*);

void *handle_create(size_t size, int *handle, void(*callback)(void*));
void *handle_get(int handle);
int handle_put(int handle);
int handle_destroy(int handle);

#endif
