#pragma once

template <typename NumberType>
int signum(NumberType value) {
    return (NumberType(0) < value) - (value < NumberType(0));
}

template <typename NumberType>
NumberType saturate (NumberType value) {
    return (abs (value) > 0.5) ? NumberType (signum (value)) : 0;
}

