#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* myThreadFunction(void *arguments){
    size_t id = *(size_t*)arguments;
    printf("Thread running: %li.\n", id);
    return NULL;
}

int main() {
    size_t thread_count = 50;
    pthread_t pool[thread_count];
    size_t ids[thread_count];
    size_t i;
    for (i = 0; i < thread_count; ++i) {
        pthread_t thread;
        // printf("Started %li\n", i);
        ids[i] = i;
        pthread_create(&thread, NULL, myThreadFunction, (void *)(ids+i));
        pool[i] = thread;
    }
    printf("Main Thread.\n");
    for (i = 0; i< thread_count; ++i) {
        pthread_t t = pool[i];
        pthread_join(t, NULL); 
    }
    return 0;
}
