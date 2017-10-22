#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int raw;
	int grp, num;

	while (1) {
		printf("TEM? ");
		scanf("%d-%d", &grp, &num);

		raw = (grp & 0x1f) << 7;
		raw |= (num & 0x7f);

		printf("Raw Code: %02x%02x\n\n", (raw & 0xff), ((raw & 0xff00) >> 8));

	}

	return 0;
}
