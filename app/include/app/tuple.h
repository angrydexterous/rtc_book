#ifndef TUPLES_H
#define TUPLES_H

#include <array>
#include <concepts>
#include <cmath>
#include "math.h"

template <typename T>
concept Number = (std::integral<T> || std::floating_point<T>) 
                 && !std::same_as<T, bool>
                 && !std::same_as<T, char>
                 && !std::same_as<T, unsigned char>                 
                 && !std::same_as<T, char8_t>
                 && !std::same_as<T, char16_t>
                 && !std::same_as<T, char32_t>
                 && !std::same_as<T, wchar_t>;

auto Point(auto x, auto y, auto z);
auto Vector(auto x, auto y, auto z);

template <typename T>
requires Number<T>                 
class Tuple {
  T x_, y_, z_, w_;
  public:
    Tuple(T x, T y, T z, T w){
        x_ = x;
        y_ = y;
        z_ = z;
        w_ = w;
    }
    Tuple();

    T x() const { return x_; }
    T y() const { return y_; }
    T z() const { return z_; }
    T w() const { return w_; }

    bool IsPoint() const{
      return epsilon_eq(w_, 1.0);
    }

    bool IsVector() const{
      return epsilon_eq(w_, 0.0);
    }

    T magnitude() const{
      return std::sqrt(this->x() * this->x() + this->y() * this->y() + this->z() * this->z() + this->w() + this->w());
    }

    Tuple<T> normalize() const{
      auto mag = this->magnitude();
      return Tuple(x_ / mag, y_ / mag, z_ / mag, w_ / mag);
    }

    T dot(Tuple<T> rhs) const{
      return this->x() * rhs.x() +
             this->y() * rhs.y() +
             this->z() * rhs.z() +
             this->w() * rhs.w();
    }

    Tuple<T> cross(Tuple<T> rhs) const{
      return Vector(this->y() * rhs.z() - this->z() * rhs.y(),
                    this->z() * rhs.x() - this->x() * rhs.z(),
                    this->x() * rhs.y() - this->y() * rhs.x());
    }
};

template <typename T>
bool operator==(const Tuple<T> lhs, const Tuple<T> rhs){
  return epsilon_eq(lhs.x(), rhs.x()) &&
    epsilon_eq(lhs.y(), rhs.y()) &&
    epsilon_eq(lhs.z(), rhs.z()) &&
    epsilon_eq(lhs.w(), rhs.w());
}

template <typename T>
auto operator+(const Tuple<T> lhs, const Tuple<T> rhs){
  return Tuple(lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z(), lhs.w() + rhs.w());
}

template <typename T>
auto operator-(const Tuple<T> lhs, const Tuple<T> rhs){
  return Tuple(lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z(), lhs.w() - rhs.w());
}

template <typename T>
auto operator-(const Tuple<T> x){
  return Tuple(-x.x(), -x.y(), -x.z(), -x.w());
}

template <typename T>
auto operator*(T scalar, const Tuple<T> t){
  return Tuple(t.x() * scalar, t.y() * scalar, t.z() * scalar, t.w() * scalar);
}

template <typename T>
auto operator*(const Tuple<T> t, T scalar){
  return Tuple(t.x() * scalar, t.y() * scalar, t.z() * scalar, t.w() * scalar);
}

template <typename T>
auto operator/(const Tuple<T> t, T div){
  return Tuple(t.x() / div, t.y() / div, t.z() / div, t.w() / div);
}

auto Point(auto x, auto y, auto z){
  return Tuple(x, y, z, static_cast<decltype(x)>(1.0));
}

auto Vector(auto x, auto y, auto z){
  return Tuple(x, y, z, static_cast<decltype(x)>(0.0));
}

#endif //TUPLES_H