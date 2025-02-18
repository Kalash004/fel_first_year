#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#define CHUNK_COUNT 5

static inline
uint32_t fill_value(int chunk, int idx)
{
  return (chunk * 77) ^ idx * 55;
}

static inline
size_t chunk_len(int chunk)
{
  return chunk * 137;
}

int main(void)
{

  uint32_t *chunks[CHUNK_COUNT];

  printf("Starting malloc-test\n");

  for (int chunk = 0; chunk < CHUNK_COUNT; chunk++) {
    size_t len = chunk_len(chunk);
    uint32_t *p = malloc(len * sizeof(uint32_t));
    chunks[chunk] = p;
    if (!p) {
      printf("FAILED alloc of chunk %d, len %d\n", chunk, (int)len);
      exit(1);
    } else {
      printf("Alloc of chunk %d, len %d address %p\n", chunk, (int)len, p);
    }
    while (len--) {
      *(p++) = fill_value(chunk, len);
    }
  }

  for (int chunk = 0; chunk < CHUNK_COUNT; chunk++) {
    size_t len = chunk_len(chunk);
    uint32_t *p = chunks[chunk];
    while (len--) {
      if (*(p++) != fill_value(chunk, len)) {
        printf("FAILED chunk %d, data compare\n", chunk);
        exit(1);
      }
    }
    printf("Chunk %d check OK\n", chunk);
  }

  printf("Succeed malloc-test\n");

  return 0;
}
