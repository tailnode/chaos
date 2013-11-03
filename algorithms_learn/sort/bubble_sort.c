#include "sort.h"

static inline void swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void bubble_sort(int* array, unsigned int num)
{
	for (unsigned int j = num; j > 0; --j)
		for (unsigned int i = 0; i < j-1; ++i)
			if (array[i] > array[i+1])
				swap(array+i, array+i+1);
}
