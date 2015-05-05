#include "flagh"
#include <stdio.h>

test_flag(int flags, int mask)
{
	if (!(flags & mask))
		printf("Flag %4x not set\n", mask);
	else
		printf("Flag %4x set\n", mask);
}

test_flag_inverse(int flags, int mask)
{
	if (flags & mask)
		printf("Flag %4x set\n", mask);
	else
		printf("Flag %4x not set\n", mask);
}

int main()
{
	int flags = 0;
	/* Activate flags */
	flags |= FLAG1;
	flags |= FLAG2;
	flags |= FLAG4;
	flags |= FLAG8;

	flags &= ~FLAG4;
	flags ^= FLAG8;

	test_flag(flags, FLAG1);
	test_flag(flags, FLAG2);
	test_flag(flags, FLAG4);
	test_flag(flags, FLAG8);

	flags &= ~FLAG4;
	flags ^= FLAG8;

	test_flag(flags, FLAG1);
	test_flag(flags, FLAG2);
	test_flag(flags, FLAG4);
	test_flag(flags, FLAG8);

/*
	test_flag_inverse(flags, FLAG1);
	test_flag_inverse(flags, FLAG2);
	test_flag_inverse(flags, FLAG4);
	test_flag_inverse(flags, FLAG8);
*/
	return 0;
}
