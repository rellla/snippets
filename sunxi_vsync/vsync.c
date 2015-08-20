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

uint64_t get_last_vsync(int disp_fd)
{
	uint32_t args[4] = { 0, 0, 0, 0};

	uint64_t vsync;
	args[1] = (uint64_t)(&vsync);

	if (ioctl(disp_fd, DISP_CMD_VSYNC_GET, args) < 0)
		return 1;

	return vsync;
}

uint32_t enable_vsync(int disp_fd)
{
	uint32_t args[4] = { 0, 1, 0, 0};
	ioctl(disp_fd, DISP_CMD_VSYNC_EVENT_EN, args);

	return 0;
}

int main ()
{
	int fb_fd, disp_fd;
	uint64_t vsync1, vsync2, sum;
	uint64_t kvsync1, kvsync2, ksum;
	static int i = 0;
	sum, ksum = 0;
	vsync2 = 0;

	disp_fd = open("/dev/disp", O_RDWR);
	if (disp_fd != -1)
		printf("VSync enabled\n");
	else
		printf("Failed to open /dev/disp! VSync disabled.\n");

	enable_vsync(disp_fd);

	vsync1 = get_time();
	kvsync1 = get_last_vsync(disp_fd);

	while (1)
	{
		while ((kvsync2 = get_last_vsync(disp_fd)) == kvsync1)
		{
			usleep(1000);
			printf("Waiting ....\n");
		}

		if (vsync2)
		{
			sum += ((vsync1 - vsync2) / 1000);
			printf("%i Time diff: %" PRIu64 ", Avg = %" PRIu64 "", i, ((vsync1 - vsync2) / 1000), (sum / i));
			if ((((vsync1 - vsync2) / 1000) > 21000) || (((vsync1 - vsync2) / 1000) < 19000))
				printf("<------ Not in time!\n");
			else
				printf("\n");
		}
		if (kvsync2)
		{
			sum += ((kvsync1 - kvsync2) / 1000);
			printf("%i KTime diff: %" PRIu64 ", Avg = %" PRIu64 "", i, ((kvsync1 - kvsync2) / 1000), (ksum / i));
			if ((((kvsync1 - kvsync2) / 1000) > 21000) || (((kvsync1 - kvsync2) / 1000) < 19000))
				printf("<------ Not in time!\n");
			else
				printf("\n");
		}

		i++;
		vsync2 = vsync1;
		kvsync2 = kvsync1;
	}
	return 0;
}