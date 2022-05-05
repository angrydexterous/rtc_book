#ifndef MATRIX_H
#define MATRIX_H

#include <cassert>
#include <vector>
#include <cstdint>
#include <stdexcept>

#include "spdlog/spdlog.h"

#include "types.h"
#include "math.h"

namespace Matrix
{

  // FORWARD DECLARATIONS
  // Needed for builder function round trip on transforms
  template <class T>
  requires Number<T>
  class Matrix;
  auto Translation(auto x, decltype(x) y, decltype(x) z) -> Matrix<decltype(x)>;
  auto Scaling(auto x, decltype(x) y, decltype(x) z) -> Matrix<decltype(x)>;
  auto RotationX(auto rads) -> Matrix<decltype(rads)>;
  auto RotationY(auto rads) -> Matrix<decltype(rads)>;
  auto RotationZ(auto rads) -> Matrix<decltype(rads)>;
  auto Shearing(auto xy,
                decltype(xy) xz,
                decltype(xy) yx,
                decltype(xy) yz,
                decltype(xy) zx,
                decltype(xy) zy)
      -> Matrix<decltype(xy)>;

  // CODE
  template <class T>
  requires Number<T>
  class Matrix
  {
    std::vector<std::vector<T>> matrix_;
    uint_fast32_t rows_;
    uint_fast32_t cols_;

  public:
    Matrix(std::vector<std::vector<T>> m) : matrix_(m)
    {
      rows_ = matrix_.size();
      cols_ = matrix_[0].size();
    };

    Matrix(uint_fast32_t rows, uint_fast32_t cols) : rows_{rows}, cols_{cols}
    {
      assert(rows > 0);
      assert(cols > 0);
      matrix_ = std::vector<std::vector<T>>(
          rows_,
          std::vector<T>(cols));
    };

    uint_fast32_t rows() const
    {
      return rows_;
    }

    uint_fast32_t cols() const
    {
      return cols_;
    }

    const T operator()(uint_fast32_t row, uint_fast32_t col) const
    {
      assert(row < rows_);
      assert(col < cols_);
      return matrix_[row][col];
    }

    T &operator()(uint_fast32_t row, uint_fast32_t col)
    {
      assert(row < rows_);
      assert(col < cols_);
      return matrix_.at(row).at(col);
    }

    bool operator==(const Matrix<T> &rhs) const
    {
      bool res = true;
      for (auto y = 0; y < cols_; y++)
      {
        for (auto x = 0; x < rows_; x++)
        {
          if (!epsilon_eq(matrix_[x][y], rhs.matrix_[x][y]))
          {
            res = false;
            break;
          }
        }
        if (!res)
          break;
      }
      return res;
    }

    Matrix<T> operator*(const Matrix<T> &rhs) const
    {
      assert(cols_ == rhs.rows_);
      auto res = Matrix<T>(rows_, rhs.cols_);
      for (auto y = 0; y < rhs.cols_; y++)
      {
        for (auto x = 0; x < rows_; x++)
        {
          T result = (T)0;
          for (auto i = 0; i < rows_; i++)
          {
            result += matrix_[x][i] * rhs.matrix_[i][y];
          }
          res(x, y) = result;
        }
      }
      return res;
    }

    Matrix<T> identity() const
    {
      auto res = Matrix<T>(rows_, rows_);
      for (auto i = 0; i < rows_; i++)
      {
        res(i, i) = (T)1;
      }
      return res;
    }

    // Transpose
    Matrix<T> t() const
    {
      auto res = Matrix<T>(cols_, rows_);
      for (auto y = 0; y < cols_; y++)
      {
        for (auto x = 0; x < rows_; x++)
        {
          res(y, x) = matrix_[x][y];
        }
      }
      return res;
    }

    T det() const
    {
      T result = static_cast<T>(0);
      assert(cols_ == rows_);
      if (rows_ == 2)
      {
        result = matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
      }
      else
      {
        for (auto i = 0; i < cols_; i++)
        {
          result += matrix_[0][i] * this->cofactor(0, i);
        }
      }
      return result;
    }

    T minor(uint_fast32_t row, uint_fast32_t col) const
    {
      return this->submatrix(row, col).det();
    }

