
#include "armory/mem.h"
#include <stdint.h>

// NOTE: These functions MUST be compiled with -fno-tree-loop-distribute-patterns
// (see the mem.o rule in the makefile). Otherwise, at -O1 and above, the
// compiler's loop-idiom recognition turns the copy/fill loops below into calls
// to memcpy/memset -- i.e. calls to themselves -- and recurses forever. The
// pragma repeats the safeguard so the file is protected even if it is ever
// built outside the makefile.
#pragma GCC optimize("no-tree-loop-distribute-patterns")

void *memset(void *dest, int c, size_t n) {
    unsigned char *d = dest;
    unsigned char b = (unsigned char)c;

    // Fill leading bytes until dest is word-aligned
    while (n && ((uintptr_t)d & 3u)) {
        *d++ = b;
        n--;
    }

    // Fill a word at a time
    uint32_t w = (uint32_t)b;
    w |= w << 8;
    w |= w << 16;
    while (n >= 4) {
        *(uint32_t *)d = w;
        d += 4;
        n -= 4;
    }

    // Fill any trailing bytes
    while (n--) {
        *d++ = b;
    }

    return dest;
}

void *memcpy(void *dest, const void *src, size_t n) {
    unsigned char *d = dest;
    const unsigned char *s = src;

    // Fast path: copy a word at a time when src and dest share word alignment
    if ((((uintptr_t)d ^ (uintptr_t)s) & 3u) == 0) {
        while (n && ((uintptr_t)d & 3u)) {
            *d++ = *s++;
            n--;
        }
        while (n >= 4) {
            *(uint32_t *)d = *(const uint32_t *)s;
            d += 4;
            s += 4;
            n -= 4;
        }
    }

    // Copy the remainder (or everything, if the pointers are misaligned)
    while (n--) {
        *d++ = *s++;
    }

    return dest;
}

void *memmove(void *dest, const void *src, size_t n) {
    unsigned char *d = dest;
    const unsigned char *s = src;

    if (d == s || n == 0) {
        return dest;
    }

    if (d < s) {
        // No overlap in the forward direction: copy front to back
        while (n--) {
            *d++ = *s++;
        }
    } else {
        // dest is above src and the regions may overlap: copy back to front
        d += n;
        s += n;
        while (n--) {
            *--d = *--s;
        }
    }

    return dest;
}

int memcmp(const void *a, const void *b, size_t n) {
    const unsigned char *pa = a;
    const unsigned char *pb = b;

    while (n--) {
        if (*pa != *pb) {
            return (int)*pa - (int)*pb;
        }
        pa++;
        pb++;
    }

    return 0;
}
