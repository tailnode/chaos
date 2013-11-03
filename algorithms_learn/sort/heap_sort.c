#include "sort.h"

static inline unsigned int left(unsigned int index)
{
	return ((index + 1)<<1) - 1;
}

static inline unsigned int right(unsigned int index)
{
	return (index + 1)<<1;
}

static inline void swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void max_heapify(int* array, unsigned int index, unsigned int heap_size)
{
	unsigned int l = left(index);
	unsigned int r = right(index);
	unsigned int largest = 0;

	if (l < heap_size && array[index] < array[l]) 
		largest = l;
	else
		largest = index;

	if (r < heap_size && array[largest] < array[r])
		largest = r;

	if (largest != index)
	{
		swap(array + index, array + largest);
		max_heapify(array, largest, heap_size);
	}
}

void build_max_heap(int* array, unsigned int num)
{
	for (unsigned int p = num/2; p > 0; --p)
		max_heapify(array, p-1, num);
}

void heap_sort(int* array, unsigned int num)
{
	build_max_heap(array, num);

	for (unsigned int i = num-1; i > 0; --i)
	{
		swap(array, array+i);
		max_heapify(array, 0, i);
	}
}
