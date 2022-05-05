#include "app/matrix.h"
#include "app/tuple.h"

#include "gtest/gtest.h"

using Tuple::Point;
using Tuple::Vector;

class MatrixTest : public ::testing::Test
{
protected:
  virtual void SetUp(){};
  virtual void TearDown(){};
};

TEST_F(MatrixTest, matrix_test_new)
{
  auto m = Matrix::Matrix<float>({{1.f, 2.f, 3.f, 4.f},
                                  {5.5f, 6.5f, 7.5f, 8.5f},
                                  {13.5f, 14.5f, 15.5f, 16.5f}});
  ASSERT_EQ(m.rows(), 3);
  ASSERT_EQ(m.cols(), 4);
  ASSERT_EQ(m(0, 0), 1.f);
  ASSERT_EQ(m(1, 0), 5.5f);
}

TEST_F(MatrixTest, matrix_equality)
{
  auto a_same = Matrix::Matrix<float>({{1.f, 2.f, 3.f, 4.f},
                                       {5.5f, 6.5f, 7.5f, 8.5f},
                                       {13.5f, 14.5f, 15.5f, 16.5f}});
  auto b_same = Matrix::Matrix<float>({{1.f, 2.f, 3.f, 4.f},
                                       {5.5f, 6.5f, 7.5f, 8.5f},
                                       {13.5f, 14.5f, 15.5f, 16.5f}});
  auto c_not_same = Matrix::Matrix<float>({{1.f, 2.f, 3.f, 4.f},
                                           {5.5f, 6.5f, 7.5f, 8.5f},
                                           {13.5f, 14.5f, 15.5f, 17.5f}});
  ASSERT_EQ(a_same, b_same);
  ASSERT_NE(a_same, c_not_same);
}

TEST_F(MatrixTest, matrix_multiply)
{
  auto a = Matrix::Matrix<double>({{1, 2, 3, 4},
                                   {5, 6, 7, 8},
                                   {9, 8, 7, 6},
                                   {5, 4, 3, 2}});
  auto b = Matrix::Matrix<double>({{-2, 1, 2, 3},
                                   {3, 2, 1, -1},
                                   {4, 3, 6, 5},
                                   {1, 2, 7, 8}});
  auto res = Matrix::Matrix<double>({{20, 22, 50, 48},
                                     {44, 54, 114, 108},
                                     {40, 58, 110, 102},
                                     {16, 26, 46, 42}});
  ASSERT_EQ(a * b, res);
}

TEST_F(MatrixTest, matrix_mult_by_tuple)
{
  auto a = Matrix::Matrix<double>({{1, 2, 3, 4},
                                   {2, 4, 4, 2},
                                   {8, 6, 4, 1},
                                   {0, 0, 0, 1}});
  auto b = Tuple::Tuple(1., 2., 3., 1.);
  auto res = Tuple::Tuple(18., 24., 33., 1.);
  ASSERT_EQ(a * b, res);
}

TEST_F(MatrixTest, matrix_identity)
{
  auto a = Matrix::Matrix<double>({{0, 1, 2, 4},
                                   {1, 2, 4, 8},
                                   {2, 4, 8, 16},
                                   {4, 8, 16, 32}});
  auto i = a.identity();
  ASSERT_EQ(a * i, a);
}

TEST_F(MatrixTest, matrix_transpose)
{
  auto a = Matrix::Identity<double>(4);
  ASSERT_EQ(a.t() * a, a);

  auto b = Matrix::Matrix<double>({{0, 9, 3, 0},
                                   {9, 8, 0, 8},
                                   {1, 8, 5, 3},
                                   {0, 0, 5, 8}});
  auto bt = Matrix::Matrix<double>({{0, 9, 1, 0},
                                    {9, 8, 8, 0},
                                    {3, 0, 5, 5},
                                    {0, 8, 3, 8}});
  ASSERT_EQ(b.t(), bt);
  ASSERT_EQ(b, b.t().t());
}

TEST_F(MatrixTest, matrix_2_2_determinant)
{
  auto a = Matrix::Matrix<double>({{1, 5},
                                   {-3, 2}});
  ASSERT_EQ(a.det(), 17);
}

TEST_F(MatrixTest, matrix_submatrix)
{
  auto a = Matrix::Matrix<double>({{1, 5, 0},
                                   {-3, 2, 7},
                                   {0, 6, -3}});
  auto a_res = Matrix::Matrix<double>({{-3, 2},
                                       {0, 6}});

  ASSERT_EQ(a.submatrix(0, 2), a_res);

  auto b = Matrix::Matrix<double>({{-6, 1, 1, 6},
                                   {-8, 5, 8, 6},
                                   {-1, 0, 8, 2},
                                   {-7, 1, -1, 1}});
  auto b_res = Matrix::Matrix<double>({{-6, 1, 6},
                                       {-8, 8, 6},
                                       {-7, -1, 1}});
  ASSERT_EQ(b.submatrix(2, 1), b_res);
}

