#include "Mymalloc.h"
#include <stdio.h>

struct allocate_Mem_args
{
    size_t Memory_block_size;
    sem_t * ptr_to_semaphore_for_waiting_allocation;
    void * ptr_to_block;
};


void * allocate_Mem(void * arg)
{
    struct allocate_Mem_args * ptr_to_allocate_Mem_args = (struct allocate_Mem_args*)arg;

    char block[sizeof(sem_t) + ptr_to_allocate_Mem_args->Memory_block_size];
    ptr_to_allocate_Mem_args->ptr_to_block = (void*)(block+sizeof(sem_t));
    sem_t * semaphore_for_waiting_deallocation = (sem_t*)block;
    sem_init(semaphore_for_waiting_deallocation,0,0);

    //printf("Allocating %d bytes at %p\n",ptr_to_allocate_Mem_args->Memory_block_size,ptr_to_allocate_Mem_args->ptr_to_block);

    sem_post(ptr_to_allocate_Mem_args->ptr_to_semaphore_for_waiting_allocation);

    sem_wait(semaphore_for_waiting_deallocation);

    //printf("Memory at %p was deallocated\n",ptr_to_allocate_Mem_args->ptr_to_block);

    pthread_exit(NULL);
    return NULL;
}

void free(void * ptr)
{
    //printf("Deallocating the memory at %p\n",ptr); 
    sem_post((sem_t*)ptr-1); 
}

void * malloc(size_t size)
{
    sem_t semaphore_for_waiting_allocation;
    sem_init(&semaphore_for_waiting_allocation,0,0);

    struct allocate_Mem_args allocate_Mem_args;
    allocate_Mem_args.Memory_block_size = size;
    allocate_Mem_args.ptr_to_semaphore_for_waiting_allocation = &semaphore_for_waiting_allocation;

    pthread_t thread;
    pthread_create(&thread,NULL,allocate_Mem,(void*)&allocate_Mem_args);
    
    sem_wait(&semaphore_for_waiting_allocation);

    return allocate_Mem_args.ptr_to_block;
}

void * calloc(size_t nelem, size_t elsize)
{
    size_t nbytes = nelem*elsize;
    unsigned char * block = (unsigned char*)malloc(nbytes);
    while (nbytes)
    {
        nbytes--;
        block[nbytes] = 0;
    }
    return (void*)block;
}

void * realloc(void * ptr, size_t size)
{
    unsigned char * original_block = (unsigned char*)ptr;
    unsigned char * new_block = (unsigned char*)malloc(size);

    while (size)
    {
        size--;
        new_block[size] = original_block[size];
    }

    free(original_block);

    return new_block;
}
