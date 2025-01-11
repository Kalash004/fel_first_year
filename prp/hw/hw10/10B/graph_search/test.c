#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

void write_buffer_to_file(const char *filename, const char *buffer, size_t buffer_size)
{
    int fd;
    void *map;

    // Open file for writing (truncate to ensure it's empty at the start)
    fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (lseek(fd, buffer_size - 1, SEEK_SET) == -1)
    {
        perror("lseek");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Write a dummy byte to expand the file size to at least `buffer_size`
    if (write(fd, "", 1) != 1)
    {
        perror("write");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Memory-map the file
    map = mmap(NULL, buffer_size, PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Copy buffer to the mapped memory
    memcpy(map, buffer, buffer_size);

    // Ensure changes are flushed to disk
    if (msync(map, buffer_size, MS_SYNC) == -1)
    {
        perror("msync");
    }

    // Unmap memory and close the file descriptor
    if (munmap(map, buffer_size) == -1)
    {
        perror("munmap");
    }

    close(fd);
}

int main()
{
    const char *filename = "output.txt";
    const char *data = "1 2 3\n1 2 3\n1 2 3\n1 2 3\n1 2 3\n";
    size_t data_len = strlen(data);

    write_buffer_to_file(filename, data, data_len);

    printf("Data written to file: %s\n", filename);
    return 0;
}