TEST_F(MatrixTest, matrix_minor)
{
  auto a = Matrix::Matrix<double>({{3, 5, 0},
                                   {2, -1, -7},
                                   {6, -1, 5}});
  ASSERT_EQ(a.submatrix(1, 0).det(), 25);
  ASSERT_EQ(a.minor(1, 0), 25);
}

TEST_F(MatrixTest, matrix_cofactor)
{
  auto a = Matrix::Matrix<double>({{3, 5, 0},
                                   {2, -1, -7},
                                   {6, -1, 5}});
  ASSERT_EQ(a.minor(0, 0), -12);
  ASSERT_EQ(a.cofactor(0, 0), -12);
  ASSERT_EQ(a.minor(1, 0), 25);
  ASSERT_EQ(a.cofactor(1, 0), -25);
}

TEST_F(MatrixTest, matrix_larger_determinant)
{
  auto a = Matrix::Matrix<double>({{1, 2, 6},
                                   {-5, 8, -4},
                                   {2, 6, 4}});
  ASSERT_EQ(a.cofactor(0, 0), 56);
  ASSERT_EQ(a.cofactor(0, 1), 12);
  ASSERT_EQ(a.cofactor(0, 2), -46);
  ASSERT_EQ(a.det(), -196);

  auto b = Matrix::Matrix<double>({{-2, -8, 3, 5},
                                   {-3, 1, 7, 3},
                                   {1, 2, -9, 6},
                                   {-6, 7, 7, -9}});
  ASSERT_EQ(b.cofactor(0, 0), 690);
  ASSERT_EQ(b.cofactor(0, 1), 447);
  ASSERT_EQ(b.cofactor(0, 2), 210);
  ASSERT_EQ(b.cofactor(0, 3), 51);
  ASSERT_EQ(b.det(), -4071);
}

TEST_F(MatrixTest, matrix_can_be_inverted)
{
  auto a = Matrix::Matrix<double>({{6, 4, 4, 4},
                                   {5, 5, 7, 6},
                                   {4, -9, 3, -7},
                                   {9, 1, 7, -6}});
  ASSERT_EQ(a.det(), -2120);
  ASSERT_EQ(a.invertible(), true);

  auto b = Matrix::Matrix<double>({{-4, 2, -2, -3},
                                   {9, 6, 2, 6},
                                   {0, -5, 1, -5},
                                   {0, 0, 0, 0}});
  ASSERT_EQ(b.det(), 0);
  ASSERT_EQ(b.invertible(), false);
}

TEST_F(MatrixTest, matrix_inversion)
{
  auto a = Matrix::Matrix<double>({{-5, 2, 6, -8},
                                   {1, -5, 1, 8},
                                   {7, 7, -6, -7},
                                   {1, -3, 7, 4}});
  ASSERT_EQ(a.det(), 532);
  ASSERT_EQ(a.cofactor(2, 3), -160);
  auto a_inv = a.inverse();
  ASSERT_EQ(a_inv(3, 2), -160. / 532);
  auto res = Matrix::Matrix<double>({{0.21805, 0.45113, 0.24060, -0.04511},
                                     {-0.80827, -1.45677, -0.44361, 0.52068},
                                     {-0.07895, -0.22368, -0.05263, 0.19737},
                                     {-0.52256, -0.81391, -0.30075, 0.30639}});
  ASSERT_EQ(a_inv, res);

  auto b = Matrix::Matrix<double>({{8, -5, 9, 2},
                                   {7, 5, 6, 1},
                                   {-6, 0, 9, 6},
                                   {-3, 0, -9, -4}});
  auto b_inv = Matrix::Matrix<double>({{-0.15385, -0.15385, -0.28205, -0.53846},
                                       {-0.07692, 0.12308, 0.02564, 0.03077},
                                       {0.35897, 0.35897, 0.43590, 0.92308},
                                       {-0.69231, -0.69231, -0.76923, -1.92308}});
  ASSERT_EQ(b.inverse(), b_inv);

  auto c = Matrix::Matrix<double>({{9, 3, 0, 9},
                                   {-5, -2, -6, -3},
                                   {-4, 9, 6, 4},
                                   {-7, 6, 6, 2}});
  auto c_inv = Matrix::Matrix<double>({{-0.04074, -0.07778, 0.14444, -0.22222},
                                       {-0.07778, 0.03333, 0.36667, -0.33333},
                                       {-0.02901, -0.14630, -0.10926, 0.12963},
                                       {0.17778, 0.06667, -0.26667, 0.33333}});
  ASSERT_EQ(c.inverse(), c_inv);
}

