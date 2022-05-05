#ifndef TUPLES_H
#define TUPLES_H

#include <array>
#include <concepts>
#include <cmath>
#include "math.h"
#include "types.h"
#include "app/matrix.h"

namespace Tuple
{
  auto Point(auto x, auto y, auto z);
  auto Vector(auto x, auto y, auto z);

  template <typename T>
  requires Number<T>
  class Tuple
  {
    T x_, y_, z_, w_;

  public:
    Tuple(T x, T y, T z, T w)
    {
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

    bool IsPoint() const
    {
      return epsilon_eq(w_, 1.0);
    }

    bool IsVector() const
    {
      return epsilon_eq(w_, 0.0);
    }

    T magnitude() const
    {
      return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_ + w_ + w_);
    }

    Tuple<T> normalize() const
    {
      auto mag = this->magnitude();
      return Tuple(x_ / mag, y_ / mag, z_ / mag, w_ / mag);
    }

    T dot(Tuple<T> rhs) const
    {
      return x_ * rhs.x_ +
             y_ * rhs.y_ +
             z_ * rhs.z_ +
             w_ * rhs.w_;
    }

    Tuple<T> cross(Tuple<T> rhs) const
    {
      return Vector(y_ * rhs.z_ - z_ * rhs.y_,
                    z_ * rhs.x_ - x_ * rhs.z_,
                    x_ * rhs.y_ - y_ * rhs.x_);
    }

    // Cast operation to a matrix from a tuple
    // Allows for matrix math on tuples
    operator Matrix::Matrix<T>() const
    {
      return Matrix::Matrix<T>({{x_}, {y_}, {z_}, {w_}});
    }

    bool operator==(const Tuple<T> rhs) const
    {
      return epsilon_eq(x_, rhs.x_) &&
             epsilon_eq(y_, rhs.y_) &&
             epsilon_eq(z_, rhs.z_) &&
             epsilon_eq(w_, rhs.w_);
    }

    auto operator+(const Tuple<T> rhs) const
    {
      return Tuple(x_ + rhs.x_, y_ + rhs.y_, z_ + rhs.z_, w_ + rhs.w_);
    }

    auto operator-(const Tuple<T> rhs) const
    {
      return Tuple(x_ - rhs.x_, y_ - rhs.y_, z_ - rhs.z_, w_ - rhs.w_);
    }

    auto operator-() const
    {
      return Tuple(-x_, -y_, -z_, -w_);
    }

    auto operator*(T scalar) const
    {
      return Tuple(x_ * scalar, y_ * scalar, z_ * scalar, w_ * scalar);
    }

    auto operator/(T div) const
    {
      return Tuple(x_ / div, y_ / div, z_ / div, w_ / div);
    }
  };

  template <typename T>
  requires Number<T>
  auto operator*(T scalar, const Tuple<T> t)
  {
    return t * scalar;
  }

  auto Point(auto x, auto y, auto z)
  {
    return Tuple(x, y, z, static_cast<decltype(x)>(1.0));
  }

  auto Vector(auto x, auto y, auto z)
  {
    return Tuple(x, y, z, static_cast<decltype(x)>(0.0));
  }
}
#endif // TUPLES_H