#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* myThreadFunction(void *arguments){
    size_t *id = (size_t*)arguments;
    *id += 2;
    return NULL;
}

int main() {
    size_t thread_count = 10000;
    pthread_t pool[thread_count];
    size_t id = 0;
    size_t i;
    for (i = 0; i < thread_count; ++i) {
        pthread_t thread;
        pthread_create(&thread, NULL, myThreadFunction, (void *)&id);
        pool[i] = thread;
    }
    printf("Main Thread.\n");
    for (i = 0; i< thread_count; ++i) {
        pthread_t t = pool[i];
        pthread_join(t, NULL); 
    }
    printf("id: %li",id);
    return 0;
}