    T cofactor(uint_fast32_t row, uint_fast32_t col) const
    {
      auto change_sign = (row + col) % 2 == 0 ? 1 : -1;
      return static_cast<T>(change_sign) * this->minor(row, col);
    }

    Matrix<T> submatrix(uint_fast32_t row, uint_fast32_t col) const
    {
      auto res = Matrix<T>(rows_ - 1, cols_ - 1);
      auto write_row = 0;
      for (int i = 0; i < matrix_.size(); i++)
      {
        if (i == row)
          continue;
        res.matrix_[write_row] = matrix_[i];
        res.matrix_[write_row].erase(res.matrix_[write_row].begin() + col);
        write_row++;
      }
      return res;
    }

    bool invertible() const
    {
      if (det() == static_cast<T>(0))
      {
        return false;
      }
      return true;
    }

    Matrix<T> inverse() const
    {
      if (!invertible())
        throw std::runtime_error("Not invertible");
      auto res = Matrix<T>(rows_, cols_);
      auto determinant = det();
      for (auto y = 0; y < cols_; y++)
      {
        for (auto x = 0; x < rows_; x++)
        {
          res(y, x) = cofactor(x, y) / determinant;
        }
      }
      return res;
    }

    Matrix<T> translate(T x, T y, T z) const
    {
      return Translation(x, y, z) * *this;
    }

    Matrix<T> scale(T x, T y, T z) const
    {
      return Scaling(x, y, z) * *this;
    }

    Matrix<T> rotate_x(T rads) const
    {
      return RotationX(rads) * *this;
    }

    Matrix<T> rotate_y(T rads) const
    {
      return RotationY(rads) * *this;
    }

    Matrix<T> rotate_z(T rads) const
    {
      return RotationZ(rads) * *this;
    }

    Matrix<T> shear(T xy, T xz, T yx, T yz, T zx, T zy) const
    {
      return Shearing(xy, xz, yx, yz, zx, zy) * *this;
    }

  private:
  };

  template <class T>
  requires Number<T>
      Matrix<T> Identity(uint_fast32_t size)
  {
    auto res = Matrix<T>(size, size);
    for (auto i = 0; i < size; i++)
    {
      res(i, i) = (T)1;
    }
    return res;
  }

  // Transforms
  auto Translation(auto x, decltype(x) y, decltype(x) z) -> Matrix<decltype(x)>
  {
    auto t = Identity<decltype(x)>(4);
    t(0, 3) = x;
    t(1, 3) = y;
    t(2, 3) = z;
    return t;
  }

  auto Scaling(auto x, decltype(x) y, decltype(x) z) -> Matrix<decltype(x)>
  {
    auto t = Identity<decltype(x)>(4);
    t(0, 0) = x;
    t(1, 1) = y;
    t(2, 2) = z;
    return t;
  }

  auto RotationX(auto rads) -> Matrix<decltype(rads)>
  {
    auto t = Identity<decltype(rads)>(4);
    t(1, 1) = cos(rads);
    t(1, 2) = -1 * sin(rads);
    t(2, 1) = sin(rads);
    t(2, 2) = cos(rads);
    return t;
  }

  auto RotationY(auto rads) -> Matrix<decltype(rads)>
  {
    auto t = Identity<decltype(rads)>(4);
    t(0, 0) = cos(rads);
    t(0, 2) = sin(rads);
    t(2, 0) = -1 * sin(rads);
    t(2, 2) = cos(rads);
    return t;
  }

  auto RotationZ(auto rads) -> Matrix<decltype(rads)>
  {
    auto t = Identity<decltype(rads)>(4);
    t(0, 0) = cos(rads);
    t(0, 1) = -1 * sin(rads);
    t(1, 0) = sin(rads);
    t(1, 1) = cos(rads);
    return t;
  }

  auto Shearing(auto xy,
                decltype(xy) xz,
                decltype(xy) yx,
                decltype(xy) yz,
                decltype(xy) zx,
                decltype(xy) zy)
      -> Matrix<decltype(xy)>
  {
    auto t = Identity<decltype(xy)>(4);
    t(0, 1) = xy;
    t(0, 2) = xz;
    t(1, 0) = yx;
    t(1, 2) = yz;
    t(2, 0) = zx;
    t(2, 1) = zy;
    return t;
  }
} // End Matrix

#endif // MATRIX_H