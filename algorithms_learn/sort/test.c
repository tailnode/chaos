#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sort.h"

#define TEST_DATA_NUM	100
#define RESULT_OK		1
#define RESULT_NG		0

void gen_test_data(int* array, unsigned int num);
void print_data(int* array, unsigned int num);
int check_sort_result(int* array, unsigned int num);
sort_func find_sort_func(char* sort_name);

int main(int argc, char* argv[])
{
	if (2 != argc && 3 != argc)
	{
		printf("%d parameter error!\n", argc);
		return -1;
	}

	unsigned int num;
	if (3 == argc)
		num = atoi(argv[2]);
	else
		num = TEST_DATA_NUM;

	sort_func fn = find_sort_func(argv[1]);

	if (empty == fn)
		return -1;

	srand(time(NULL));

	int* array = (int*)malloc(sizeof(int)*num);

	gen_test_data(array, num);
	printf("============================\n");
	printf("before sort:\n");
	print_data(array, num);

	fn(array, num);

	printf("\nafter sort:\n");
	print_data(array, num);

	printf("============================\n");
	if (RESULT_OK == check_sort_result(array, num))
		printf("sort OK\n");
	else
		printf("sort NG\n");
	printf("============================\n");

	free(array);
	return 0;
}

void gen_test_data(int* array, unsigned int num)
{
	for (unsigned int i = 0; i < num; ++i)
		array[i] = rand() % (2*num);
}

void print_data(int* array, unsigned int num)
{
	for (unsigned int i = 0; i < num; ++i)
	{
		printf("%d\t", array[i]);
		if (0 == (i+1)%10)
			printf("\n");
	}
	printf("\n");
}

int check_sort_result(int* array, unsigned int num)
{
	if (num < 2)
		return RESULT_OK;

	for (unsigned int i = 0; i < num-2; ++i)
		if (array[i] > array[i+1])
			return RESULT_NG;

	return RESULT_OK;
}

sort_func find_sort_func(char* sort_name)
{
	static const struct {
		char* name;
		sort_func fn;
	} func_matrix[] = {
		{"insertion", insertion_sort},
		{"bubble", bubble_sort},
		{"merge", merge_sort},
		{"heap", heap_sort},
		{"quick", quick_sort},
		{"counting", counting_sort},
	};

	for (unsigned int i = 0; i < sizeof(func_matrix)/sizeof(func_matrix[0]); ++i)
	{
		if (!strcmp(sort_name, func_matrix[i].name))
		{
			printf("use %s sort\n", func_matrix[i].name);
			return func_matrix[i].fn;
		}
	}

	printf("can't find sort function, check your input\n");
	return empty;
}

void empty(int* array, unsigned int num)
{
}
