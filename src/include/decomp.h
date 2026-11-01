#ifndef DECOMP_H
#define DECOMP_H

#define DECOMP_PASTE2(a, b) a##b
#define DECOMP_PASTE(a, b) DECOMP_PASTE2(a, b)
#define DECOMP_SIZE_ASSERT(type, size) \
    typedef char DECOMP_PASTE(decomp_size_assert_, __LINE__)[(sizeof(type) == (size)) ? 1 : -1]

#endif
