CFLAGS = -std=gnu99 -Wall

all:test

debug:CFLAGS += -g
debug:all

test:test.o bubble_sort.o heap_sort.o insertion_sort.o merge_sort.o \
		quick_sort.o counting_sort.o radix_sort.o
	gcc -o test $^ $(CFLAGS)

%.o:%.c sort.h
	gcc -c -o $@ $< $(CFLAGS)

clean:
	rm test *.o -f
