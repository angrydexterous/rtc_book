#include <vector>
#include <range/v3/all.hpp>
#include <app/color.h>
#include "gtest/gtest.h"

class ColorTest : public ::testing::Test
{

protected:
  virtual void SetUp(){};

  virtual void TearDown(){};
};

TEST_F(ColorTest, color_is_correct_components)
{
  auto a = Color::Color(-0.5, 0.4, 1.7);
  EXPECT_FLOAT_EQ(a.r(), -0.5);
  EXPECT_FLOAT_EQ(a.g(), 0.4);
  EXPECT_FLOAT_EQ(a.b(), 1.7);
}

TEST_F(ColorTest, color_can_add)
{
  auto c1 = Color::Color(0.9, 0.6, 0.75);
  auto c2 = Color::Color(0.7, 0.1, 0.25);
  EXPECT_EQ(c1 + c2, Color::Color(1.6, 0.7, 1.0));
}

TEST_F(ColorTest, color_can_subtract)
{
  auto c1 = Color::Color(0.9, 0.6, 0.75);
  auto c2 = Color::Color(0.7, 0.1, 0.25);
  EXPECT_EQ(c1 - c2, Color::Color(0.2, 0.5, 0.5));
}

TEST_F(ColorTest, color_can_scalar_multiply)
{
  auto c1 = Color::Color(0.2, 0.3, 0.4);
  EXPECT_EQ(c1 * 2, Color::Color(0.4, 0.6, 0.8));
}

TEST_F(ColorTest, color_can_multiply)
{
  auto c1 = Color::Color(1.0, 0.2, 0.4);
  auto c2 = Color::Color(0.9, 1.0, 0.1);
  EXPECT_EQ(c1 * c2, Color::Color(0.9, 0.2, 0.04));
}

TEST_F(ColorTest, color_test_size_is_tightly_packed)
{
  auto cFloat = Color::Color(1.0f, 0.2f, 0.4f);
  ASSERT_EQ(sizeof(cFloat), sizeof(1.0f) * 3);
  auto cDouble = Color::Color(1.0, 0.2, 0.4);
  ASSERT_EQ(sizeof(cDouble), sizeof(1.0) * 3);
  auto cLongDouble = Color::Color(1.0l, 0.2l, 0.4l);
  ASSERT_EQ(sizeof(cLongDouble), sizeof(1.0l) * 3);
}