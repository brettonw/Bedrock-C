#pragma once

// standard includes
#include    <cstring>
#include    <cmath>
#include    <cstdlib>
#include    <cstdio>
#include    <iostream>
#include    <iomanip>
#include    <sstream>
#include    <vector>
#include    <set>
#include    <map>
#include    <stdexcept>
#include    <algorithm>
#include    <chrono>
#include    <thread>

using namespace std;

// a few basic types
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

//------------------------------------------------------------------------------
// helpers
//------------------------------------------------------------------------------

template <typename T>
int signum(T value) {
    return (T(0) < value) - (value < T(0));
}

inline
double saturate (double value) {
    return (abs (value) > 0.5) ? double (signum (value)) : 0;
}
