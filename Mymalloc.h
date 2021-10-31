#pragma once

#include <pthread.h>
#include <semaphore.h>
#include <stddef.h>

void free(void * ptr);

void * malloc(size_t size);

void * calloc(size_t nelem, size_t elsize);

void * realloc(void * ptr, size_t size);
