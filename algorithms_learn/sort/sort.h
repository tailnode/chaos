#ifndef __SORT_H__
#define __SORT_H__

typedef void (*sort_func) (int*, unsigned int);

void insertion_sort(int* array, unsigned int num);
void bubble_sort(int* array, unsigned int num);
void merge_sort(int* array, unsigned int num);
void heap_sort(int* array, unsigned int num);
void quick_sort(int* array, unsigned int num);
void counting_sort(unsigned int* array, unsigned int num);
void empty(int* array, unsigned int num);

#endif // __SORT_H__

