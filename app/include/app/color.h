#ifndef COLOR_H
#define COLOR_H

#include <array>
#include <concepts>
#include <cmath>
#include "math.h"
#include "types.h"

template <typename T>
requires std::floating_point<T>
class __attribute__ ((packed)) Color
{
    T r_, g_, b_;

public:
    Color(T r, T g, T b)
    {
        r_ = r;
        g_ = g;
        b_ = b;
    }
    Color(){};

    T r() const { return r_; }
    T g() const { return g_; }
    T b() const { return b_; }
};

template <typename T>
bool operator==(const Color<T> lhs, const Color<T> rhs)
{
    return epsilon_eq(lhs.r(), rhs.r()) &&
           epsilon_eq(lhs.g(), rhs.g()) &&
           epsilon_eq(lhs.b(), rhs.b());
}

template <typename T>
auto operator+(const Color<T> lhs, const Color<T> rhs)
{
    return Color(lhs.r() + rhs.r(), lhs.g() + rhs.g(), lhs.b() + rhs.b());
}

template <typename T>
auto operator-(const Color<T> lhs, const Color<T> rhs)
{
    return Color(lhs.r() - rhs.r(), lhs.g() - rhs.g(), lhs.b() - rhs.b());
}

template <typename T, typename U>
requires Number<U>
auto operator*(const Color<T> c, U s)
{
    return Color(c.r() * s, c.g() * s, c.b() * s);
}

template <typename T>
auto operator*(const Color<T> lhs, const Color<T> rhs)
{
    return Color(lhs.r() * rhs.r(), lhs.g() * rhs.g(), lhs.b() * rhs.b());
}
#endif // COLOR_H