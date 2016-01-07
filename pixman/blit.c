#include <stdio.h>
#include <stdlib.h>
#include "pixman-1/pixman.h"

typedef struct
{
	uint32_t width, height;
	uint32_t *data; /* pointer to the argb data */
} argb_surface_t;

typedef struct
{
	uint32_t x0, x1, y0, y1;
} rect_t;

void pixman_blit(pixman_image_t *dst, rect_t *dst_rect, pixman_image_t *src, rect_t *src_rect)
{
	pixman_transform_t transform;
	double fscale_x, fscale_y;

	/* Transform src_rct to dest_rct size */
	fscale_x = (dst_rect->x1 - dst_rect->x0) / (src_rect->x1 - src_rect->x0);
	fscale_y = (dst_rect->y1 - dst_rect->y0) / (src_rect->y1 - src_rect->y0);
	pixman_transform_init_identity(&transform);
	pixman_transform_scale(&transform, NULL, fscale_x, fscale_y);
	pixman_image_set_transform(src, &transform);

	/* Composite to the dest_img */
	pixman_image_composite32 (
		PIXMAN_OP_OVER, src, NULL, dst,
		(src_rect->x0 * fscale_x), (src_rect->y0 * fscale_y),
		0, 0,
		dst_rect->x0, dst_rect->y0,
		(dst_rect->x1 - dst_rect->x0), (dst_rect->y1 - dst_rect->y0));
}

int main (int argc, char **argv)
{
	#define SRC_WIDTH 10
	#define SRC_HEIGHT 10
	#define DST_WIDTH 8
	#define DST_HEIGHT 12
	#define WHITE 0x00000000
	#define BLACK 0xffffffff
/*
	FILE *fp1;
	fp1 = fopen("/tmp/blit.bmp", "w+");
	FILE *fp2;
	fp2 = fopen("/tmp/src.bmp", "w+");
	FILE *fp3;
	fp3 = fopen("/tmp/dst.bmp", "w+");
*/
	argb_surface_t *src = malloc(sizeof(argb_surface_t));
	argb_surface_t *dst = malloc(sizeof(argb_surface_t));

	src->width = SRC_WIDTH;
	src->height = SRC_HEIGHT;
	dst->width = DST_WIDTH;
	dst->height = DST_HEIGHT;

	rect_t src_rect = {4, 8, 4, 8};
	rect_t dst_rect = {2, 5, 2, 8};

	/* Zero size check */
	if(src->width == 0 || src->height == 0 || dst->width == 0 || dst->height == 0 ||
	   dst_rect.x1 - dst_rect.x0 == 0 || dst_rect.y1 - dst_rect.y0 == 0 ||
	   src_rect.x1 - src_rect.x0 == 0 || src_rect.y1 - src_rect.y0 == 0)
	{
		printf("Zero Size!");
		goto zero_out;
	}

	uint32_t *src_argb = malloc(SRC_WIDTH * SRC_HEIGHT * 4);
	uint32_t *dst_argb = malloc(DST_WIDTH * DST_HEIGHT * 4);

	src->data = src_argb;
	dst->data = dst_argb;

	int i;
	for (i = 0; i < SRC_WIDTH * SRC_HEIGHT; ++i)
		src_argb[i] = BLACK;
	for (i = 0; i < DST_WIDTH * DST_HEIGHT; ++i)
		dst_argb[i] = WHITE;

	pixman_image_t *src_img = pixman_image_create_bits (PIXMAN_a8r8g8b8,
					    src->width, src->height,
					    src->data, (src->width * 4));
	pixman_image_t *dst_img = pixman_image_create_bits (PIXMAN_a8r8g8b8,
					    dst->width, dst->height,
					    dst->data, (dst->width * 4));

// Showtime
/*
	fwrite(src_argb, 4, SRC_WIDTH * SRC_HEIGHT, fp2);
	fwrite(dst_argb, 4, DST_WIDTH * DST_HEIGHT, fp3);
*/
	pixman_blit(dst_img, &dst_rect, src_img, &src_rect);
/*
	fwrite(dst_argb, 4, DST_WIDTH * DST_HEIGHT, fp1);

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
*/
	printf("OK!");

	pixman_image_unref (dst_img);
	pixman_image_unref (src_img);

	return 0;

zero_out:
	free(src);
	free(dst);
	return 1;
}
