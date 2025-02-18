#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int arr[10] = {
    1,2,3,4,5,6,70,8,10,9
};
int max = 0;
pthread_mutex_t mtx;

void *find_max(void *id) {
    int locked = pthread_mutex_trylock(&mtx);
    while (!locked)
    {
        usleep(100);
        locked = pthread_mutex_trylock(&mtx);
    }
    
    int checking = arr[*(size_t*)id];
    if (checking > max) {
        max = checking;
    }
    pthread_mutex_unlock(&mtx);
    return NULL;
}

int main() {
    printf("Start Max : %i\n", max);
    size_t ids[10] = {0,1,2,3,4,5,6,7,8,9};
    pthread_t pool[10];
    for (size_t i = 0; i < 10; ++i) {
        pthread_create(&pool[i], NULL, find_max, &ids[i]);
    }
    for (size_t i = 0; i < 10; ++i) {
        pthread_join(pool[i], NULL);
    }
    printf("New Max : %i\n", max);
}
