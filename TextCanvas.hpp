// TextCanvas.hpp
///////////////////////////////////////////////////////////////////////////

#ifndef TEXT_CANVAS_HPP_
#define TEXT_CANVAS_HPP_    1   // Version 1

#if _MSC_VER > 1000
    #pragma once
#endif

#include <string>       // for std::string
#include <vector>       // for std::vector
#include <algorithm>    // for std::swap
#include <cmath>        // for std::sqrt
#include <cassert>      // for assert macro
#include <iostream>     // for std::cout, std::endl, ...

///////////////////////////////////////////////////////////////////////////

namespace textcanvas
{
    typedef std::string string_type;
    typedef string_type::value_type char_type;
    typedef std::size_t size_t;
    typedef std::ptrdiff_t ptrdiff_t;
    typedef ptrdiff_t coord_t;

    static const char_type zero = char_type(0);
    static const char_type one = char_type(1);
    static const char_type space = char_type(' ');
    static const char_type newline = char_type('\n');
#ifdef M_PI
    static const double pi = M_PI;
#else
    static const double pi = 3.1415926535;
#endif

    namespace
    {
        inline coord_t round(double d)
        {
            return coord_t(d + 0.5);
        }
        inline double normalize_radian(double radian)
        {
            radian = fmod(radian, 2 * M_PI);
            if (radian <= -pi)
                radian += 2 * pi;
            else if (radian >= M_PI)
                radian -= 2 * pi;
            return radian;
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // Point and Points

    struct Point
    {
        coord_t x, y;
        Point()
        {
        }
        Point(coord_t x_, coord_t y_) : x(x_), y(y_)
        {
        }
    };
    typedef std::vector<Point> Points;

    inline bool operator==(Point p0, Point p1)
    {
        return p0.x == p1.x && p0.y == p1.y;
    }
    inline bool operator!=(Point p0, Point p1)
    {
        return p0.x != p1.x || p0.y != p1.y;
    }

    ///////////////////////////////////////////////////////////////////////////
    // TextCanvas

    class TextCanvas
    {
    public:
        TextCanvas();
        TextCanvas(coord_t width, coord_t height, char_type ch = space);
        TextCanvas(const TextCanvas& tc);
        TextCanvas& operator=(const TextCanvas& tc);
        virtual ~TextCanvas();

    protected:
        coord_t     m_width;
        coord_t     m_height;
        string_type m_text;
        char_type   m_ch;

    public:
        coord_t width() const;
        coord_t height() const;
        coord_t count() const;
              char_type& operator[](size_t index);
        const char_type& operator[](size_t index) const;

        string_type to_str() const;
        bool in_range(coord_t x, coord_t y) const;
        bool in_range(Point p) const;
              string_type& data();
        const string_type& data() const;
        bool same_size(const TextCanvas& tc) const;

        void clear(char_type ch = space);
        void reset(coord_t width, coord_t height, char_type ch = space);
        void resize(coord_t width, coord_t height, char_type ch = space);

        char_type get_pixel(coord_t x, coord_t y) const;
        char_type get_pixel(Point p) const;
        void set_pixel(coord_t x, coord_t y, char_type ch);
        void set_pixel(Point p, char_type ch);
        void set_pixel(coord_t x, coord_t y);
        void set_pixel(Point p);

        char_type pixel() const;
        void pixel(char_type ch);

        friend TextCanvas operator!(const TextCanvas& bin);
        friend TextCanvas operator&(const TextCanvas& bin1, const TextCanvas& bin2);
        friend TextCanvas operator|(const TextCanvas& bin1, const TextCanvas& bin2);
        friend TextCanvas operator^(const TextCanvas& bin1, const TextCanvas& bin2);

        void fill(const TextCanvas& bin);
        void do_mask(const TextCanvas& image, const TextCanvas& mask);

        void flood_fill(coord_t x, coord_t y, char_type ch, bool surface = false);
        void flood_fill(Point p, char_type ch, bool surface = false);
        void flood_fill_bordered(coord_t x, coord_t y, char_type border_ch);
        void flood_fill_bordered(Point p, char_type border_ch);
        void flood_fill_surface(coord_t x, coord_t y, char_type surface_ch);
        void flood_fill_surface(Point p, char_type surface_ch);

        void line(coord_t x0, coord_t y0, coord_t x1, coord_t y1);
        void line(Point p0, Point p1);
        void rectangle(coord_t x0, coord_t y0, coord_t x1, coord_t y1);
        void rectangle(Point p0, Point p1);
        void fill_rectangle(coord_t x0, coord_t y0, coord_t x1, coord_t y1);
        void fill_rectangle(Point p0, Point p1);
        void circle(coord_t x0, coord_t y0, coord_t r);
        void circle(const Point& c, coord_t r);
        void fill_circle(coord_t x0, coord_t y0, coord_t r);
        void fill_circle(const Point& c, coord_t r);
        void ellipse(coord_t x0, coord_t y0, coord_t x1, coord_t y1);
        void ellipse(Point p0, Point p1);
        void fill_ellipse(coord_t x0, coord_t y0, coord_t x1, coord_t y1);
        void fill_ellipse(Point p0, Point p1);
        void arc(coord_t x0, coord_t y0, coord_t x1, coord_t y1, float start_radian, float end_radian);
        void arc(Point p0, Point p1, float start_radian, float end_radian);
        void pie(coord_t x0, coord_t y0, coord_t x1, coord_t y1, float start_radian, float end_radian);
        void pie(Point p0, Point p1, float start_radian, float end_radian);
        void fill_pie(coord_t x0, coord_t y0, coord_t x1, coord_t y1, float start_radian, float end_radian);
        void fill_pie(Point p0, Point p1, float start_radian, float end_radian);
        void round_rect(coord_t x0, coord_t y0, coord_t x1, coord_t y1, coord_t rx, coord_t ry);
        void round_rect(Point p0, Point p1, coord_t rx, coord_t ry);
        void fill_round_rect(coord_t x0, coord_t y0, coord_t x1, coord_t y1, coord_t rx, coord_t ry);
        void fill_round_rect(Point p0, Point p1, coord_t rx, coord_t ry);

        void lines(size_t num_points, const Point *points);
        void lines(const Points& points);
        void polyline(size_t num_points, const Point *points);
        void polyline(const Points& points);
        void polygon(size_t num_points, const Point *points);
        void polygon(const Points& points);
        void fill_polygon(size_t num_points, const Point *points, bool alternate = false);
        void fill_polygon(const Points& points, bool alternate = false);
        void fill_polygon_alternate(size_t num_points, const Point *points);
        void fill_polygon_alternate(const Points& points);
        void fill_polygon_winding(size_t num_points, const Point *points);
        void fill_polygon_winding(const Points& points);
    };

    ///////////////////////////////////////////////////////////////////////////

    namespace
    {
        inline bool get_polygon_box(Point& p0, Point& p1, size_t num_points, const Point *points)
        {
            if (num_points == 0)
                return false;

            p0 = p1 = points[0];
            for (size_t i = 1; i < num_points; ++i)
            {
                coord_t x = points[i].x;
                if (x < p0.x)
                    p0.x = x;
                if (x > p1.x)
                    p1.x = x;
                coord_t y = points[i].y;
                if (y < p0.y)
                    p0.y = y;
                if (y > p1.y)
                    p1.y = y;
            }
            return true;
        }

        inline size_t crossing_number(Point p, const Points& points)
        {
            size_t ret = 0;
            for (size_t i = 0; i < points.size() - 1; ++i)
            {
                if ((points[i].y <= p.y && points[i + 1].y > p.y) ||
                    (points[i].y > p.y && points[i + 1].y <= p.y))
                {
                    float f = float(p.y - points[i].y) / (points[i + 1].y - points[i].y);
                    if (p.x - points[i].x < f * (points[i + 1].x - points[i].x))
                    {
                        ++ret;
                    }
                }
            }
            return ret;
        }
        inline coord_t winding_number(Point p, const Points& points)
        {
            coord_t ret = 0;
            for (size_t i = 0; i < points.size() - 1; ++i)
            {
                if (points[i].y <= p.y && points[i + 1].y > p.y)
                {
                    float f = float(p.y - points[i].y) / (points[i + 1].y - points[i].y);
                    if (p.x - points[i].x < f * (points[i + 1].x - points[i].x))
                        ++ret;
                }
                else if (points[i].y > p.y && points[i + 1].y <= p.y)
                {
                    float f = float(p.y - points[i].y) / (points[i + 1].y - points[i].y);
                    if (p.x - points[i].x < f * (points[i + 1].x - points[i].x))
                        --ret;
                }
            }
            return ret;
        }
    }

    inline TextCanvas::TextCanvas() : m_width(0), m_height(0), m_text(), m_ch('*')
    {
    }
    inline TextCanvas::TextCanvas(coord_t width, coord_t height, char_type ch)
        : m_width(width), m_height(height), m_text(width * height, ch), m_ch('*')
    {
    }
    inline TextCanvas::TextCanvas(const TextCanvas& tc)
        : m_width(tc.m_width), m_height(tc.m_height), m_text(tc.m_text), m_ch(tc.m_ch)
    {
    }
    inline TextCanvas& TextCanvas::operator=(const TextCanvas& tc)
    {
        m_width = tc.m_width;
        m_height = tc.m_height;
        m_text = tc.m_text;
        return *this;
    }
    inline TextCanvas::~TextCanvas()
    {
    }

    inline coord_t TextCanvas::width() const
    {
        return m_width;
    }
    inline coord_t TextCanvas::height() const
    {
        return m_height;
    }
    inline coord_t TextCanvas::count() const
    {
        return m_width * m_height;
    }
    inline char_type& TextCanvas::operator[](size_t index)
    {
        return m_text[index];
    }
    inline const char_type& TextCanvas::operator[](size_t index) const
    {
        return m_text[index];
    }

    inline string_type TextCanvas::to_str() const
    {
        string_type ret;
        for (coord_t y = 0; y < m_height; ++y)
        {
            ret += m_text.substr(y * m_width, m_width);
            ret += newline;
        }
        return ret;
    }
    inline bool TextCanvas::in_range(coord_t x, coord_t y) const
    {
        return (0 <= x && x < m_width && 0 <= y && y < m_height);
    }
    inline bool TextCanvas::in_range(Point p) const
    {
        return in_range(p.x, p.y);
    }
    inline string_type& TextCanvas::data()
    {
        return m_text;
    }
    inline const string_type& TextCanvas::data() const
    {
        return m_text;
    }
    inline bool TextCanvas::same_size(const TextCanvas& tc) const
    {
        return m_width == tc.m_width && m_height == tc.m_height;
    }

    inline void TextCanvas::reset(coord_t width, coord_t height, char_type ch)
    {
        m_width = width;
        m_height = height;
        m_text.assign(width * height, ch);
    }
    inline void TextCanvas::clear(char_type ch)
    {
        m_text.assign(m_width * m_height, ch);
    }
    inline void TextCanvas::resize(coord_t width, coord_t height, char_type ch)
    {
        const coord_t min_width = std::min(m_width, width);
        const coord_t min_height = std::min(m_height, height);
        string_type str(width * height, ch);
        for (coord_t y = 0; y < min_height; ++y)
        {
            for (coord_t x = 0; x < min_width; ++x)
            {
                str[y * width + x] = m_text[y * m_width + x];
            }
        }
        std::swap(str, m_text);
        m_width = width;
        m_height = height;
    }

    inline char_type TextCanvas::get_pixel(coord_t x, coord_t y) const
    {
        if (in_range(x, y))
        {
            return m_text[y * m_width + x];
        }
        return space;
    }
    inline char_type TextCanvas::get_pixel(Point p) const
    {
        return get_pixel(p.x, p.y);
    }
    inline void TextCanvas::set_pixel(coord_t x, coord_t y, char_type ch)
    {
        if (in_range(x, y))
        {
            m_text[y * m_width + x] = ch;
        }
    }
    inline void TextCanvas::set_pixel(Point p, char_type ch)
    {
        set_pixel(p.x, p.y, ch);
    }
    inline void TextCanvas::set_pixel(coord_t x, coord_t y)
    {
        set_pixel(x, y, m_ch);
    }
    inline void TextCanvas::set_pixel(Point p)
    {
        set_pixel(p, m_ch);
    }

    inline char_type TextCanvas::pixel() const
    {
        return m_ch;
    }
    inline void TextCanvas::pixel(char_type ch)
    {
        m_ch = ch;
    }

    inline TextCanvas operator!(const TextCanvas& bin)
    {
        TextCanvas ret(bin.width(), bin.height(), zero);
        for (coord_t i = 0; i < bin.count(); ++i)
        {
            ret[i] = !bin[i];
        }
        return ret;
    }
    inline TextCanvas operator&(const TextCanvas& bin1, const TextCanvas& bin2)
    {
        assert(bin1.same_size(bin2));
        TextCanvas ret(bin1.width(), bin1.height(), zero);
        for (coord_t i = 0; i < bin1.count(); ++i)
        {
            ret[i] = bin1[i] & bin2[i];
        }
        return ret;
    }
    inline TextCanvas operator|(const TextCanvas& bin1, const TextCanvas& bin2)
    {
        assert(bin1.same_size(bin2));
        TextCanvas ret(bin1.width(), bin1.height(), zero);
        for (coord_t i = 0; i < bin1.count(); ++i)
        {
            ret[i] = bin1[i] | bin2[i];
        }
        return ret;
    }
    inline TextCanvas operator^(const TextCanvas& bin1, const TextCanvas& bin2)
    {
        assert(bin1.same_size(bin2));
        TextCanvas ret(bin1.width(), bin1.height(), zero);
        for (coord_t i = 0; i < bin1.count(); ++i)
        {
            ret[i] = bin1[i] ^ bin2[i];
        }
        return ret;
    }

    inline void TextCanvas::fill(const TextCanvas& bin)
    {
        assert(same_size(bin));
        for (coord_t i = 0; i < count(); ++i)
        {
            if (bin[i])
            {
                m_text[i] = m_ch;
            }
        }
    }
    inline void TextCanvas::do_mask(const TextCanvas& image, const TextCanvas& mask)
    {
        assert(same_size(image));
        assert(same_size(mask));
        for (coord_t i = 0; i < count(); ++i)
        {
            if (mask[i] && image[i])
            {
                m_text[i] = image[i];
            }
        }
    }

    inline void TextCanvas::flood_fill(coord_t x, coord_t y, char_type ch, bool surface)
    {
        if (surface)
            flood_fill_surface(x, y, ch);
        else
            flood_fill_bordered(x, y, ch);
    }
    inline void TextCanvas::flood_fill(Point p, char_type ch, bool surface)
    {
        flood_fill(p.x, p.y, ch, surface);
    }
    inline void TextCanvas::flood_fill_bordered(coord_t x, coord_t y, char_type border_ch)
    {
        Points points;
        Point p(x, y), p2;
        if (!in_range(p))
            return;

        points.push_back(p);
        for (size_t i = 0; i < points.size(); ++i)
        {
            p = points[i];
            if (get_pixel(p) != border_ch)
            {
                set_pixel(p, m_ch);
                p2.x = p.x - 1;
                p2.y = p.y;
                if (in_range(p2) && get_pixel(p2) != border_ch &&
                    std::find(points.begin(), points.end(), p2) == points.end())
                {
                    points.push_back(p2);
                }
                p2.x = p.x + 1;
                p2.y = p.y;
                if (in_range(p2) && get_pixel(p2) != border_ch &&
                    std::find(points.begin(), points.end(), p2) == points.end())
                {
                    points.push_back(p2);
                }
                p2.x = p.x;
                p2.y = p.y - 1;
                if (in_range(p2) && get_pixel(p2) != border_ch &&
                    std::find(points.begin(), points.end(), p2) == points.end())
                {
                    points.push_back(p2);
                }
                p2.x = p.x;
                p2.y = p.y + 1;
                if (in_range(p2) && get_pixel(p2) != border_ch &&
                    std::find(points.begin(), points.end(), p2) == points.end())
                {
                    points.push_back(p2);
                }
            }
        }
    }
    inline void TextCanvas::flood_fill_surface(coord_t x, coord_t y, char_type surface_ch)
    {
        Points points;
        Point p(x, y), p2;
        if (!in_range(p))
            return;

        points.push_back(p);
        for (size_t i = 0; i < points.size(); ++i)
        {
            p = points[i];
            if (get_pixel(p) == surface_ch)
            {
                set_pixel(p, m_ch);
                p2.x = p.x - 1;
                p2.y = p.y;
                if (in_range(p2) && get_pixel(p2) == surface_ch)
                    points.push_back(p2);
                p2.x = p.x + 1;
                p2.y = p.y;
                if (in_range(p2) && get_pixel(p2) == surface_ch)
                    points.push_back(p2);
                p2.x = p.x;
                p2.y = p.y - 1;
                if (in_range(p2) && get_pixel(p2) == surface_ch)
                    points.push_back(p2);
                p2.x = p.x;
                p2.y = p.y + 1;
                if (in_range(p2) && get_pixel(p2) == surface_ch)
                    points.push_back(p2);
            }
        }
    }
    inline void TextCanvas::flood_fill_bordered(Point p, char_type border_ch)
    {
        flood_fill_bordered(p.x, p.y, border_ch);
    }
    inline void TextCanvas::flood_fill_surface(Point p, char_type surface_ch)
    {
        flood_fill_surface(p.x, p.y, surface_ch);
    }

    inline void TextCanvas::line(coord_t x0, coord_t y0, coord_t x1, coord_t y1)
    {
        const coord_t dx = std::abs(x1 - x0);
        const coord_t dy = std::abs(y1 - y0);
        const coord_t sx = (x0 < x1) ? 1 : -1;
        const coord_t sy = (y0 < y1) ? 1 : -1;
        coord_t err = dx - dy;
        for (;;)
        {
            set_pixel(x0, y0);
            if (x0 == x1 && y0 == y1)
                break;

            coord_t e2 = err * 2;
            if (e2 > -dy)
            {
                err -= dy;
                x0 += sx;
            }
            if (e2 < dx)
            {
                err += dx;
                y0 += sy;
            }
        }
    }
    inline void TextCanvas::line(Point p0, Point p1)
    {
        line(p0.x, p0.y, p1.x, p1.y);
    }

    inline void TextCanvas::rectangle(coord_t x0, coord_t y0, coord_t x1, coord_t y1)
    {
        line(x0, y0, x1, y0);
        line(x1, y0, x1, y1);
        line(x1, y1, x0, y1);
        line(x0, y1, x0, y0);
    }
    inline void TextCanvas::rectangle(Point p0, Point p1)
    {
        rectangle(p0.x, p0.y, p1.x, p1.y);
    }
    inline void TextCanvas::fill_rectangle(coord_t x0, coord_t y0, coord_t x1, coord_t y1)
    {
        if (x0 > x1)
            std::swap(x0, x1);
        if (y0 > y1)
            std::swap(y0, y1);
        if (x0 < 0)
            x0 = 0;
        if (x1 > m_width)
            x1 = m_width - 1;
        if (y0 < 0)
            y0 = 0;
        if (y1 > m_height)
            y1 = m_height - 1;
        for (coord_t y = y0; y <= y1; ++y)
        {
            for (coord_t x = x0; x <= x1; ++x)
            {
                set_pixel(x, y);
            }
        }
    }
    inline void TextCanvas::fill_rectangle(Point p0, Point p1)
    {
        fill_rectangle(p0.x, p0.y, p1.x, p1.y);
    }

    inline void TextCanvas::circle(coord_t x0, coord_t y0, coord_t r)
    {
        coord_t x = r, y = 0;
        coord_t f = 3 - 2 * x;

        while (x >= y)
        {
            set_pixel(x0 + x, y0 + y);
            set_pixel(x0 - x, y0 + y);
            set_pixel(x0 + x, y0 - y);
            set_pixel(x0 - x, y0 - y);
            set_pixel(x0 + y, y0 + x);
            set_pixel(x0 - y, y0 + x);
            set_pixel(x0 + y, y0 - x);
            set_pixel(x0 - y, y0 - x);
            if (f >= 0)
            {
                --x;
                f -= 4 * x;
            }
            ++y;
            f += 4 * y + 2;
        }
    }
    inline void TextCanvas::circle(const Point& c, coord_t r)
    {
        circle(c.x, c.y, r);
    }
    inline void TextCanvas::fill_circle(coord_t x0, coord_t y0, coord_t r)
    {
        coord_t x = r, y = 0;
        coord_t f = 3 - 2 * x;

        while (x >= y)
        {
            for (coord_t px = x0 - x; px <= x0 + x; ++px)
            {
                set_pixel(px, y0 + y);
                set_pixel(px, y0 - y);
            }
            for (coord_t px = x0 - y; px <= x0 + y; ++px)
            {
                set_pixel(px, y0 + x);
                set_pixel(px, y0 - x);
            }
            if (f >= 0)
            {
                --x;
                f -= 4 * x;
            }
            ++y;
            f += 4 * y + 2;
        }
    }
    inline void TextCanvas::fill_circle(const Point& c, coord_t r)
    {
        fill_circle(c.x, c.y, r);
    }

    inline void TextCanvas::ellipse(coord_t x0, coord_t y0, coord_t x1, coord_t y1)
    {
        if (x0 > x1)
            std::swap(x0, x1);
        if (y0 > y1)
            std::swap(y0, y1);

        coord_t dx = x1 - x0, dy = y1 - y0;
        float u = dx * 0.5, v = dy * 0.5;
        coord_t u0 = coord_t(u), v0 = coord_t(v);
        float px = x0 + u, py = y0 + v;

        if (dx <= 1 && dy <= 1)
        {
            for (coord_t y = y0; y <= y1; ++y)
            {
                for (coord_t x = x0; x <= x1; ++x)
                {
                    set_pixel(x, y);
                }
            }
            return;
        }
        if (u < 1)
        {
            for (coord_t y = y0; y <= y1; ++y)
            {
                set_pixel(x0, y);
            }
            if (dx == 1)
            {
                for (coord_t y = -dy / 4; y <= dy / 4; ++y)
                {
                    set_pixel(x0 + 1, py + y);
                }
            }
            return;
        }
        if (v < 1)
        {
            for (coord_t x = x0; x <= x1; ++x)
            {
                set_pixel(x, y0);
            }
            if (dy == 1)
            {
                for (coord_t x = -dx / 4; x <= dx / 4; ++x)
                {
                    set_pixel(px + x, y0 + 1);
                }
            }
            return;
        }

        if (dy & 1)
            --py;
        float a = float(v * v);
        float b = float(u * u);
        float r = float(u * v);
        float x = float(u), y = 0;
        float d = u * (v * v);
        float F = (float)(-2.0 * d) + a + 2 * b;
        float H = (float)(-4.0 * d) + 2 * a + b;

        if (!(dy & 1) && dx < 3)
        {
            set_pixel(px + x, round(py + y));
            set_pixel(px - x, round(py + y));
        }

        while (x >= 0)
        {
            if (dy & 1)
            {
                set_pixel(round(px + x), round(py + y) + 1);
                set_pixel(round(px - x), round(py + y) + 1);
                set_pixel(round(px + x), round(py - y));
                set_pixel(round(px - x), round(py - y));
            }
            else if (dx < 3)
            {
                set_pixel(round(px + x), round(py + y) + 1);
                set_pixel(round(px - x), round(py + y) + 1);
                set_pixel(round(px + x), round(py - y) - 1);
                set_pixel(round(px - x), round(py - y) - 1);
            }
            else
            {
                set_pixel(round(px + x), round(py + y));
                set_pixel(round(px - x), round(py + y));
                set_pixel(round(px + x), round(py - y));
                set_pixel(round(px - x), round(py - y));
            }
            if (F >= 0)
            {
                x -= 1;
                F -= 4 * a * x;
                H -= 4 * a * x - 2 * a;
            }
            if (H < 0)
            {
                y += 1;
                F += 4 * b * y + 2 * b;
                H += 4 * b * y;
            }
        }
        y += 1;
        while (y <= v0)
        {
            if (dy & 1)
            {
                if (!(dx & 1))
                {
                    set_pixel(round(px), round(py - y));
                    set_pixel(round(px), round(py + y) + 1);
                }
            }
            else
            {
                if (dx & 1)
                {
                    set_pixel(round(px), round(py - y));
                    set_pixel(round(px), round(py + y));
                    set_pixel(round(px) - 1, round(py - y));
                    set_pixel(round(px) - 1, round(py + y));
                }
                else
                {
                    set_pixel(round(px), round(py - y));
                    set_pixel(round(px), round(py + y));
                }
            }
            y += 1;
        }
    }
    inline void TextCanvas::fill_ellipse(coord_t x0, coord_t y0, coord_t x1, coord_t y1)
    {
        if (x0 > x1)
            std::swap(x0, x1);
        if (y0 > y1)
            std::swap(y0, y1);

        coord_t dx = x1 - x0, dy = y1 - y0;
        float u = dx * 0.5, v = dy * 0.5;
        coord_t u0 = coord_t(u), v0 = coord_t(v);
        float px = x0 + u, py = y0 + v;

        if (dx <= 1 && dy <= 1)
        {
            for (coord_t y = y0; y <= y1; ++y)
            {
                for (coord_t x = x0; x <= x1; ++x)
                {
                    set_pixel(x, y);
                }
            }
            return;
        }
        if (u < 1)
        {
            for (coord_t y = y0; y <= y1; ++y)
            {
                set_pixel(x0, y);
            }
            if (dx == 1)
            {
                for (coord_t y = -dy / 4; y <= dy / 4; ++y)
                {
                    set_pixel(x0 + 1, py + y);
                }
            }
            return;
        }
        if (v < 1)
        {
            for (coord_t x = x0; x <= x1; ++x)
            {
                set_pixel(x, y0);
            }
            if (dy == 1)
            {
                for (coord_t x = -dx / 4; x <= dx / 4; ++x)
                {
                    set_pixel(px + x, y0 + 1);
                }
            }
            return;
        }

        if (dy & 1)
            --py;
        float a = float(v * v);
        float b = float(u * u);
        float r = float(u * v);
        float x = float(u), y = 0;
        float d = u * (v * v);
        float F = (float)(-2.0 * d) + a + 2 * b;
        float H = (float)(-4.0 * d) + 2 * a + b;

        if (!(dy & 1) && dx < 3)
        {
            for (coord_t qx = round(px - x); qx <= round(px + x); ++qx)
            {
                set_pixel(qx, round(py + y));
            }
        }

        while (x >= 0)
        {
            if (dy & 1)
            {
                for (coord_t qx = round(px - x); qx <= round(px + x); ++qx)
                {
                    set_pixel(qx, round(py + y) + 1);
                    set_pixel(qx, round(py - y));
                }
            }
            else if (dx < 3)
            {
                for (coord_t qx = round(px - x); qx <= round(px + x); ++qx)
                {
                    set_pixel(qx, round(py + y) + 1);
                    set_pixel(qx, round(py - y) - 1);
                }
            }
            else
            {
                for (coord_t qx = round(px - x); qx <= round(px + x); ++qx)
                {
                    set_pixel(qx, round(py + y));
                    set_pixel(qx, round(py - y));
                }
            }
            if (F >= 0)
            {
                x -= 1;
                F -= 4 * a * x;
                H -= 4 * a * x - 2 * a;
            }
            if (H < 0)
            {
                y += 1;
                F += 4 * b * y + 2 * b;
                H += 4 * b * y;
            }
        }
        y += 1;
        while (y <= v0)
        {
            if (dy & 1)
            {
                if (!(dx & 1))
                {
                    set_pixel(round(px), round(py - y));
                    set_pixel(round(px), round(py + y) + 1);
                }
            }
            else
            {
                if (dx & 1)
                {
                    set_pixel(round(px), round(py - y));
                    set_pixel(round(px), round(py + y));
                    set_pixel(round(px) - 1, round(py - y));
                    set_pixel(round(px) - 1, round(py + y));
                }
                else
                {
                    set_pixel(round(px), round(py - y));
                    set_pixel(round(px), round(py + y));
                }
            }
            y += 1;
        }
    }

    inline arc_area(TextCanvas& mask, float px, float py, coord_t x0, coord_t y0, coord_t x1, coord_t y1,
                    float u, float v, float start_radian, float end_radian)
    {
        for (coord_t y = y0; y <= y1; ++y)
        {
            for (coord_t x = x0; x <= x1; ++x)
            {
                double at = atan2(y - py, x - px);  // [-pi, pi]
                if (start_radian <= end_radian)
                {
                    if (start_radian <= at && at <= end_radian)
                    {
                        mask.set_pixel(x, y);
                        continue;
                    }
                }
                else
                {
                    if ((start_radian <= at && at <= pi) ||
                        (-pi <= at && at <= end_radian))
                    {
                        mask.set_pixel(x, y);
                        continue;
                    }
                }
            }
        }

        coord_t qx = px + u * cos(start_radian);
        coord_t qy = py + v * sin(start_radian);
        mask.line(px, py, qx, qy);
        coord_t rx = px + u * cos(end_radian);
        coord_t ry = py + v * sin(end_radian);
        mask.line(px, py, rx, ry);
    }

    inline void TextCanvas::arc(coord_t x0, coord_t y0, coord_t x1, coord_t y1, float start_radian, float end_radian)
    {
        if (start_radian > end_radian)
            return;

        start_radian = normalize_radian(start_radian);
        end_radian = normalize_radian(end_radian);

        if (x0 > x1)
            std::swap(x0, x1);
        if (y0 > y1)
            std::swap(y0, y1);

        coord_t dx = x1 - x0, dy = y1 - y0;
        float u = dx * 0.5, v = dy * 0.5;
        float px = x0 + u, py = y0 + v;

        TextCanvas mask(width(), height(), 0);
        arc_area(mask, px, py, x0, y0, x1, y1, u, v, start_radian, end_radian);

        TextCanvas image(*this);
        image.ellipse(x0, y0, x1, y1);

        do_mask(image, mask);
    }
    inline void TextCanvas::pie(coord_t x0, coord_t y0, coord_t x1, coord_t y1, float start_radian, float end_radian)
    {
        if (start_radian > end_radian)
            return;

        start_radian = normalize_radian(start_radian);
        end_radian = normalize_radian(end_radian);

        if (x0 > x1)
            std::swap(x0, x1);
        if (y0 > y1)
            std::swap(y0, y1);

        coord_t dx = x1 - x0, dy = y1 - y0;
        float u = dx * 0.5, v = dy * 0.5;
        float px = x0 + u, py = y0 + v;

        TextCanvas mask(width(), height(), 0);
        arc_area(mask, px, py, x0, y0, x1, y1, u, v, start_radian, end_radian);

        TextCanvas image(*this);
        image.ellipse(x0, y0, x1, y1);

        coord_t qx = px + u * cos(start_radian);
        coord_t qy = py + v * sin(start_radian);
        image.line(px, py, qx, qy);
        coord_t rx = px + u * cos(end_radian);
        coord_t ry = py + v * sin(end_radian);
        image.line(px, py, rx, ry);

        do_mask(image, mask);
    }
    inline void TextCanvas::fill_pie(coord_t x0, coord_t y0, coord_t x1, coord_t y1, float start_radian, float end_radian)
    {
        if (start_radian > end_radian)
            return;

        start_radian = normalize_radian(start_radian);
        end_radian = normalize_radian(end_radian);

        if (x0 > x1)
            std::swap(x0, x1);
        if (y0 > y1)
            std::swap(y0, y1);

        coord_t dx = x1 - x0, dy = y1 - y0;
        float u = dx * 0.5, v = dy * 0.5;
        float px = x0 + u, py = y0 + v;

        TextCanvas mask(width(), height(), 0);
        arc_area(mask, px, py, x0, y0, x1, y1, u, v, start_radian, end_radian);

        TextCanvas image(*this);
        image.fill_ellipse(x0, y0, x1, y1);
        image.ellipse(x0, y0, x1, y1);

        do_mask(image, mask);
    }

    inline void TextCanvas::ellipse(Point p0, Point p1)
    {
        ellipse(p0.x, p0.y, p1.x, p1.y);
    }
    inline void TextCanvas::fill_ellipse(Point p0, Point p1)
    {
        fill_ellipse(p0.x, p0.y, p1.x, p1.y);
    }
    inline void TextCanvas::arc(Point p0, Point p1, float start_radian, float end_radian)
    {
        arc(p0.x, p0.y, p1.x, p1.y, start_radian, end_radian);
    }
    inline void TextCanvas::pie(Point p0, Point p1, float start_radian, float end_radian)
    {
        pie(p0.x, p0.y, p1.x, p1.y, start_radian, end_radian);
    }
    inline void TextCanvas::fill_pie(Point p0, Point p1, float start_radian, float end_radian)
    {
        fill_pie(p0.x, p0.y, p1.x, p1.y, start_radian, end_radian);
    }

    inline void TextCanvas::round_rect(coord_t x0, coord_t y0, coord_t x1, coord_t y1, coord_t rx, coord_t ry)
    {
        if (x0 > x1)
            std::swap(x0, x1);
        if (y0 > y1)
            std::swap(y0, y1);

        coord_t dx = x1 - x0, dy = y1 - y0;
        if (dx <= 2 * rx)
            rx = dx / 2;
        if (dy <= 2 * ry)
            ry = dy / 2;

        line(x0 + rx, y0, x1 - rx, y0);
        arc(x1 - 2 * rx, y0, x1, y0 + 2 * ry, pi / -2, 0);
        line(x1, y0 + ry, x1, y1 - ry);
        arc(x1 - 2 * rx, y1 - 2 * ry, x1, y1, 0, pi / 2);
        line(x1 - rx, y1, x0 + rx, y1);
        arc(x0, y1 - 2 * rx, x0 + 2 * rx, y1, pi / 2, pi);
        line(x0, y1 - ry, x0, y0 + ry);
        arc(x0, y0, x0 + 2 * rx, y0 + 2 * ry, -pi, pi / -2);
    }
    inline void TextCanvas::fill_round_rect(coord_t x0, coord_t y0, coord_t x1, coord_t y1, coord_t rx, coord_t ry)
    {
        if (x0 > x1)
            std::swap(x0, x1);
        if (y0 > y1)
            std::swap(y0, y1);

        coord_t dx = x1 - x0, dy = y1 - y0;
        if (dx <= 2 * rx)
            rx = dx / 2;
        if (dy <= 2 * ry)
            ry = dy / 2;

        fill_rectangle(x0 + rx, y0, x1 - rx, y0 + ry);
        fill_pie(x1 - 2 * rx, y0, x1, y0 + 2 * ry, pi / -2, 0);
        fill_rectangle(x1 - rx, y0 + ry, x1, y1 - ry);
        fill_pie(x1 - 2 * rx, y1 - 2 * ry, x1, y1, 0, pi / 2);
        fill_rectangle(x1 - rx, y1 - ry, x0 + rx, y1);
        fill_pie(x0, y1 - 2 * rx, x0 + 2 * rx, y1, pi / 2, pi);
        fill_rectangle(x0, y1 - ry, x0 + rx, y0 + ry);
        fill_pie(x0, y0, x0 + 2 * rx, y0 + 2 * ry, -pi, pi / -2);

        fill_rectangle(x0 + rx, y0 + ry, x1 - rx, y1 - ry);
    }
    inline void TextCanvas::round_rect(Point p0, Point p1, coord_t rx, coord_t ry)
    {
        round_rect(p0.x, p0.y, p1.x, p1.y, rx, ry);
    }
    inline void TextCanvas::fill_round_rect(Point p0, Point p1, coord_t rx, coord_t ry)
    {
        fill_round_rect(p0.x, p0.y, p1.x, p1.y, rx, ry);
    }

    inline void TextCanvas::lines(size_t num_points, const Point *points)
    {
        for (size_t i = 0; i < num_points; i += 2)
        {
            line(points[i], points[i + 1]);
        }
    }
    inline void TextCanvas::lines(const Points& points)
    {
        lines(points.size(), &points[0]);
    }
    inline void TextCanvas::polyline(size_t num_points, const Point *points)
    {
        if (num_points > 0)
        {
            for (size_t i = 0; i < num_points - 1; ++i)
            {
                line(points[i], points[i + 1]);
            }
        }
    }
    inline void TextCanvas::polyline(const Points& points)
    {
        polyline(points.size(), &points[0]);
    }
    inline void TextCanvas::polygon(size_t num_points, const Point *points)
    {
        if (num_points > 0)
        {
            for (size_t i = 0; i < num_points - 1; ++i)
            {
                line(points[i], points[i + 1]);
            }
            line(points[num_points - 1], points[0]);
        }
    }
    inline void TextCanvas::polygon(const Points& points)
    {
        polygon(points.size(), &points[0]);
    }

    inline void TextCanvas::fill_polygon_alternate(const Points& points)
    {
        Point p0, p1;
        if (get_polygon_box(p0, p1, points.size(), &points[0]))
        {
            Points new_points = points;
            new_points.push_back(points[0]);

            for (coord_t y = p0.y; y <= p1.y; ++y)
            {
                for (coord_t x = p0.x; x <= p1.x; ++x)
                {
                    if (crossing_number(Point(x, y), new_points) & 1)
                    {
                        set_pixel(x, y);
                    }
                }
            }
        }
    }
    inline void TextCanvas::fill_polygon_winding(const Points& points)
    {
        Point p0, p1;
        if (get_polygon_box(p0, p1, points.size(), &points[0]))
        {
            Points new_points = points;
            new_points.push_back(points[0]);

            for (coord_t y = p0.y; y <= p1.y; ++y)
            {
                for (coord_t x = p0.x; x <= p1.x; ++x)
                {
                    if (winding_number(Point(x, y), new_points))
                    {
                        set_pixel(x, y);
                    }
                }
            }
        }
    }

    inline void TextCanvas::fill_polygon_alternate(size_t num_points, const Point *points)
    {
        Point p0, p1;
        if (get_polygon_box(p0, p1, num_points, points))
        {
            Points new_points(&points[0], &points[num_points]);
            new_points.push_back(points[0]);

            for (coord_t y = p0.y; y <= p1.y; ++y)
            {
                for (coord_t x = p0.x; x <= p1.x; ++x)
                {
                    if (crossing_number(Point(x, y), new_points) & 1)
                    {
                        set_pixel(x, y);
                    }
                }
            }
        }
    }
    inline void TextCanvas::fill_polygon_winding(size_t num_points, const Point *points)
    {
        Point p0, p1;
        if (get_polygon_box(p0, p1, num_points, points))
        {
            Points new_points(&points[0], &points[num_points]);
            new_points.push_back(points[0]);

            for (coord_t y = p0.y; y <= p1.y; ++y)
            {
                for (coord_t x = p0.x; x <= p1.x; ++x)
                {
                    if (winding_number(Point(x, y), new_points))
                    {
                        set_pixel(x, y);
                    }
                }
            }
        }
    }

    inline void TextCanvas::fill_polygon(size_t num_points, const Point *points, bool alternate)
    {
        if (alternate)
            fill_polygon_alternate(num_points, points);
        else
            fill_polygon_winding(num_points, points);
    }

    inline void TextCanvas::fill_polygon(const Points& points, bool alternate)
    {
        if (alternate)
            fill_polygon_alternate(points);
        else
            fill_polygon_winding(points);
    }
} // namespace textcanvas

///////////////////////////////////////////////////////////////////////////

#endif  // ndef TEXT_CANVAS_HPP_
