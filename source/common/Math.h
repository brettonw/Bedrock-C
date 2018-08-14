#pragma once

/*
template <typename NumberType>
int signum(NumberType value) {
    return (NumberType(0) < value) - (value < NumberType(0));
}
*/

template <typename NumberType> inline constexpr
int signum (NumberType value, false_type is_signed) {
    return (NumberType(0) < value);
}

template <typename NumberType> inline constexpr
int signum (NumberType value, true_type is_signed) {
    return (NumberType (0) < value) - (value < NumberType (0));
}

template <typename NumberType> inline constexpr
int signum (NumberType value) {
    return signum(value, is_signed<NumberType>());
}

template <typename NumberType>
NumberType saturate (NumberType value) {
    return (abs (value) > 0.5) ? NumberType (signum (value)) : 0;
}

