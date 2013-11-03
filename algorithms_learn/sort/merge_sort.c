#include <string.h>
#include <stdlib.h>
#include "sort.h"

void merge(int* array, unsigned int p, unsigned int q, unsigned int r)
{
	unsigned int l_size  = q - p + 1;
	unsigned int r_size  = r - q;

	unsigned int l_index = 0;
	unsigned int r_index = 0;
	unsigned int array_index = p;

	int* larray = malloc(sizeof(int) * l_size);
	int* rarray = malloc(sizeof(int) * r_size);

	memcpy(larray, array + p, sizeof(int) * l_size);
	memcpy(rarray, array + q + 1, sizeof(int) * r_size);

	while (l_index < l_size && r_index < r_size)
	{
		if (larray[l_index] < rarray[r_index])
			array[array_index] = larray[l_index++];
		else
			array[array_index] = rarray[r_index++];
		++array_index;
	}

	if (l_index < l_size)
		memcpy(array + array_index, larray + l_index, sizeof(int) * (l_size - l_index));
	else if (r_index < r_size)
		memcpy(array + array_index, rarray + r_index, sizeof(int) * (r_size - r_index));

	free(larray);
	free(rarray);
	larray = rarray = NULL;
}

static void _merge_sort(int* array, unsigned int p, unsigned int r)
{
	if (p < r)
	{
		unsigned int q = (p+r)/2;
		_merge_sort(array, p, q);
		_merge_sort(array, q+1, r);
		merge(array, p, q, r);
	}
}

void merge_sort(int* array, unsigned int num)
{
	_merge_sort(array, 0, num-1);
}
