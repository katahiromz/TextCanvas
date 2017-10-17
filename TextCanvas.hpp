// TextCanvas.hpp --- TextCanvas by katahiromz
///////////////////////////////////////////////////////////////////////////

#ifndef TEXT_CANVAS_HPP_
#define TEXT_CANVAS_HPP_    11  // Version 11

#if _MSC_VER > 1000
    #pragma once
#endif

#include <string>       // for std::string
#include <vector>       // for std::vector
#include <algorithm>    // for std::swap, std::find
#include <cmath>        // for std::sqrt, std::atan2, std::fmod, ...
#include <cassert>      // for assert macro

///////////////////////////////////////////////////////////////////////////

namespace textcanvas
{
    typedef std::string                 string_type;
    typedef string_type::value_type     char_type;
    typedef char_type                   color_t;
    typedef std::ptrdiff_t              coord_t;

#ifdef M_PI
    static const double pi = M_PI;
#else
    static const double pi = 3.14159265358979323846;
#endif
    static const double epsilon = 0.00001;

    static const color_t zero = color_t(0);
    static const color_t one = color_t(1);
    static const color_t space = color_t(' ');
    static const color_t newline = color_t('\n');
    static const color_t asterisk = color_t('*');

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

    inline bool operator==(const Point& p0, const Point& p1)
    {
        return p0.x == p1.x && p0.y == p1.y;
    }
    inline bool operator!=(const Point& p0, const Point& p1)
    {
        return p0.x != p1.x || p0.y != p1.y;
    }

    ///////////////////////////////////////////////////////////////////////////
    // functions

    inline coord_t do_round(double d)
    {
        return coord_t(d + 0.5);
    }

    double normalize_radian(double radian);

    inline bool
    in_rect(coord_t x, coord_t y, coord_t x0, coord_t y0, coord_t x1, coord_t y1)
    {
        return x0 <= x && x <= x1 && y0 <= y && y <= y1;
    }

    bool get_polygon_box(Point& p0, Point& p1, size_t num_points, const Point *points);

    size_t crossing_number(const Point& p, const Points& points);
    coord_t winding_number(const Point& p, const Points& points);

    bool check_range(double at, bool clockwise, double a0, double a1);

    ///////////////////////////////////////////////////////////////////////////
    // TextCanvas

    class TextCanvas
    {
    public:
        TextCanvas();
        TextCanvas(coord_t width, coord_t height,
                   color_t fore_color = asterisk, color_t back_color = space);
        TextCanvas(const TextCanvas& tc);
        TextCanvas& operator=(const TextCanvas& tc);
        virtual ~TextCanvas();

    protected:
        coord_t     m_width;
        coord_t     m_height;
        string_type m_text;
        color_t     m_fore_color;
        color_t     m_back_color;
        Point       m_pos;

    public:
        coord_t width() const;
        coord_t height() const;
        coord_t count() const;
              Point& pos();
        const Point& pos() const;
              color_t& operator[](size_t index);
        const color_t& operator[](size_t index) const;

        string_type to_str() const;
        bool in_range(coord_t x, coord_t y) const;
        bool in_range(const Point& p) const;
              string_type& data();
        const string_type& data() const;
        bool same_size(const TextCanvas& tc) const;

        void clear();
        void clear(color_t ch);
        void reset(coord_t width, coord_t height);
        void reset(coord_t width, coord_t height, color_t ch);
        void resize(coord_t width, coord_t height);
        void resize(coord_t width, coord_t height, color_t ch);

        color_t get_pixel(coord_t x, coord_t y) const;
        color_t get_pixel(const Point& p) const;
        void put_pixel(coord_t x, coord_t y, color_t ch);
        void put_pixel(const Point& p, color_t ch);
        void put_pixel(coord_t x, coord_t y);
        void put_pixel(const Point& p);

        color_t fore_color() const;
        void fore_color(color_t ch);

        color_t back_color() const;
        void back_color(color_t ch);

        friend TextCanvas operator!(const TextCanvas& bin);
        friend TextCanvas operator&(const TextCanvas& bin1, const TextCanvas& bin2);
        friend TextCanvas operator|(const TextCanvas& bin1, const TextCanvas& bin2);
        friend TextCanvas operator^(const TextCanvas& bin1, const TextCanvas& bin2);

        void fill(const TextCanvas& bin);
        void do_mask(const TextCanvas& image, const TextCanvas& mask);
        void get_subimage(TextCanvas& image, coord_t x0, coord_t y0, coord_t x1, coord_t y1) const;
        void put_subimage(coord_t x0, coord_t y0, const TextCanvas& image);
        void put_subimage(coord_t x0, coord_t y0, const TextCanvas& image, coord_t x_zoom, coord_t y_zoom);

        void flood_fill(coord_t x, coord_t y, color_t ch, bool surface = false);
        void flood_fill(const Point& p, color_t ch, bool surface = false);
        void flood_fill_bordered(coord_t x, coord_t y, color_t border_ch);
        void flood_fill_bordered(const Point& p, color_t border_ch);
        void flood_fill_surface(coord_t x, coord_t y, color_t surface_ch);
        void flood_fill_surface(const Point& p, color_t surface_ch);

        void move_to(coord_t x, coord_t y);
        void move_to(const Point& p);

        void line_to(coord_t x, coord_t y);
        void line_to(const Point& p);
        template <typename T_PUTTER>
        void line_to(coord_t x, coord_t y, T_PUTTER& putter);
        template <typename T_PUTTER>
        void line_to(const Point& p, T_PUTTER& putter);

        void line(coord_t x0, coord_t y0, coord_t x1, coord_t y1);
        void line(const Point& p0, const Point& p1);
        template <typename T_PUTTER>
        void line(coord_t x0, coord_t y0, coord_t x1, coord_t y1, T_PUTTER& putter);

        void rectangle(coord_t x0, coord_t y0, coord_t x1, coord_t y1);
        void rectangle(const Point& p0, const Point& p1);
        template <typename T_PUTTER>
        void rectangle(coord_t x0, coord_t y0, coord_t x1, coord_t y1, T_PUTTER& putter);

        void fill_rectangle(coord_t x0, coord_t y0, coord_t x1, coord_t y1);
        void fill_rectangle(const Point& p0, const Point& p1);
        template <typename T_PUTTER>
        void fill_rectangle(coord_t x0, coord_t y0, coord_t x1, coord_t y1, T_PUTTER& putter);

        void circle(coord_t x0, coord_t y0, coord_t r);
        void circle(const Point& c, coord_t r);
        template <typename T_PUTTER>
        void circle(coord_t x0, coord_t y0, coord_t r, T_PUTTER& putter);

