/*
 * libsrc.c
 *
 *  Created on: 2015��4��22��
 *      Author: zhangqf
 */
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"


void * calloc(size_t n, size_t size)
{
	void *result;

	result = malloc(n*size);
	if(NULL != result)
	{
		memset(result,0,n*size);
	}

	return result;
}
