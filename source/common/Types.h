#pragma once

// a few basic types, somewhat pedantic...
typedef unsigned char   byte;
typedef unsigned int    sint;
typedef unsigned int    uint;

// if you want to care about the actual size of a storage type in use, these are guaranteed
typedef signed char     s1;
typedef signed short    s2;
typedef signed int      s4;
typedef signed long     s8;

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
