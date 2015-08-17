#include <time.h>
#include <inttypes.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include "sunxi_disp_ioctl.h"

uint64_t get_time(void)
{
	struct timespec tp;

	if (clock_gettime(CLOCK_MONOTONIC, &tp) == -1)
		return 0;

	return (uint64_t)tp.tv_sec * 1000000000ULL + (uint64_t)tp.tv_nsec;
}

static int wait_for_vsync(int fb_fd)
{
	if (ioctl(fb_fd, FBIO_WAITFORVSYNC, 0))
		return 1;

	return 0;
}

int main ()
{
	int fb_fd;
	uint64_t vsync1, vsync2, sum;
	static int i = 0;
	sum = 0;
	vsync2 = 0;

	fb_fd = open("/dev/fb0", O_RDWR);
	if (fb_fd != -1)
		printf("VSync enabled\n");
	else
		printf("Failed to open /dev/fb0! VSync disabled.\n");

	while (1)
	{
		vsync1 = get_time();
		if (wait_for_vsync(fb_fd))
			printf("VSync failed\n");

		if (vsync2)
		{
			sum += ((vsync1 - vsync2) / 1000);
			printf("%i Time diff: %" PRIu64 ", Avg = %" PRIu64 "", i, ((vsync1 - vsync2) / 1000), (sum / i));
			if ((((vsync1 - vsync2) / 1000) > 21000) || (((vsync1 - vsync2) / 1000) < 19000))
				printf("<------ Not in time!\n");
			else
				printf("\n");
		}
		i++;

		vsync2 = vsync1;
	}
	return 0;
}