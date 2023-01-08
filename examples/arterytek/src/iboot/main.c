#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

ptu32_t djy_main(void)
{
	while(1) {
		printf("hello world!\r\n");
		DJY_EventDelay(100000);
	}
	return 0;
}
