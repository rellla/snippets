#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int raw, raw2;
	int grp, num;

	while (1) {
		printf("Raw Daten? ");
		scanf("%x", &raw);

		raw2 = (raw & 0xff00) >> 8;
		raw2 |= (raw & 0xff) << 8;

		grp = ((raw2 >> 7) & 0x1f);
		num = (raw2 & 0x7f);

		printf("TEM Code: %02d-%03d\n\n", grp, num);
	}

	return 0;
}
