#pragma once

// a few basic types, somewhat pedantic...
typedef unsigned char   byte;
typedef          int    sint;

typedef unsigned char   uchar;
typedef unsigned short  ushort;
typedef unsigned int    uint;
typedef unsigned long   ulong;

// if you want to care about the actual size of a storage type in use, these are guaranteed
typedef int8_t          s1;
typedef int16_t         s2;
typedef int32_t         s4;
typedef int64_t         s8;

typedef uint8_t         u1;
typedef uint16_t        u2;
typedef uint32_t        u4;
typedef uint64_t        u8;

typedef float           f4;
typedef double          f8;

// a few definitions to substitute for the standard names
#define bit_and         &
#define bit_or          |
#define complement      ~