TEST_F(MatrixTest, matrix_inverse_product)
{
  auto a = Matrix::Matrix<double>({{3, -9, 7, 3},
                                   {3, -8, 2, -9},
                                   {-4, 4, 4, 1},
                                   {-6, 5, -1, 1}});
  auto b = Matrix::Matrix<double>({{8, 2, 2, 2},
                                   {3, -1, 7, 0},
                                   {7, 0, 5, 4},
                                   {6, -2, 0, 5}});
  auto c = a * b;
  ASSERT_EQ(c * b.inverse(), a);
}

TEST_F(MatrixTest, matrix_translation)
{
  auto transform = Matrix::Translation(5., -3., 2.);
  auto p = Point(-3., 4., 5.);
  auto v = Vector(-3., 4., 5.);

  ASSERT_EQ(transform * p, Point(2., 1., 7.));
  ASSERT_EQ(transform.inverse() * p, Point(-8., 7., 3.));
  ASSERT_EQ(transform * v, v);
}

TEST_F(MatrixTest, matrix_scaling)
{
  auto transform = Matrix::Scaling(2., 3., 4.);
  auto p = Point(-4., 6., 8.);
  auto v = Vector(-4., 6., 8.);

  ASSERT_EQ(transform * p, Point(-8., 18., 32.));
  ASSERT_EQ(transform * v, Vector(-8., 18., 32.));
  ASSERT_EQ(transform.inverse() * v, Vector(-2., 2., 2.));
}

TEST_F(MatrixTest, matrix_reflection)
{
  auto transform = Matrix::Scaling(-1., 1., 1.);
  auto p = Point(2., 3., 4.);

  ASSERT_EQ(transform * p, Point(-2., 3., 4.));
}

TEST_F(MatrixTest, matrix_rotate_x)
{
  auto p = Point(0., 1., 0.);
  auto half_quarter = Matrix::RotationX(PI / 4);
  auto full_quarter = Matrix::RotationX(PI / 2);

  ASSERT_EQ(half_quarter * p, Point(0., sqrt(2) / 2., sqrt(2) / 2.));
  ASSERT_EQ(full_quarter * p, Point(0., 0., 1.));
  ASSERT_EQ(half_quarter.inverse() * p, Point(0., sqrt(2) / 2., -1 * sqrt(2) / 2.));
}

TEST_F(MatrixTest, matrix_rotate_y)
{
  auto p = Point(0., 0., 1.);
  auto half_quarter = Matrix::RotationY(PI / 4);
  auto full_quarter = Matrix::RotationY(PI / 2);

  ASSERT_EQ(half_quarter * p, Point(sqrt(2) / 2., 0., sqrt(2) / 2.));
  ASSERT_EQ(full_quarter * p, Point(1., 0., 0.));
}

TEST_F(MatrixTest, matrix_rotate_z)
{
  auto p = Point(0., 1., 0.);
  auto half_quarter = Matrix::RotationZ(PI / 4);
  auto full_quarter = Matrix::RotationZ(PI / 2);

  ASSERT_EQ(half_quarter * p, Point(-1. * sqrt(2) / 2., sqrt(2) / 2., 0.));
  ASSERT_EQ(full_quarter * p, Point(-1., 0., 0.));
}

TEST_F(MatrixTest, matrix_shearing)
{
  auto transform_xy = Matrix::Shearing(1., 0., 0., 0., 0., 0.);
  auto transform_xz = Matrix::Shearing(0., 1., 0., 0., 0., 0.);
  auto transform_yx = Matrix::Shearing(0., 0., 1., 0., 0., 0.);
  auto transform_yz = Matrix::Shearing(0., 0., 0., 1., 0., 0.);
  auto transform_zx = Matrix::Shearing(0., 0., 0., 0., 1., 0.);
  auto transform_zy = Matrix::Shearing(0., 0., 0., 0., 0., 1.);

  auto p = Point(2., 3., 4.);

  ASSERT_EQ(transform_xy * p, Point(5., 3., 4.));
  ASSERT_EQ(transform_xz * p, Point(6., 3., 4.));
  ASSERT_EQ(transform_yx * p, Point(2., 5., 4.));
  ASSERT_EQ(transform_yz * p, Point(2., 7., 4.));
  ASSERT_EQ(transform_zx * p, Point(2., 3., 6.));
  ASSERT_EQ(transform_zy * p, Point(2., 3., 7.));
}

TEST_F(MatrixTest, matrix_combined_transforms)
{
  auto transform = Matrix::Identity<double>(4)
                       .rotate_x(PI / 2.)
                       .scale(5, 5, 5)
                       .translate(10, 5, 7);
  auto p = Point(1., 0., 1.);

  ASSERT_EQ(transform * p, Point(15., 0., 7.));
}