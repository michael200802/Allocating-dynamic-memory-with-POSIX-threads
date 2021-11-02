compiler = gcc

Mymalloc_test: Mymalloc.o
	$(compiler) -pthread $^ Mymalloc_test.c -o $@

Mymalloc.o: Mymalloc.c Mymalloc.h
	$(compiler) -pthread -c Mymalloc.c -o $@

clean: *.o
	rm *.o