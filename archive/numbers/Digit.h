#pragma once

#include        "Types.h"

// the basic memory types to use for half-digits, whole-digits, and double-digits
typedef u16 uHalfDigit;
typedef s16 sHalfDigit;
typedef u32 uDigit;
typedef s32 sDigit;
typedef	u64 uTwoDigit;
typedef	s64 sTwoDigit;

//------------------------------------------------------------------------------
#define     DIGIT_BITS      (sizeof (uDigit) * 8)
#define	    DIGIT_SHIFT     4
#define	    DIGIT_MASK      ((1 << DIGIT_BITS) - 1)

//------------------------------------------------------------------------------
extern  uDigit  gRadixMax[37];

//------------------------------------------------------------------------------
sInt4	    StrToDigit (cstr string, uDigit radix, uDigit &digit, uDigit &multiplier);
sInt4		DigitToStr (uDigit digit, cstr string, uDigit radix);
sInt4		Cmp (sDigit size, const uDigit *a, const uDigit *b);
uDigit      Sub (sDigit size, const uDigit *a, const uDigit *b, uDigit *result);
uDigit      SubMult (sDigit size, const uDigit *a, const uDigit *b, uDigit c, uDigit *result);
sInt4		DigitBits (uDigit a);

//------------------------------------------------------------------------------

#endif  //  _DIGIT_