        void fill_circle(coord_t x0, coord_t y0, coord_t r);
        void fill_circle(const Point& c, coord_t r);
        template <typename T_PUTTER>
        void fill_circle(coord_t x0, coord_t y0, coord_t r, T_PUTTER& putter);

        void ellipse(coord_t x0, coord_t y0, coord_t x1, coord_t y1);
        void ellipse(const Point& p0, const Point& p1);
        template <typename T_PUTTER>
        void ellipse(coord_t x0, coord_t y0, coord_t x1, coord_t y1, T_PUTTER& putter);

        void fill_ellipse(coord_t x0, coord_t y0, coord_t x1, coord_t y1);
        void fill_ellipse(const Point& p0, const Point& p1);
        template <typename T_PUTTER>
        void fill_ellipse(coord_t x0, coord_t y0, coord_t x1, coord_t y1, T_PUTTER& putter);

        void arc(coord_t x0, coord_t y0, coord_t x1, coord_t y1, double start_radian, double end_radian, bool clockwise = false);
        void arc(const Point& p0, const Point& p1, double start_radian, double end_radian, bool clockwise = false);
        template <typename T_PUTTER>
        void arc(coord_t x0, coord_t y0, coord_t x1, coord_t y1, double start_radian, double end_radian, bool clockwise, T_PUTTER& putter);

        void arc_to(coord_t x0, coord_t y0, coord_t x1, coord_t y1, double start_radian, double end_radian, bool clockwise = false);
        void arc_to(const Point& p0, const Point& p1, double start_radian, double end_radian, bool clockwise = false);
        template <typename T_PUTTER>
        void arc_to(coord_t x0, coord_t y0, coord_t x1, coord_t y1, double start_radian, double end_radian, bool clockwise, T_PUTTER& putter);

        void pie(coord_t x0, coord_t y0, coord_t x1, coord_t y1, double start_radian, double end_radian);
        void pie(const Point& p0, const Point& p1, double start_radian, double end_radian);
        template <typename T_PUTTER>
        void pie(coord_t x0, coord_t y0, coord_t x1, coord_t y1, double start_radian, double end_radian, T_PUTTER& putter);

        void fill_pie(coord_t x0, coord_t y0, coord_t x1, coord_t y1, double start_radian, double end_radian, bool clockwise = false);
        void fill_pie(const Point& p0, const Point& p1, double start_radian, double end_radian, bool clockwise = false);
        template <typename T_PUTTER>
        void fill_pie(coord_t x0, coord_t y0, coord_t x1, coord_t y1, double start_radian, double end_radian, bool clockwise, T_PUTTER& putter);

        void round_rect(coord_t x0, coord_t y0, coord_t x1, coord_t y1, coord_t rx, coord_t ry);
        void round_rect(const Point& p0, const Point& p1, coord_t rx, coord_t ry);
        template <typename T_PUTTER>
        void round_rect(coord_t x0, coord_t y0, coord_t x1, coord_t y1, coord_t rx, coord_t ry, T_PUTTER& putter);

        void fill_round_rect(coord_t x0, coord_t y0, coord_t x1, coord_t y1, coord_t rx, coord_t ry);
        void fill_round_rect(const Point& p0, const Point& p1, coord_t rx, coord_t ry);
        template <typename T_PUTTER>
        void fill_round_rect(coord_t x0, coord_t y0, coord_t x1, coord_t y1, coord_t rx, coord_t ry, T_PUTTER& putter);

        void lines(size_t num_points, const Point *points);
        void lines(const Points& points);
        template <typename T_PUTTER>
        void lines(size_t num_points, const Point *points, T_PUTTER& putter);

        void polyline(size_t num_points, const Point *points);
        void polyline(const Points& points);
        template <typename T_PUTTER>
        void polyline(size_t num_points, const Point *points, T_PUTTER& putter);

        void polygon(size_t num_points, const Point *points);
        void polygon(const Points& points);
        template <typename T_PUTTER>
        void polygon(size_t num_points, const Point *points, T_PUTTER& putter);

        void fill_polygon(size_t num_points, const Point *points, bool alternate = false);
        void fill_polygon(const Points& points, bool alternate = false);
        template <typename T_PUTTER>
        void fill_polygon(size_t num_points, const Point *points, bool alternate, T_PUTTER& putter);

        template <typename T_PUTTER>
        void fill_polygon_alternate(size_t num_points, const Point *points, T_PUTTER& putter);
        template <typename T_PUTTER>
        void fill_polygon_alternate(const Points& points, T_PUTTER& putter);

        template <typename T_PUTTER>
        void fill_polygon_winding(size_t num_points, const Point *points, T_PUTTER& putter);
        template <typename T_PUTTER>
        void fill_polygon_winding(const Points& points, T_PUTTER& putter);
    };

    ///////////////////////////////////////////////////////////////////////////
    // pixel putters

    struct NonePutter
    {
        NonePutter()
        {
        }
        void operator()(coord_t x, coord_t y)
        {
        }
    };
    struct SimplePutter
    {
        TextCanvas& m_tc;
        SimplePutter(TextCanvas& tc) : m_tc(tc)
        {
        }
        void operator()(coord_t x, coord_t y)
        {
            m_tc.put_pixel(x, y);
        }
    };
    struct ColorPutter
    {
        TextCanvas& m_tc;
        color_t m_color;
        ColorPutter(TextCanvas& tc, color_t color) : m_tc(tc), m_color(color)
        {
        }
        void operator()(coord_t x, coord_t y)
        {
            m_tc.put_pixel(x, y, m_color);
        }
    };
    struct XorPutter
    {
        TextCanvas& m_tc;
        XorPutter(TextCanvas& tc) : m_tc(tc)
        {
        }
        void operator()(coord_t x, coord_t y)
        {
            color_t c = m_tc.get_pixel(x, y);
            m_tc.put_pixel(x, y, c ^ m_tc.fore_color());
        }
    };
    struct WidenPutter
    {
        TextCanvas& m_tc;
        WidenPutter(TextCanvas& tc) : m_tc(tc)
        {
        }
        void operator()(coord_t x, coord_t y)
        {
            m_tc.put_pixel(x, y);
            m_tc.put_pixel(x - 1, y);
            m_tc.put_pixel(x + 1, y);
            m_tc.put_pixel(x, y - 1);
            m_tc.put_pixel(x, y + 1);
        }
    };

    ///////////////////////////////////////////////////////////////////////////

    inline double normalize_radian(double radian)
    {
        radian = fmod(radian, 2 * pi);
        if (radian < -pi)
            radian += 2 * pi;
        else if (radian > pi)
            radian -= 2 * pi;
        return radian;
    }

