#include <stdlib.h>
#include <string.h>
#include "sort.h"

void counting_sort(unsigned int* array, unsigned int num)
{
	unsigned int max_num = array[0];

	for (unsigned int i = 1; i < num; ++i)
		if (max_num < array[i])
			max_num = array[i];

	unsigned int* array_out = (unsigned int*)malloc(sizeof(unsigned int)*num);
	unsigned int* array_count = (unsigned int*)malloc(sizeof(unsigned int)*(max_num + 1));

	memset(array_count, 0, max_num+1);

	for (unsigned int i = 0; i < num; ++i)
		++array_count[array[i]];

	for (unsigned int i = 1; i <= max_num; ++i)
		array_count[i] += array_count[i-1];

	for (unsigned int i = 0; i < num; ++i)
		array_out[--array_count[array[i]]] = array[i];

	memcpy(array, array_out, sizeof(unsigned int)*num);

	free(array_out);
	free(array_count);
}
