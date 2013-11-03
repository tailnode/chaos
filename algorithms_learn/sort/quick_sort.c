#include <stdlib.h>
#include <time.h>
#include "sort.h"

static inline void swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

static void _quick_sort(int* array, unsigned int l, unsigned int r)
{
	if (l >= r)
		return;

	unsigned int i = l;
	unsigned int j = r;
	unsigned int base_index = rand()%(r-l+1) + l;
	swap(array+base_index, array+l);
	
	int base = array[l];


	while (i < j)
	{
		while (i < j && j > 0 && base <= array[j])
			--j;

		if (i < j)
			array[i++] = array[j];

		while (i < j && base >= array[i])
			++i;

		if (i < j)
			array[j--] = array[i];
	}

	array[i] = base;

	if (i > 0)
		_quick_sort(array, l, i-1);
	_quick_sort(array, i+1, r);
}

void quick_sort(int* array, unsigned int num)
{
	srand(time(NULL));
	_quick_sort(array, 0, num-1);
}