    inline bool check_range(double at, bool clockwise, double a0, double a1)
    {
        if (clockwise)
        {
            if ((-pi <= at && at <= a0 + epsilon) ||
                (a1 - epsilon <= at && at <= pi))
            {
                return true;
            }
        }
        else
        {
            if (a0 - epsilon <= at && at <= a1 + epsilon)
            {
                return true;
            }
        }
        return false;
    }

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

    inline size_t crossing_number(const Point& p, const Points& points)
    {
        size_t ret = 0;
        for (size_t i = 0; i < points.size() - 1; ++i)
        {
            if ((points[i].y <= p.y && points[i + 1].y > p.y) ||
                (points[i].y > p.y && points[i + 1].y <= p.y))
            {
                double f = double(p.y - points[i].y) / (points[i + 1].y - points[i].y);
                if (p.x - points[i].x < f * (points[i + 1].x - points[i].x))
                {
                    ++ret;
                }
            }
        }
        return ret;
    }
    inline coord_t winding_number(const Point& p, const Points& points)
    {
        coord_t ret = 0;
        for (size_t i = 0; i < points.size() - 1; ++i)
        {
            if (points[i].y <= p.y && points[i + 1].y > p.y)
            {
                double f = double(p.y - points[i].y) / (points[i + 1].y - points[i].y);
                if (p.x - points[i].x < f * (points[i + 1].x - points[i].x))
                    ++ret;
            }
            else if (points[i].y > p.y && points[i + 1].y <= p.y)
            {
                double f = double(p.y - points[i].y) / (points[i + 1].y - points[i].y);
                if (p.x - points[i].x < f * (points[i + 1].x - points[i].x))
                    --ret;
            }
        }
        return ret;
    }

    inline TextCanvas::TextCanvas()
        : m_width(0), m_height(0), m_text(),
          m_fore_color(asterisk), m_back_color(space), m_pos(0, 0)
    {
    }
    inline TextCanvas::TextCanvas(coord_t width, coord_t height, color_t fore_color, color_t back_color)
        : m_width(width), m_height(height), m_text(width * height, back_color),
          m_fore_color(fore_color), m_back_color(back_color), m_pos(0, 0)
    {
    }
    inline TextCanvas::TextCanvas(const TextCanvas& tc)
        : m_width(tc.m_width), m_height(tc.m_height), m_text(tc.m_text),
          m_fore_color(tc.m_fore_color), m_back_color(tc.m_back_color), m_pos(tc.m_pos)
    {
    }
    inline TextCanvas& TextCanvas::operator=(const TextCanvas& tc)
    {
        m_width = tc.m_width;
        m_height = tc.m_height;
        m_text = tc.m_text;
        m_fore_color = tc.m_fore_color;
        m_back_color = tc.m_back_color;
        m_pos = tc.m_pos;
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
    inline Point& TextCanvas::pos()
    {
        return m_pos;
    }
    inline const Point& TextCanvas::pos() const
    {
        return m_pos;
    }
    inline color_t& TextCanvas::operator[](size_t index)
    {
        return m_text[index];
    }
    inline const color_t& TextCanvas::operator[](size_t index) const
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
    inline bool TextCanvas::in_range(const Point& p) const
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

    inline void TextCanvas::clear(color_t ch)
    {
        m_text.assign(m_width * m_height, ch);
    }
    inline void TextCanvas::clear()
    {
        clear(m_back_color);
    }

    inline void TextCanvas::reset(coord_t width, coord_t height, color_t ch)
    {
        m_width = width;
        m_height = height;
        m_text.assign(width * height, ch);
        m_pos.x = m_pos.y = 0;
    }
    inline void TextCanvas::reset(coord_t width, coord_t height)
    {
        reset(width, height, m_back_color);
    }

    inline void TextCanvas::resize(coord_t width, coord_t height, color_t ch)
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
    inline void TextCanvas::resize(coord_t width, coord_t height)
    {
        resize(width, height, m_back_color);
    }

    inline color_t TextCanvas::get_pixel(coord_t x, coord_t y) const
    {
        if (in_range(x, y))
        {
            return m_text[y * m_width + x];
        }
        return space;
    }
    inline color_t TextCanvas::get_pixel(const Point& p) const
    {
        return get_pixel(p.x, p.y);
    }
    inline void TextCanvas::put_pixel(coord_t x, coord_t y, color_t ch)
    {
        if (in_range(x, y))
        {
            m_text[y * m_width + x] = ch;
        }
        m_pos.x = x;
        m_pos.y = y;
    }
    inline void TextCanvas::put_pixel(const Point& p, color_t ch)
    {
        put_pixel(p.x, p.y, ch);
    }
    inline void TextCanvas::put_pixel(coord_t x, coord_t y)
    {
        put_pixel(x, y, m_fore_color);
    }
    inline void TextCanvas::put_pixel(const Point& p)
    {
        put_pixel(p, m_fore_color);
    }

    inline color_t TextCanvas::fore_color() const
    {
        return m_fore_color;
    }
    inline void TextCanvas::fore_color(color_t ch)
    {
        m_fore_color = ch;
    }

    inline color_t TextCanvas::back_color() const
    {
        return m_back_color;
    }
    inline void TextCanvas::back_color(color_t ch)
    {
        m_back_color = ch;
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
                m_text[i] = m_fore_color;
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

    inline void TextCanvas::get_subimage(TextCanvas& image, coord_t x0, coord_t y0, coord_t x1, coord_t y1) const
    {
        if (x0 > x1)
            std::swap(x0, x1);
        if (y0 > y1)
            std::swap(y0, y1);

        image.reset(x1 - x0 + 1, y1 - y0 + 1, 0);

        coord_t py = 0;
        for (coord_t y = y0; y <= y1; ++y)
        {
            coord_t px = 0;
            for (coord_t x = x0; x <= x1; ++x)
            {
                color_t ch = get_pixel(x, y);
                image.put_pixel(px, py, ch);
                ++px;
            }
            ++py;
        }
    }
    inline void TextCanvas::put_subimage(coord_t x0, coord_t y0, const TextCanvas& image)
    {
        put_subimage(x0, y0, image, 1, 1);
    }
    inline void TextCanvas::put_subimage(coord_t x0, coord_t y0, const TextCanvas& image, coord_t x_zoom, coord_t y_zoom)
    {
        for (coord_t py = 0; py < image.height(); ++py)
        {
            for (coord_t px = 0; px < image.width(); ++px)
            {
                color_t ch = image.get_pixel(px, py);
                for (coord_t y2 = 0; y2 < y_zoom; ++y2)
                {
                    for (coord_t x2 = 0; x2 < x_zoom; ++x2)
                    {
                        put_pixel(x0 + px * x_zoom + x2, y0 + py * y_zoom + y2, ch);
                    }
                }
            }
        }
    }

    inline void TextCanvas::flood_fill(coord_t x, coord_t y, color_t ch, bool surface)
    {
        if (surface)
            flood_fill_surface(x, y, ch);
        else
            flood_fill_bordered(x, y, ch);
    }
    inline void TextCanvas::flood_fill(const Point& p, color_t ch, bool surface)
    {
        flood_fill(p.x, p.y, ch, surface);
    }
    inline void TextCanvas::flood_fill_bordered(coord_t x, coord_t y, color_t border_ch)
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
                put_pixel(p);
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
        m_pos.x = x;
        m_pos.y = y;
    }
    inline void TextCanvas::flood_fill_surface(coord_t x, coord_t y, color_t surface_ch)
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
                put_pixel(p);
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
        m_pos.x = x;
        m_pos.y = y;
    }
    inline void TextCanvas::flood_fill_bordered(const Point& p, color_t border_ch)
    {
        flood_fill_bordered(p.x, p.y, border_ch);
    }
    inline void TextCanvas::flood_fill_surface(const Point& p, color_t surface_ch)
    {
        flood_fill_surface(p.x, p.y, surface_ch);
    }

