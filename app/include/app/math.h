#ifndef RTC_PRIMITIVES_MATH_H_
#define RTC_PRIMITIVES_MATH_H_

#include <stdlib.h>

#define PI 3.14159265358979323846

const float EPSILON = 0.00001 * 3;

// Avoid using infinite so math works for groups.
const float GROUP_INFINITE_BIGNUM = 10e6;

inline bool epsilon_eq(float a, float b) { return abs(a - b) < EPSILON; };

#endif // RTC_PRIMITIVES_MATH_H_