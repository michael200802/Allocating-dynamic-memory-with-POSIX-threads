#include "Mymalloc.h"
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

void * thread(void * args)
{
    return NULL;
    int *x = (int*)malloc(sizeof(int));
    *x = 5;
    printf("%d",*x);
    //free(x);
    pthread_exit(NULL);
}

int main()
{
    puts("Hello");
    pthread_t th;
    pthread_create(&th,NULL,thread,NULL);
    usleep(1000000);
    return 0;
}