    inline void TextCanvas::move_to(coord_t x, coord_t y)
    {
        m_pos.x = x;
        m_pos.y = y;
    }
    inline void TextCanvas::move_to(const Point& p)
    {
        m_pos = p;
    }

    inline void TextCanvas::line_to(coord_t x, coord_t y)
    {
        line(m_pos.x, m_pos.y, x, y);
    }
    inline void TextCanvas::line_to(const Point& p)
    {
        line(m_pos, p);
    }
    template <typename T_PUTTER>
    inline void TextCanvas::line_to(coord_t x, coord_t y, T_PUTTER& putter)
    {
        line(m_pos.x, m_pos.y, x, y, putter);
    }
    template <typename T_PUTTER>
    inline void TextCanvas::line_to(const Point& p, T_PUTTER& putter)
    {
        line_to(p.x, p.y, putter);
    }

    inline void TextCanvas::line(coord_t x0, coord_t y0, coord_t x1, coord_t y1)
    {
        SimplePutter putter(*this);
        line(x0, y0, x1, y1, putter);
    }
    inline void TextCanvas::line(const Point& p0, const Point& p1)
    {
        line(p0.x, p0.y, p1.x, p1.y);
    }
    template <typename T_PUTTER>
    inline void TextCanvas::line(coord_t x0, coord_t y0, coord_t x1, coord_t y1, T_PUTTER& putter)
    {
        // Bresenham's line algorithm
        const coord_t dx = std::abs(x1 - x0), dy = std::abs(y1 - y0);
        const coord_t sx = (x0 < x1) ? 1 : -1, sy = (y0 < y1) ? 1 : -1;
        coord_t err = dx - dy;
        for (;;)
        {
            putter(x0, y0);
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
        m_pos.x = x1;
        m_pos.y = y1;
    }

    inline void TextCanvas::rectangle(coord_t x0, coord_t y0, coord_t x1, coord_t y1)
    {
        SimplePutter putter(*this);
        rectangle(x0, y0, x1, y1, putter);
    }
    inline void TextCanvas::rectangle(const Point& p0, const Point& p1)
    {
        rectangle(p0.x, p0.y, p1.x, p1.y);
    }
    template <typename T_PUTTER>
    inline void TextCanvas::rectangle(coord_t x0, coord_t y0, coord_t x1, coord_t y1, T_PUTTER& putter)
    {
        line(x0, y0, x1, y0, putter);
        line(x1, y0, x1, y1, putter);
        line(x1, y1, x0, y1, putter);
        line(x0, y1, x0, y0, putter);
        m_pos.x = x1;
        m_pos.y = y1;
    }

    inline void TextCanvas::fill_rectangle(coord_t x0, coord_t y0, coord_t x1, coord_t y1)
    {
        SimplePutter putter(*this);
        fill_rectangle(x0, y0, x1, y1, putter);
    }
    inline void TextCanvas::fill_rectangle(const Point& p0, const Point& p1)
    {
        fill_rectangle(p0.x, p0.y, p1.x, p1.y);
    }
    template <typename T_PUTTER>
    inline void TextCanvas::fill_rectangle(coord_t x0, coord_t y0, coord_t x1, coord_t y1, T_PUTTER& putter)
    {
        const coord_t x2 = x1, y2 = y1;
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
                putter(x, y);
            }
        }
        m_pos.x = x2;
        m_pos.y = y2;
    }

    inline void TextCanvas::circle(coord_t x0, coord_t y0, coord_t r)
    {
        SimplePutter putter(*this);
        circle(x0, y0, r, putter);
    }
    inline void TextCanvas::circle(const Point& c, coord_t r)
    {
        circle(c.x, c.y, r);
    }
    template <typename T_PUTTER>
    inline void TextCanvas::circle(coord_t x0, coord_t y0, coord_t r, T_PUTTER& putter)
    {
        coord_t x = r, y = 0;
        coord_t f = 3 - 2 * x;

        while (x >= y)
        {
            putter(x0 + x, y0 + y);
            putter(x0 - x, y0 + y);
            putter(x0 + x, y0 - y);
            putter(x0 - x, y0 - y);
            putter(x0 + y, y0 + x);
            putter(x0 - y, y0 + x);
            putter(x0 + y, y0 - x);
            putter(x0 - y, y0 - x);
            if (f >= 0)
            {
                --x;
                f -= 4 * x;
            }
            ++y;
            f += 4 * y + 2;
        }
        m_pos.x = x0;
        m_pos.y = y0;
    }

    inline void TextCanvas::fill_circle(coord_t x0, coord_t y0, coord_t r)
    {
        SimplePutter putter(*this);
        fill_circle(x0, y0, r, putter);
    }
    inline void TextCanvas::fill_circle(const Point& c, coord_t r)
    {
        fill_circle(c.x, c.y, r);
    }
    template <typename T_PUTTER>
    inline void TextCanvas::fill_circle(coord_t x0, coord_t y0, coord_t r, T_PUTTER& putter)
    {
        coord_t x = r, y = 0;
        coord_t f = 3 - 2 * x;

        while (x >= y)
        {
            for (coord_t px = x0 - x; px <= x0 + x; ++px)
            {
                putter(px, y0 + y);
                putter(px, y0 - y);
            }
            for (coord_t px = x0 - y; px <= x0 + y; ++px)
            {
                putter(px, y0 + x);
                putter(px, y0 - x);
            }
            if (f >= 0)
            {
                --x;
                f -= 4 * x;
            }
            ++y;
            f += 4 * y + 2;
        }
        m_pos.x = x0;
        m_pos.y = y0;
    }

    template <typename T_PUTTER>
    inline void TextCanvas::ellipse(coord_t x0, coord_t y0, coord_t x1, coord_t y1, T_PUTTER& putter)
    {
        if (x0 > x1)
            std::swap(x0, x1);
        if (y0 > y1)
            std::swap(y0, y1);

        coord_t dx = x1 - x0, dy = y1 - y0;
        double u = dx * 0.5, v = dy * 0.5;
        coord_t u0 = coord_t(u), v0 = coord_t(v);
        double px = x0 + u, py = y0 + v;

        if (dx <= 1 || dy <= 1)
        {
            for (coord_t y = y0; y <= y1; ++y)
            {
                for (coord_t x = x0; x <= x1; ++x)
                {
                    putter(x, y);
                }
            }
            m_pos.x = do_round(px);
            m_pos.y = do_round(py);
            return;
        }
        if (u < 1)
        {
            for (coord_t y = y0; y <= y1; ++y)
            {
                putter(x0, y);
            }
            m_pos.x = do_round(px);
            m_pos.y = do_round(py);
            return;
        }
        if (v < 1)
        {
            for (coord_t x = x0; x <= x1; ++x)
            {
                putter(x, y0);
            }
            m_pos.x = do_round(px);
            m_pos.y = do_round(py);
            return;
        }

        if (dy & 1)
            --py;
        double a = double(v * v);
        double b = double(u * u);
        double r = double(u * v);
        double x = double(u), y = 0;
        double d = u * (v * v);
        double F = (double)(-2.0 * d) + a + 2 * b;
        double H = (double)(-4.0 * d) + 2 * a + b;

        if (!(dy & 1) && dx < 3)
        {
            putter(coord_t(px + x), do_round(py + y));
            putter(coord_t(px - x), do_round(py + y));
        }

        while (x >= 0)
        {
            if (dy & 1)
            {
                putter(do_round(px + x), do_round(py + y) + 1);
                putter(do_round(px - x), do_round(py + y) + 1);
                putter(do_round(px + x), do_round(py - y));
                putter(do_round(px - x), do_round(py - y));
            }
            else if (dx < 3)
            {
                putter(do_round(px + x), do_round(py + y) + 1);
                putter(do_round(px - x), do_round(py + y) + 1);
                putter(do_round(px + x), do_round(py - y) - 1);
                putter(do_round(px - x), do_round(py - y) - 1);
            }
            else
            {
                putter(do_round(px + x), do_round(py + y));
                putter(do_round(px - x), do_round(py + y));
                putter(do_round(px + x), do_round(py - y));
                putter(do_round(px - x), do_round(py - y));
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
                    putter(do_round(px), do_round(py - y));
                    putter(do_round(px), do_round(py + y) + 1);
                }
            }
            else
            {
                if (dx & 1)
                {
                    putter(do_round(px), do_round(py - y));
                    putter(do_round(px), do_round(py + y));
                    putter(do_round(px) - 1, do_round(py - y));
                    putter(do_round(px) - 1, do_round(py + y));
                }
                else
                {
                    putter(do_round(px), do_round(py - y));
                    putter(do_round(px), do_round(py + y));
                }
            }
            y += 1;
        }

        m_pos.x = do_round(px);
        m_pos.y = do_round(py);
    }
    template <typename T_PUTTER>
    inline void TextCanvas::fill_ellipse(coord_t x0, coord_t y0, coord_t x1, coord_t y1, T_PUTTER& putter)
    {
        if (x0 > x1)
            std::swap(x0, x1);
        if (y0 > y1)
            std::swap(y0, y1);

        coord_t dx = x1 - x0, dy = y1 - y0;
        double u = dx * 0.5, v = dy * 0.5;
        coord_t u0 = coord_t(u), v0 = coord_t(v);
        double px = x0 + u, py = y0 + v;

        if (dx <= 1 || dy <= 1)
        {
            for (coord_t y = y0; y <= y1; ++y)
            {
                for (coord_t x = x0; x <= x1; ++x)
                {
                    putter(x, y);
                }
            }
            m_pos.x = do_round(px);
            m_pos.y = do_round(py);
            return;
        }
        if (u < 1)
        {
            for (coord_t y = y0; y <= y1; ++y)
            {
                putter(x0, y);
            }
            m_pos.x = do_round(px);
            m_pos.y = do_round(py);
            return;
        }
        if (v < 1)
        {
            for (coord_t x = x0; x <= x1; ++x)
            {
                putter(x, y0);
            }
            m_pos.x = do_round(px);
            m_pos.y = do_round(py);
            return;
        }

        if (dy & 1)
            --py;
        double a = double(v * v);
        double b = double(u * u);
        double r = double(u * v);
        double x = double(u), y = 0;
        double d = u * (v * v);
        double F = (double)(-2.0 * d) + a + 2 * b;
        double H = (double)(-4.0 * d) + 2 * a + b;

        if (!(dy & 1) && dx < 3)
        {
            for (coord_t qx = do_round(px - x); qx <= do_round(px + x); ++qx)
            {
                putter(qx, do_round(py + y));
            }
        }

        while (x >= 0)
        {
            if (dy & 1)
            {
                for (coord_t qx = do_round(px - x); qx <= do_round(px + x); ++qx)
                {
                    putter(qx, do_round(py + y) + 1);
                    putter(qx, do_round(py - y));
                }
            }
            else if (dx < 3)
            {
                for (coord_t qx = do_round(px - x); qx <= do_round(px + x); ++qx)
                {
                    putter(qx, do_round(py + y) + 1);
                    putter(qx, do_round(py - y) - 1);
                }
            }
            else
            {
                for (coord_t qx = do_round(px - x); qx <= do_round(px + x); ++qx)
                {
                    putter(qx, do_round(py + y));
                    putter(qx, do_round(py - y));
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
                    putter(do_round(px), do_round(py - y));
                    putter(do_round(px), do_round(py + y) + 1);
                }
            }
            else
            {
                if (dx & 1)
                {
                    putter(do_round(px), do_round(py - y));
                    putter(do_round(px), do_round(py + y));
                    putter(do_round(px) - 1, do_round(py - y));
                    putter(do_round(px) - 1, do_round(py + y));
                }
                else
                {
                    putter(do_round(px), do_round(py - y));
                    putter(do_round(px), do_round(py + y));
                }
            }
            y += 1;
        }

        m_pos.x = do_round(px);
        m_pos.y = do_round(py);
    }

    inline void TextCanvas::ellipse(coord_t x0, coord_t y0, coord_t x1, coord_t y1)
    {
        SimplePutter putter(*this);
        ellipse(x0, y0, x1, y1, putter);
    }
    inline void TextCanvas::fill_ellipse(coord_t x0, coord_t y0, coord_t x1, coord_t y1)
    {
        SimplePutter putter(*this);
        fill_ellipse(x0, y0, x1, y1, putter);
    }

    template <typename T_PUTTER>
    inline void TextCanvas::arc(coord_t x0, coord_t y0, coord_t x1, coord_t y1, double start_radian, double end_radian, bool clockwise, T_PUTTER& putter)
    {
        bool whole_range = fabs(end_radian - start_radian) >= 2 * pi;
        start_radian = normalize_radian(start_radian);
        end_radian = normalize_radian(end_radian);
        if (start_radian > end_radian)
        {
            std::swap(start_radian, end_radian);
            clockwise = !clockwise;
        }

        if (x0 > x1)
            std::swap(x0, x1);
        if (y0 > y1)
            std::swap(y0, y1);

        coord_t dx = x1 - x0, dy = y1 - y0;
        double u = dx * 0.5, v = dy * 0.5;
        coord_t u0 = coord_t(u), v0 = coord_t(v);
        double px = x0 + u, py = y0 + v;

        struct PUTTER
        {
            T_PUTTER& putter;
            double px, py, u, v;
            double start_radian, end_radian;
            bool clockwise, whole_range;
            PUTTER(T_PUTTER& putter_, double px_, double py_, double u_, double v_,
                   double start_radian_, double end_radian_,
                   bool clockwise_, bool whole_range_)
                : putter(putter_), px(px_), py(py_), u(u_), v(v_),
                  start_radian(start_radian_), end_radian(end_radian_),
                  clockwise(clockwise_), whole_range(whole_range_)
            {
            }
            bool check(double& at, coord_t x, coord_t y) const
            {
                at = atan2(-u * (y - py), v * (x - px));
                return whole_range || check_range(at, clockwise, start_radian, end_radian);
            }
            void operator()(coord_t x, coord_t y)
            {
                double at;
                if (check(at, x, y))
                {
                    putter(x, y);
                }
            }
        } another_putter(putter, px, py, u, v, start_radian, end_radian, clockwise, whole_range);
        ellipse(x0, y0, x1, y1, another_putter);
    }
    inline void TextCanvas::arc(coord_t x0, coord_t y0, coord_t x1, coord_t y1, double start_radian, double end_radian, bool clockwise)
    {
        SimplePutter putter(*this);
        arc(x0, y0, x1, y1, start_radian, end_radian, clockwise, putter);
    }

    template <typename T_PUTTER>
    inline void TextCanvas::arc_to(coord_t x0, coord_t y0, coord_t x1, coord_t y1, double start_radian, double end_radian, bool clockwise, T_PUTTER& putter)
    {
        Point old_pos = pos();

        bool whole_range = fabs(end_radian - start_radian) >= 2 * pi;
        start_radian = normalize_radian(start_radian);
        end_radian = normalize_radian(end_radian);
        if (start_radian > end_radian)
        {
            std::swap(start_radian, end_radian);
            clockwise = !clockwise;
        }

        if (x0 > x1)
            std::swap(x0, x1);
        if (y0 > y1)
            std::swap(y0, y1);

        coord_t dx = x1 - x0, dy = y1 - y0;
        double u = dx * 0.5, v = dy * 0.5;
        coord_t u0 = coord_t(u), v0 = coord_t(v);
        double px = x0 + u, py = y0 + v;

        struct PUTTER
        {
            T_PUTTER& putter;
            double px, py, u, v;
            double start_radian, end_radian;
            bool clockwise, whole_range;
            double q_radian, qx, qy;
            double r_radian, rx, ry;
            PUTTER(T_PUTTER& putter_,
                double px_, double py_, double u_, double v_,
                double start_radian_, double end_radian_,
                bool clockwise_, bool whole_range_,
                double q_radian_, double qx_, double qy_,
                double r_radian_, double rx_, double ry_)
                : putter(putter_), px(px_), py(py_), u(u_), v(v_), 
                  start_radian(start_radian_), end_radian(end_radian_), 
                  clockwise(clockwise_), whole_range(whole_range_), 
                  q_radian(q_radian_), qx(qx_), qy(qy_), 
                  r_radian(r_radian_), rx(rx_), ry(ry_)
            {
            }
            bool check(double& at, coord_t x, coord_t y) const
            {
                at = atan2(-u * (y - py), v * (x - px));
                return whole_range || check_range(at, clockwise, start_radian, end_radian);
            }
            void operator()(coord_t x, coord_t y)
            {
                double at;
                if (check(at, x, y))
                {
                    putter(x, y);
                    update(at, x, y);
                }
            }
            void update(double at, coord_t x, coord_t y)
            {
                if (fabs(at - start_radian) < fabs(q_radian - start_radian))
                {
                    q_radian = at;
                    qx = double(x);
                    qy = double(y);
                }
                if (fabs(at - end_radian) < fabs(r_radian - end_radian))
                {
                    r_radian = at;
                    rx = double(x);
                    ry = double(y);
                }
            }
        } another_putter(putter, px, py, u, v, start_radian, end_radian, clockwise,
                         whole_range, 100, px, py, -100, px, py);
        ellipse(x0, y0, x1, y1, another_putter);
        line(old_pos.x, old_pos.y, do_round(another_putter.qx), do_round(another_putter.qy), putter);
    }
    inline void TextCanvas::arc_to(coord_t x0, coord_t y0, coord_t x1, coord_t y1, double start_radian, double end_radian, bool clockwise)
    {
        SimplePutter putter(*this);
        arc_to(x0, y0, x1, y1, start_radian, end_radian, clockwise, putter);
    }

    template <typename T_PUTTER>
    inline void TextCanvas::pie(coord_t x0, coord_t y0, coord_t x1, coord_t y1, double start_radian, double end_radian, T_PUTTER& putter)
    {
        bool clockwise = false;
        bool whole_range = fabs(end_radian - start_radian) >= 2 * pi;
        start_radian = normalize_radian(start_radian);
        end_radian = normalize_radian(end_radian);
        if (start_radian > end_radian)
        {
            std::swap(start_radian, end_radian);
            clockwise = !clockwise;
        }

        if (x0 > x1)
            std::swap(x0, x1);
        if (y0 > y1)
            std::swap(y0, y1);

        coord_t dx = x1 - x0, dy = y1 - y0;
        double u = dx * 0.5, v = dy * 0.5;
        coord_t u0 = coord_t(u), v0 = coord_t(v);
        double px = x0 + u, py = y0 + v;

        struct PUTTER
        {
            T_PUTTER& putter;
            double px, py, u, v;
            double start_radian, end_radian;
            bool clockwise, whole_range;
            double q_radian, qx, qy;
            double r_radian, rx, ry;
            PUTTER(T_PUTTER& putter_,
                double px_, double py_, double u_, double v_,
                double start_radian_, double end_radian_,
                bool clockwise_, bool whole_range_,
                double q_radian_, double qx_, double qy_,
                double r_radian_, double rx_, double ry_)
                : putter(putter_), px(px_), py(py_), u(u_), v(v_), 
                  start_radian(start_radian_), end_radian(end_radian_), 
                  clockwise(clockwise_), whole_range(whole_range_), 
                  q_radian(q_radian_), qx(qx_), qy(qy_), 
                  r_radian(r_radian_), rx(rx_), ry(ry_)
            {
            }
            bool check(double& at, coord_t x, coord_t y) const
            {
                at = atan2(-u * (y - py), v * (x - px));
                return whole_range || check_range(at, clockwise, start_radian, end_radian);
            }
            void operator()(coord_t x, coord_t y)
            {
                double at;
                if (check(at, x, y))
                {
                    putter(x, y);
                    update(at, x, y);
                }
            }
            void update(double at, coord_t x, coord_t y)
            {
                if (fabs(at - start_radian) < fabs(q_radian - start_radian))
                {
                    q_radian = at;
                    qx = double(x);
                    qy = double(y);
                }
                if (fabs(at - end_radian) < fabs(r_radian - end_radian))
                {
                    r_radian = at;
                    rx = double(x);
                    ry = double(y);
                }
            }
        } another_putter(putter, px, py, u, v, start_radian, end_radian, clockwise,
                         whole_range, 100, px, py, -100, px, py);
        ellipse(x0, y0, x1, y1, another_putter);

        line(coord_t(px), coord_t(py), do_round(another_putter.qx), do_round(another_putter.qy), putter);
        line(coord_t(px), coord_t(py), do_round(another_putter.rx), do_round(another_putter.ry), putter);
    }

    inline void TextCanvas::pie(coord_t x0, coord_t y0, coord_t x1, coord_t y1, double start_radian, double end_radian)
    {
        SimplePutter putter(*this);
        pie(x0, y0, x1, y1, start_radian, end_radian, putter);
    }

    template <typename T_PUTTER>
    inline void TextCanvas::fill_pie(coord_t x0, coord_t y0, coord_t x1, coord_t y1, double start_radian, double end_radian, bool clockwise, T_PUTTER& putter)
    {
        bool whole_range = fabs(end_radian - start_radian) >= 2 * pi;
        start_radian = normalize_radian(start_radian);
        end_radian = normalize_radian(end_radian);
        if (start_radian > end_radian)
        {
            std::swap(start_radian, end_radian);
            clockwise = !clockwise;
        }

        if (x0 > x1)
            std::swap(x0, x1);
        if (y0 > y1)
            std::swap(y0, y1);

        coord_t dx = x1 - x0, dy = y1 - y0;
        double u = dx * 0.5, v = dy * 0.5;
        coord_t u0 = coord_t(u), v0 = coord_t(v);
        double px = x0 + u, py = y0 + v;

        struct PUTTER
        {
            T_PUTTER& putter;
            double px, py, u, v;
            double start_radian, end_radian;
            bool clockwise, whole_range;
            PUTTER(T_PUTTER& putter_,
                double px_, double py_, double u_, double v_,
                double start_radian_, double end_radian_,
                bool clockwise_, bool whole_range_)
                : putter(putter_), px(px_), py(py_), u(u_), v(v_), 
                  start_radian(start_radian_), end_radian(end_radian_), 
                  clockwise(clockwise_), whole_range(whole_range_)
            {
            }
            bool check(double& at, coord_t x, coord_t y) const
            {
                at = atan2(-u * (y - py), v * (x - px));
                return whole_range || check_range(at, clockwise, start_radian, end_radian);
            }
            void operator()(coord_t x, coord_t y)
            {
                double at;
                if (check(at, x, y))
                {
                    putter(x, y);
                }
            }
        } another_putter(putter, px, py, u, v, start_radian, end_radian, clockwise, whole_range);
        fill_ellipse(x0, y0, x1, y1, another_putter);
        putter(coord_t(px), coord_t(py));
    }
    inline void TextCanvas::fill_pie(coord_t x0, coord_t y0, coord_t x1, coord_t y1, double start_radian, double end_radian, bool clockwise)
    {
        SimplePutter putter(*this);
        fill_pie(x0, y0, x1, y1, start_radian, end_radian, clockwise, putter);
    }

    inline void TextCanvas::ellipse(const Point& p0, const Point& p1)
    {
        ellipse(p0.x, p0.y, p1.x, p1.y);
    }
    inline void TextCanvas::fill_ellipse(const Point& p0, const Point& p1)
    {
        fill_ellipse(p0.x, p0.y, p1.x, p1.y);
    }
    inline void TextCanvas::arc(const Point& p0, const Point& p1, double start_radian, double end_radian, bool clockwise)
    {
        arc(p0.x, p0.y, p1.x, p1.y, start_radian, end_radian, clockwise);
    }
    inline void TextCanvas::arc_to(const Point& p0, const Point& p1, double start_radian, double end_radian, bool clockwise)
    {
        arc_to(p0.x, p0.y, p1.x, p1.y, start_radian, end_radian, clockwise);
    }
    inline void TextCanvas::pie(const Point& p0, const Point& p1, double start_radian, double end_radian)
    {
        pie(p0.x, p0.y, p1.x, p1.y, start_radian, end_radian);
    }
    inline void TextCanvas::fill_pie(const Point& p0, const Point& p1, double start_radian, double end_radian, bool clockwise)
    {
        fill_pie(p0.x, p0.y, p1.x, p1.y, start_radian, end_radian, clockwise);
    }

    template <typename T_PUTTER>
    inline void TextCanvas::round_rect(coord_t x0, coord_t y0, coord_t x1, coord_t y1, coord_t rx, coord_t ry, T_PUTTER& putter)
    {
        Point old_pos = pos();

        if (x0 > x1)
            std::swap(x0, x1);
        if (y0 > y1)
            std::swap(y0, y1);

        coord_t dx = x1 - x0, dy = y1 - y0;
        if (dx <= 2 * rx)
            rx = dx / 2;
        if (dy <= 2 * ry)
            ry = dy / 2;

        line(x0 + rx, y0, x1 - rx, y0, putter);
        arc(x1 - 2 * rx, y0, x1, y0 + 2 * ry, pi / 2, 0, true, putter);
        line(x1, y0 + ry, x1, y1 - ry, putter);
        arc(x1 - 2 * rx, y1 - 2 * ry, x1, y1, 0, pi / -2, true, putter);
        line(x1 - rx, y1, x0 + rx, y1, putter);
        arc(x0, y1 - 2 * rx, x0 + 2 * rx, y1, pi / -2, -pi, true, putter);
        line(x0, y1 - ry, x0, y0 + ry, putter);
        arc(x0, y0, x0 + 2 * rx, y0 + 2 * ry, pi, pi / 2, true, putter);

        pos() = old_pos;
    }
    inline void TextCanvas::round_rect(coord_t x0, coord_t y0, coord_t x1, coord_t y1, coord_t rx, coord_t ry)
    {
        SimplePutter putter(*this);
        round_rect(x0, y0, x1, y1, rx, ry, putter);
    }
    inline void TextCanvas::round_rect(const Point& p0, const Point& p1, coord_t rx, coord_t ry)
    {
        round_rect(p0.x, p0.y, p1.x, p1.y, rx, ry);
    }

    template <typename T_PUTTER>
    inline void TextCanvas::fill_round_rect(coord_t x0, coord_t y0, coord_t x1, coord_t y1, coord_t rx, coord_t ry, T_PUTTER& putter)
    {
        Point old_pos = pos();

        if (x0 > x1)
            std::swap(x0, x1);
        if (y0 > y1)
            std::swap(y0, y1);

        coord_t dx = x1 - x0, dy = y1 - y0;
        if (dx <= 2 * rx)
            rx = dx / 2;
        if (dy <= 2 * ry)
            ry = dy / 2;

        fill_rectangle(x0 + rx, y0, x1 - rx, y0 + ry, putter);
        fill_pie(x1 - 2 * rx, y0, x1, y0 + 2 * ry, pi / 2, 0, true, putter);
        fill_rectangle(x1 - rx, y0 + ry, x1, y1 - ry, putter);
        fill_pie(x1 - 2 * rx, y1 - 2 * ry, x1, y1, 0, pi / -2, true, putter);
        fill_rectangle(x1 - rx, y1 - ry, x0 + rx, y1, putter);
        fill_pie(x0, y1 - 2 * rx, x0 + 2 * rx, y1, pi / -2, -pi, true, putter);
        fill_rectangle(x0, y1 - ry, x0 + rx, y0 + ry, putter);
        fill_pie(x0, y0, x0 + 2 * rx, y0 + 2 * ry, pi, pi / 2, true, putter);

        fill_rectangle(x0 + rx, y0 + ry, x1 - rx, y1 - ry, putter);

        pos() = old_pos;
    }
    inline void TextCanvas::fill_round_rect(coord_t x0, coord_t y0, coord_t x1, coord_t y1, coord_t rx, coord_t ry)
    {
        SimplePutter putter(*this);
        fill_round_rect(x0, y0, x1, y1, rx, ry, putter);
    }
    inline void TextCanvas::fill_round_rect(const Point& p0, const Point& p1, coord_t rx, coord_t ry)
    {
        fill_round_rect(p0.x, p0.y, p1.x, p1.y, rx, ry);
    }

    inline void TextCanvas::lines(size_t num_points, const Point *points)
    {
        for (size_t i = 0; i < num_points / 2; ++i)
        {
            line(points[2 * i], points[2 * i + 1]);
        }
    }
    inline void TextCanvas::lines(const Points& points)
    {
        lines(points.size(), &points[0]);
    }
    template <typename T_PUTTER>
    inline void TextCanvas::lines(size_t num_points, const Point *points, T_PUTTER& putter)
    {
        for (size_t i = 0; i < num_points / 2; ++i)
        {
            line(points[2 * i], points[2 * i + 1], putter);
        }
    }

    inline void TextCanvas::polyline(size_t num_points, const Point *points)
    {
        if (num_points > 0)
        {
            move_to(points[0]);
            for (size_t i = 1; i < num_points; ++i)
            {
                line_to(points[i]);
            }
        }
    }
    inline void TextCanvas::polyline(const Points& points)
    {
        polyline(points.size(), &points[0]);
    }
    template <typename T_PUTTER>
    inline void TextCanvas::polyline(size_t num_points, const Point *points, T_PUTTER& putter)
    {
        if (num_points > 0)
        {
            move_to(points[0]);
            for (size_t i = 1; i < num_points; ++i)
            {
                line_to(points[i], putter);
            }
        }
    }

    inline void TextCanvas::polygon(size_t num_points, const Point *points)
    {
        SimplePutter putter(*this);
        polygon(num_points, points, putter);
    }
    inline void TextCanvas::polygon(const Points& points)
    {
        polygon(points.size(), &points[0]);
    }
    template <typename T_PUTTER>
    inline void TextCanvas::polygon(size_t num_points, const Point *points, T_PUTTER& putter)
    {
        if (num_points > 0)
        {
            move_to(points[0]);
            for (size_t i = 1; i < num_points; ++i)
            {
                line_to(points[i], putter);
            }
            line_to(points[0], putter);
        }
    }

    template <typename T_PUTTER>
    inline void TextCanvas::fill_polygon_alternate(const Points& points, T_PUTTER& putter)
    {
        fill_polygon_alternate(points.size(), &points[0], putter);
    }
    template <typename T_PUTTER>
    inline void TextCanvas::fill_polygon_winding(const Points& points, T_PUTTER& putter)
    {
        fill_polygon_winding(points.size(), &points[0], putter);
    }

    template <typename T_PUTTER>
    inline void TextCanvas::fill_polygon_alternate(size_t num_points, const Point *points, T_PUTTER& putter)
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
                        putter(x, y);
                    }
                }
            }
            polyline(num_points + 1, &new_points[0], putter);
        }
    }
    template <typename T_PUTTER>
    inline void TextCanvas::fill_polygon_winding(size_t num_points, const Point *points, T_PUTTER& putter)
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
                        putter(x, y);
                    }
                }
            }
            polyline(num_points + 1, &new_points[0], putter);
        }
    }

    inline void TextCanvas::fill_polygon(size_t num_points, const Point *points, bool alternate)
    {
        SimplePutter putter(*this);
        if (alternate)
            fill_polygon_alternate(num_points, points, putter);
        else
            fill_polygon_winding(num_points, points, putter);
    }
    inline void TextCanvas::fill_polygon(const Points& points, bool alternate)
    {
        SimplePutter putter(*this);
        if (alternate)
            fill_polygon_alternate(points, putter);
        else
            fill_polygon_winding(points, putter);
    }
    template <typename T_PUTTER>
    inline void TextCanvas::fill_polygon(size_t num_points, const Point *points, bool alternate, T_PUTTER& putter)
    {
        if (alternate)
            fill_polygon_alternate(num_points, points, putter);
        else
            fill_polygon_winding(num_points, points, putter);
    }
} // namespace textcanvas

///////////////////////////////////////////////////////////////////////////

#endif  // ndef TEXT_CANVAS_HPP_
