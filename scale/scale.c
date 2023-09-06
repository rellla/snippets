#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int ARGBScale(uint8_t* src_argb, int src_stride_argb,
              int src_width, int src_height,
              uint8_t* dst_argb, int dst_stride_argb,
              int dst_width, int dst_height) {
  int dx, dy, x, y = 0;
  int j, k;
  int tx;
  uint32_t* src;
  uint32_t* dst;

  dx = ((int64_t)(src_width) << 16) / dst_width;
  dy = ((int64_t)(src_height) << 16) / dst_height;
  x = (dx < 0) ? (-(-dx >> 1)) : (dx >> 1);
  y = (dy < 0) ? (-(-dy >> 1))  : (dy >> 1);

  for (j = 0; j < dst_height; ++j) {
    tx = x; 
    src = (uint32_t*)(src_argb + (y >> 16) * src_stride_argb);
    dst = (uint32_t*)(dst_argb);
    for (k = 0; k < dst_width - 1; k += 2) {
      dst[0] = src[tx >> 16];
      tx += dx;
      dst[1] = src[tx >> 16];
      tx += dx;
      dst += 2;
    }
    if (dst_width & 1) {
      dst[0] = src[tx >> 16];
    }
    dst_argb += dst_stride_argb;
    y += dy;
  }

  return 0;
}

#define SRC_IMG_WIDTH 1280
#define SRC_IMG_HEIGHT 720
#define DST_IMG_WIDTH 1920
#define DST_IMG_HEIGHT 1080

int main (int argc, char **argv)
{
  FILE *input = fopen("in.rgba", "r+");
  uint8_t *image_in = (unsigned char *)malloc(SRC_IMG_WIDTH * SRC_IMG_HEIGHT * 4);
  fread(image_in, 1, SRC_IMG_WIDTH * SRC_IMG_HEIGHT * 4, input);

  uint8_t *image_out = (unsigned char *)malloc(DST_IMG_WIDTH * DST_IMG_HEIGHT * 4);

  ARGBScale(image_in, SRC_IMG_WIDTH * 4,
            SRC_IMG_WIDTH, SRC_IMG_HEIGHT,
            image_out, DST_IMG_WIDTH * 4,
            DST_IMG_WIDTH, DST_IMG_HEIGHT);

  FILE *output = fopen("out.rgba", "w+");
  fwrite(image_out, 1, DST_IMG_WIDTH * DST_IMG_HEIGHT * 4, output);

  fflush(output);
  fclose(input);
  fclose(output);

  return 0;
}