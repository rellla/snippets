#ifndef __HANDLES_H__
#define __HANDLES_H__

#define INVALID_HANDLE 0xffffffffU

enum {
	HT_DECODER	= 0,
	HT_DEVICE	= 1,
	HT_LENGTH,
};

typedef struct
{
	int device;
} device_ctx_t;

typedef struct
{
	int decoder;
} decoder_ctx_t;

void *handle_create(size_t size, int *handle);
void *handle_get(int handle);
int handle_put(int handle);
int handle_destroy(int handle);

#endif
