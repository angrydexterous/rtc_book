#ifndef COLOR_H
#define COLOR_H

#include <array>
#include <concepts>
#include <cmath>
#include <Eigen/Dense>
#include "math.h"
#include "types.h"

namespace Color
{
  using Eigen::Matrix;

  template <typename T>
  requires std::floating_point<T>
  class Color
  {
    Matrix<T, 3, 1> rgb_;

  public:
    Color(T r, T g, T b)
    {
      rgb_ = Matrix<T, 3, 1>(r, g, b);
    }

    Color(Matrix<T, 3, 1> &&fromMatrix) : rgb_{std::move(fromMatrix)} {};
    Color(){};

    T r() const { return rgb_[0]; }
    T g() const { return rgb_[1]; }
    T b() const { return rgb_[2]; }

    bool operator==(const Color<T> rhs) const
    {
      return rgb_.isApprox(std::move(rhs.rgb_), EPSILON);
    }

    auto operator+(const Color<T> rhs) const
    {
      return Color(rgb_ + rhs.rgb_);
    }

    auto operator-(const Color<T> rhs) const
    {
      return Color(rgb_ - rhs.rgb_);
    }

    template <typename U>
    requires Number<U>
    auto operator*(U s) const
    {
      return Color(rgb_ * s);
    }

    auto operator*(const Color<T> rhs) const
    {
      Matrix<T, 3, 1> res = rgb_.array() * rhs.rgb_.array();
      return Color(std::move(res));
    }
  };
}
#endif // COLOR_H