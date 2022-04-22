#ifndef CANVAS_H
#define CANVAS_H

#include <cassert>
#include <mutex>
#include <shared_mutex>
#include <fmt/core.h>
#include <fstream>

#include "color.h"

template <typename T>
requires std::floating_point<T>
class Canvas
{
public:
    Canvas(int w, int h)
    {
        assert(w >= 0);
        assert(h >= 0);
        w_ = w;
        h_ = h;
        T color_component = static_cast<T>(0);
        auto color = Color<T>(color_component, color_component, color_component);
        data_ = (T *)calloc(sizeof(color), w_ * h_);
        for (auto j = 0; j < h_; j++)
        {
            for (auto i = 0; i < w_; i++)
            {
                writePixel(color, i, j);
            }
        }
    }
    ~Canvas() = default;

    int width() { return w_; };
    int height() { return h_; };

    void writePixel(Color<T> c, int x, int y)
    {
        auto idx = PixelIndex(x, y);
        memcpy(&data_[idx], &c, sizeof(c));
    }

    Color<T> pixelAt(int w, int h)
    {
        auto idx = PixelIndex(w, h);
        auto res = Color<T>();
        memcpy(&res, &data_[idx], sizeof(res));
        return std::move(res);
    }

    T *data()
    {
        return data_;
    }

    void writeFile(std::string filename)
    {
        std::ofstream out(filename, std::ofstream::out);
        writeStream(out);
        out.close();
    }



    std::string mkPPMHeader()
    {
        std::string out;
        out.append("P3\n");
        out.append(fmt::format("{} {}\n", width(), height()));
        out.append("255\n");
        return out;
    }

    std::string mkPPMPayload()
    {
        auto w = width();
        auto h = height();
        std::string out;
        for (auto j = 0; j < h; j++)
        {
            for (auto i = 0; i < w; i++)
            {
                auto c = pixelAt(i, j);
                auto r = std::clamp((int)std::ceil(c.r() * 255), 0, 255);
                auto g = std::clamp((int)std::ceil(c.g() * 255), 0, 255);
                auto b = std::clamp((int)std::ceil(c.b() * 255), 0, 255);
                out.append(fmt::format("{} {} {}", r, g, b));
                if (i + 1 < w)
                    out.append(" ");
            }
            out.append("\n");
        }
        out.append("\n");
        return out;
    }

private:
    // Canvas data in GL_FLOAT or GL_DOUBLE format (r, g, b triplets).
    T *data_;
    mutable std::mutex MXdata_;

    int w_, h_;
    int PixelIndex(int x, int y)
    {
        assert(x >= 0);
        assert(x < w_);
        assert(y >= 0);
        assert(y < h_);

        return 3 * (x + y * w_);
    }

    void writeStream(std::ostream &stream)
    {
        stream << mkPPMHeader();
        stream << mkPPMPayload();
    }
};

#endif // CANVAS_H