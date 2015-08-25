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
//	memset(&vsync, 0, sizeof(uint64_t));
	args[1] = (unsigned long)(&vsync);

	if (ioctl(disp_fd, DISP_CMD_VSYNC_GET, args) < 0)
		return -1;

	return vsync;
}

uint32_t wait_for_vsync(int fb_fd)
{
	ioctl(fb_fd, FBIO_WAITFORVSYNC, 0);
	return 0;
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
	uint64_t kvsync1;
	uint64_t kvsync2;
	uint64_t ksum;
	static int i = 1;
	sum = 0;
	ksum = 0;
	vsync2 = 0;

	disp_fd = open("/dev/disp", O_RDWR);
	if (disp_fd != -1)
		printf("VSync enabled\n");
	else
		printf("Failed to open /dev/disp! VSync disabled.\n");

	fb_fd = open("/dev/fb0", O_RDWR);
	if (fb_fd != -1)
		printf("VSync enabled\n");
	else
		printf("Failed to open /dev/fb0! VSync disabled.\n");

	enable_vsync(disp_fd);

	vsync1 = get_time();
	kvsync1 = get_last_vsync(disp_fd);
	kvsync2 = get_last_vsync(disp_fd);

	struct timespec sleeping;
	sleeping.tv_sec = 0;
	sleeping.tv_nsec = 1;

	while (1)
	{
//		printf("%" PRIu64 "  %" PRIu64 "", kvsync1 / 1000000, kvsync2 / 1000000);
//		printf("    %" PRIu64 "\n", (kvsync2 / 1000000 - kvsync1 / 1000000));
//		vsync2 = get_time();
//		if (((kvsync2 / 1000) == (kvsync1 / 1000)))
//		{
//			usleep(1);
//			nanosleep(&sleeping, NULL);
//			printf(".");
//			kvsync2 = get_last_vsync(disp_fd);
//			continue;
//		}
//		else
//		{
/*			if (vsync2)
			{
				sum += ((vsync2 - vsync1) / 1000);
				printf("%i Time diff: %" PRIu64 ", Avg = %" PRIu64 "", i, ((vsync2 - vsync1) / 1000), (sum / i));
				if ((((vsync2 - vsync1) / 1000) > 20500) || (((vsync2 - vsync1) / 1000) < 19500))
					printf("<------ Not in time!\n");
				else
					printf("\n");
			}
*/
			wait_for_vsync(fb_fd);
			kvsync2 = get_last_vsync(disp_fd);
			usleep(1000);
//			printf("\n");
			if (kvsync2)
			{
				ksum += ((kvsync2 - kvsync1) / 1000);
//				printf("%" PRIu64 "  %" PRIu64 "", kvsync1 / 1000000, kvsync2 / 1000000);
//				printf("    %" PRIu64 "\n", (kvsync2 / 1000000 - kvsync1 / 1000000));
				printf("%i KTime diff: %" PRIu64 ", Avg = %" PRIu64 "", i, ((kvsync2 - kvsync1) / 1000), (ksum / i));
				if ((((kvsync2 - kvsync1) / 1000) > 20050) || (((kvsync2 - kvsync1) / 1000) < 19950))
					printf("<------ Not in time!\n");
				else
					printf("\n");
//				vsync1 = vsync2;
//				kvsync2 = get_last_vsync(disp_fd);
			}
			kvsync1 = kvsync2;
			i++;
//		}
	}
	return 0;
}
