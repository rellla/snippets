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

int handle_create(void *data, int handletype);
void *handle_get(int handle, int handletype);
int handle_put(int handle, int handletype);
int handle_destroy(int handle, int handletype);

#endif
