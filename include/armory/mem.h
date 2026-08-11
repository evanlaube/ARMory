
#ifndef MEM_H
#define MEM_H

#include <stddef.h>

/**
 * @brief Freestanding implementations of the C library memory functions.
 *
 * The project links with -nostdlib, so no C library provides these. At -O1 and
 * above the compiler may synthesize calls to them (e.g. from initialization or
 * copy loops), so they must exist to link. Signatures match the standard
 * <string.h> declarations so the two can coexist without conflict.
 */

/**
 * @brief Copies n bytes from src to dest. The regions must not overlap.
 *
 * @return dest.
 */
void *memcpy(void *dest, const void *src, size_t n);

/**
 * @brief Copies n bytes from src to dest, handling overlapping regions.
 *
 * @return dest.
 */
void *memmove(void *dest, const void *src, size_t n);

/**
 * @brief Fills the first n bytes of dest with the byte value c.
 *
 * @return dest.
 */
void *memset(void *dest, int c, size_t n);

/**
 * @brief Compares the first n bytes of a and b.
 *
 * @return <0, 0, or >0 if a is less than, equal to, or greater than b.
 */
int memcmp(const void *a, const void *b, size_t n);

#endif // !MEM_H
