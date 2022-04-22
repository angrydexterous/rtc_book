#include <app/canvas.h>
#include <app/color.h>

#include <vector>
#include "spdlog/spdlog.h"
#include "gtest/gtest.h"

class CanvasTest : public ::testing::Test
{

protected:
    virtual void SetUp(){};

    virtual void TearDown(){};
};

TEST_F(CanvasTest, canvas_creation_size_and_starting_colors)
{
    auto runTest = []<typename T>(T black)
    {
        int w = 20;
        int h = 21;
        auto c = Canvas<T>(w, h);
        EXPECT_EQ(c.width(), w);
        EXPECT_EQ(c.height(), h);
        auto color = Color(black, black, black);
        for (auto j = 0; j < h; j++)
        {
            for (auto i = 0; i < w; i++)
            {
                EXPECT_EQ(c.pixelAt(i, j), color);
            }
        }
    };
    runTest(0.0f);
    runTest(0.0);
    runTest(0.0l);
}

TEST_F(CanvasTest, canvas_creation_set_color)
{
    int w = 10;
    int h = 20;
    auto c = Canvas<float>(w, h);
    auto color = Color(1.0f, 0.0f, 0.0f);
    c.writePixel(color, 2, 3);
    ASSERT_EQ(c.pixelAt(2, 3), color);
}

TEST_F(CanvasTest, canvas_construct_ppm_header)
{
    auto ans = "P3\n"
               "5 3\n"
               "255\n";
    constexpr int w = 5;
    constexpr int h = 3;
    auto canvas = Canvas<float>(w, h);
    auto dut = canvas.mkPPMHeader();
    ASSERT_EQ(dut, ans);
}

TEST_F(CanvasTest, canvas_construct_ppm_payload)
{
    auto ans = "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
                "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0\n"
                "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255\n\n";
    constexpr int w = 5;
    constexpr int h = 3;
    auto canvas = Canvas<float>(w, h);
    canvas.writePixel(Color(1.5f, 0.f, 0.f), 0, 0);
    canvas.writePixel(Color(0.f, 0.5f, 0.f), 2, 1);
    canvas.writePixel(Color(-0.5f, 0.f, 1.f), 4, 2);
    auto dut = canvas.mkPPMPayload();
    ASSERT_EQ(dut, ans);
}