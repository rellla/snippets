#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct sunxi_sram_func {
	char		*func;
	int		val;
	int		bits;
};

struct sunxi_sram_data {
	char			*name;
	struct sunxi_sram_func	*func;
};

struct sunxi_sram_desc {
	struct sunxi_sram_data	data;
};

#define SUNXI_SRAM_MAP(_val, _func, _bits)	\
	{					\
		.func = _func,			\
		.val = _val,			\
		.bits = _bits,			\
	}

#define SUNXI_SRAM_DATA(_name, ...)			\
	{						\
		.name = _name,				\
		.func = (struct sunxi_sram_func[]){	\
			__VA_ARGS__, { } },		\
	}

static struct sunxi_sram_desc sun4i_a10_sram_c1 = {
	.data = SUNXI_SRAM_DATA("C1",
				SUNXI_SRAM_MAP(0, "cpu", 0),
				SUNXI_SRAM_MAP(1, "ve", 0x7fffffff)),
};

int main()
{
	int i;

	i = 1;

	struct sunxi_sram_func *func;
	struct sunxi_sram_desc *sp = &sun4i_a10_sram_c1;

	struct sunxi_sram_data sram_data = sp->data;

	func = &sram_data.func[i];
	if (!func->func) {
		printf("Error");
		return 1;
	}
	printf("\t\t%s %c 0x%x\n", func->func, func->val == 1 ? '*' : ' ', func->bits);

/*
	for (func = sram_data.func; func->func; func++) {
		printf("\t\t%s %c\n", func->func, func->val == 1 ? '*' : ' ');
	}
*/
	return 0;
}
