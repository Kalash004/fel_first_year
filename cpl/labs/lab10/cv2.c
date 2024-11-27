#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex;

size_t buffer_index = 0;

int buffer[3];

int random(int min, int max){
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

void *produce(void *buffer_index) {
    int temp = random(0, 100000);
    buffer[*(size_t*)buffer_index] = temp;
    sleep(1);
}

int main(void) {
    pthread_t pool[6];
    size_t buff_ids[3] = {0,1,2};
    for (size_t producer_id = 0; producer_id < 3; ++producer_id) {
        pthread_create(pool[producer_id], NULL, produce, (void *)&buff_ids[producer_id]);
    }
}