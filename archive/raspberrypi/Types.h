#pragma once

// regular includes
#include    <iostream>
#include    <cstdlib>
#include    <cstdio>
#include    <queue>
#include    <set>

// keywords that VC used to leave out
/*
#define     and     &&
#define     or      ||
#define     not     !
#define     xor     ^
#define     and_eq  &=
#define     bit_and &
#define     bitand  &
#define     bit_or  |
#define     bitor   |
#define     compl   ~
#define     not_eq  !=
#define     or_eq   |=
#define     xor_eq  ^=
*/

// renamed functions
#define     StackNew    _alloca

// basic types I like to have, all type names should be capitalized
typedef bool            Bool;

typedef unsigned int    UInt;
typedef int             SInt;

typedef unsigned long   UInt8;
typedef long            SInt8;

typedef unsigned int    UInt4;
typedef int             SInt4;

typedef unsigned short  UInt2;
typedef short           SInt2;

typedef unsigned char   UInt1;
typedef char            SInt1;

typedef UInt1           Byte;
typedef char            Char;
typedef char*           String;
typedef const char*     CString;

typedef wchar_t         WideChar;

typedef double          Real;

typedef double          Real8;
typedef float           Real4;

// this is primarily for testing purposes
#ifdef      DEBUG
extern void testAssert(CString string, Bool condition, CString file, SInt line);
#define     assertCall(x) testAssert (#x, x, __FILE__, __LINE__)
#endif  //  DEBUG

#ifndef     assertCall
#define     assertCall
#endif  //  assertCall

