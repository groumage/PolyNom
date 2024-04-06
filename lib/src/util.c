#include "../include/util.h"

/**
 * @brief Convert a buffer of bytes to an unsigned long int.
 * 
 * @param buffer The buffer to convert.
 * @param size The size of the buffer.
*/
unsigned long int buffer_to_ulong(const unsigned char *buffer, size_t size) {
    if (size > sizeof(unsigned long int))
    {
        fprintf(stderr, "Buffer size exceeds the size of unsigned long int.\n");
        exit(EXIT_FAILURE);
    }

    unsigned long int result = 0;

    for (size_t i = 0; i < size; ++i)
    {
        result <<= 8; // Shift the current result left by 8 bits (1 byte)
        result |= buffer[i]; // Set the least significant byte with the buffer content
    }

    return result;
}

/**
 * @brief Read from /dev/urandom and fill the buffer with random bytes.
 * 
 * @param buffer The buffer to fill with random bytes.
 * @param size The size of the buffer.
*/
void read_urandom_full(unsigned char *buffer, size_t size)
{
    int fd = open("/dev/urandom", O_RDONLY);
    size_t totalRead = 0;

    if (fd == -1)
    {
        perror("Failed to open /dev/urandom");
        exit(EXIT_FAILURE);
    }

    while (totalRead < size)
    {
        ssize_t bytesRead = read(fd, buffer + totalRead, size - totalRead);
        if (bytesRead <= 0)
        {
            perror("Failed to read from /dev/urandom");
            close(fd);
            exit(EXIT_FAILURE);
        }
        totalRead += bytesRead;
    }

    close(fd);
}