#pragma once

// a few basic types
typedef unsigned char   byte;
typedef unsigned short  ushort;
typedef unsigned int    uint;
typedef unsigned long   ulong;

// my echos of the posix types, with some checks to see what the memory/architecture model is
static_assert((sizeof(int) == 4) and (sizeof(long) == 8), "unsupported data model");

typedef char            s1;
typedef short           s2;
typedef int             s4;
typedef long            s8;

typedef unsigned char   u1;
typedef unsigned short  u2;
typedef unsigned int    u4;
typedef unsigned long   u8;

typedef float           f4;
typedef double          f8;

// a few definitions to substitute for the standard names
#define bit_and         &
#define bit_or          |
#define complement      ~
