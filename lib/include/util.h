/**
 * @file util.h
 * @brief Header of utility functions.
 * @author Guillaume Roumage
 * @date 04/2024
*/

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#ifndef UTIL_H
#define UTIL_H

void read_urandom_full(unsigned char *, size_t);
unsigned long int buffer_to_ulong(const unsigned char *, size_t);

#endif //UTIL_H