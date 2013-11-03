#include "sort.h"

void insertion_sort(int* array, unsigned int num)
{
	for (unsigned int i = 1; i < num; i++)
	{
		int j = i - 1;
		int key = array[i];
		for (; array[j] > key && j >= 0; --j)
			array[j+1] = array[j];
		array[j+1] = key;
	}
}
