// TextCanvas.hpp --- TextCanvas by katahiromz
///////////////////////////////////////////////////////////////////////////

#ifndef TEXT_CANVAS_HPP_
#define TEXT_CANVAS_HPP_    31  // Version 31

#if _MSC_VER > 1000
    #pragma once
#endif

#include <string>       // for std::string
#include <vector>       // for std::vector
#include <set>          // for std::set
#include <algorithm>    // for std::swap
#include <cmath>        // for std::sqrt, std::atan2, std::fmod, ...
#include <cstring>      // for std::memset, std::memcpy, ...
#include <cassert>      // for assert macro

#if __cplusplus >= 201103L
    #include <cstdint>      // uint8_t, uint16_t, ...
#else
    #include "pstdint.h"    // uint8_t, uint16_t, ...
#endif

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

    struct Point;
    class XbmImage;
        class XbmFont;
    class TextCanvas;

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
    typedef Point Size;
    typedef std::vector<Point> Points;

    inline bool operator==(const Point& p0, const Point& p1)
    {
        return p0.x == p1.x && p0.y == p1.y;
    }
    inline bool operator!=(const Point& p0, const Point& p1)
    {
        return p0.x != p1.x || p0.y != p1.y;
    }
    struct point_less
    {
        inline bool operator()(const Point& p0, const Point& p1) const
        {
            if (p0.x < p1.x)
                return true;
            return p0.x == p1.x && p0.y < p1.y;
        }
    };

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
    // Japanese character code related

    namespace japanese
    {
        inline bool is_jis_byte(uint8_t ch) {
            return ((0x21 <= ch) && (ch <= 0x7E));
        }
        inline bool is_sjis_lead(uint8_t ch) {
            return (
                ((0x81 <= ch) && (ch <= 0x9F)) ||
                ((0xE0 <= ch) && (ch <= 0xEF))
            );
        }
        inline bool is_sjis_trail(uint8_t ch) {
            return (
                ((0x40 <= ch) && (ch <= 0x7E)) ||
                ((0x80 <= ch) && (ch <= 0xFC))
            );
        }
        inline bool is_jis_code(uint16_t w) {
            uint8_t ch0 = uint8_t(w >> 8);
            uint8_t ch1 = uint8_t(w);
            return (is_jis_byte(ch0) && is_jis_byte(ch1));
        }
        inline uint16_t jis2sjis(uint8_t c0, uint8_t c1) {
            if (c0 & 0x01) {
                c0 >>= 1;
                if (c0 < 0x2F) {
                    c0 += 0x71;
                } else {
                    c0 -= 0x4F;
                }
                if (c1 > 0x5F) {
                    c1 += 0x20;
                } else {
                    c1 += 0x1F;
                }
            } else {
                c0 >>= 1;
                if (c0 < 0x2F) {
                    c0 += 0x70;
                } else {
                    c0 -= 0x50;
                }
                c1 += 0x7E;
            }
            uint16_t sjis_code = uint16_t((c0 << 8) | c1);
            return sjis_code;
        }
        inline uint16_t jis2sjis(uint16_t jis_code) {
            uint8_t c0 = uint8_t(jis_code >> 8);
            uint8_t c1 = uint8_t(jis_code);
            return jis2sjis(c0, c1);
        }
        inline bool is_sjis_code(uint16_t w) {
            return (
                is_sjis_lead(uint8_t(w >> 8)) &&
                is_sjis_trail(uint8_t(w))
            );
        }
        inline uint16_t sjis2jis(uint8_t c0, uint8_t c1) {
            c0 <<= 1;
            if (c1 < 0x9F) {
                if (c0 < 0x3F) {
                    c0 += 0x1F;
                } else {
                    c0 -= 0x61;
                }
                if (c1 > 0x7E) {
                    c1 -= 0x20;
                } else {
                    c1 -= 0x1F;
                }
            } else {
                if (c0 < 0x3F) {
                    c0 += 0x20;
                } else {
                    c0 -= 0x60;
                }
                c1 -= 0x7E;
            }
            uint16_t jis_code = ((uint16_t(c0) << 8) | c1);
            return jis_code;
        }
        inline uint16_t sjis2jis(uint16_t w) {
            uint8_t ch0 = uint8_t(w >> 8);
            uint8_t ch1 = uint8_t(w);
            return sjis2jis(ch0, ch1);
        }
        inline bool is_hankaku_kana(uint8_t b) {
            return ((0xA1 <= b) && (b <= 0xDF));
        }
        inline bool is_ki_code(uint16_t w) {
            return w == 0x1B4B;
        }
        inline bool is_ko_code(uint16_t w) {
            return w == 0x1B48;
        }
    } // namespace japanese

    ///////////////////////////////////////////////////////////////////////////
    // XbmImage

    class XbmImage
    {
    public:
        typedef uint8_t value_type;

        XbmImage(coord_t width, coord_t height, const void *data = NULL,
                 void *alloc = NULL) : m_data(NULL), m_alloc(NULL)
        {
            reset(width, height, data, alloc);
        }
        XbmImage(const XbmImage& image) : m_data(NULL), m_alloc(NULL)
        {
            *this = image;
        }
        XbmImage& operator=(const XbmImage& image);
        virtual ~XbmImage()
        {
            clear();
        }

        void clear()
        {
            delete[] m_alloc;
            m_alloc = NULL;
        }

        void reset(coord_t width, coord_t height,
                   const void *data = NULL, void *alloc = NULL);

        value_type *cast(const void *data) const
        {
            return reinterpret_cast<value_type *>(const_cast<void *>(data));
        }

        coord_t width() const
        {
            return m_width;
        }
        coord_t height() const
        {
            return m_height;
        }
        coord_t stride() const
        {
            return m_stride;
        }
        coord_t size() const
        {
            return stride() * height();
        }
        value_type *data()
        {
            return m_data;
        }
        const value_type *data() const
        {
            return m_data;
        }

        bool in_range(coord_t x, coord_t y) const;

        bool get_dot(coord_t x, coord_t y) const;
        void put_dot(coord_t x, coord_t y, bool dot);

        std::string to_str(const char *name) const;

        void get_subimage(TextCanvas& image, coord_t x0, coord_t y0, coord_t x1, coord_t y1) const;
        void get_subimage(XbmImage& image, coord_t x0, coord_t y0, coord_t x1, coord_t y1) const;
        void put_subimage(coord_t x0, coord_t y0, const TextCanvas& image);
        void put_subimage(coord_t x0, coord_t y0, const XbmImage& image);

    protected:
        value_type *m_data;
        value_type *m_alloc;
        coord_t     m_width;
        coord_t     m_height;
        coord_t     m_stride;
    };

    ///////////////////////////////////////////////////////////////////////////
    // XbmFont

    class XbmFont : public XbmImage
    {
    public:
        XbmFont(coord_t width, coord_t height, const void *data,
                coord_t columns, coord_t rows,
                coord_t cell_width, coord_t cell_height)
            : XbmImage(width, height, data),
              m_columns(columns), m_rows(rows),
              m_cell_width(cell_width), m_cell_height(cell_height)
        {
        }

        coord_t columns() const
        {
            return m_columns;
        }
        coord_t rows() const
        {
            return m_rows;
        }
        coord_t cell_width() const
        {
            return m_cell_width;
        }
        coord_t cell_height() const
        {
            return m_cell_height;
        }

    protected:
        coord_t m_columns;
        coord_t m_rows;
        coord_t m_cell_width;
        coord_t m_cell_height;
    };

    inline const XbmFont& qbasic8x16_font()
    {
        // QBasic 8x16 bitmap font
        #include "bitmaps/qbasic8x16_chars.xbm"
        static const XbmFont qbasic8x16_chars(
            qbasic8x16_chars_width, qbasic8x16_chars_height,
            qbasic8x16_chars_bits, 16, 16, 8, 16);
        return qbasic8x16_chars;
    }

    #ifndef TEXTCANVAS_NO_N88_FONTS
        inline const XbmFont& n88_normal_font()
        {
            // PC-8801 bitmap font
            #include "bitmaps/n88_normal_chars.xbm"
            static const XbmFont n88_normal_chars(
                n88_normal_chars_width, n88_normal_chars_height,
                n88_normal_chars_bits, 256, 1, 8, 16);
            return n88_normal_chars;
        }
        inline const XbmFont& n88_quality_font()
        {
            // PC-9801 bitmap font
            #include "bitmaps/n88_quality_chars.xbm"
            static const XbmFont n88_quality_chars(
                n88_quality_chars_width, n88_quality_chars_height,
                n88_quality_chars_bits, 256, 1, 8, 16);
            return n88_quality_chars;
        }
        inline const XbmFont& n88_graph_font()
        {
            // PC-8801 graphical bitmap font
            #include "bitmaps/n88_graph_chars.xbm"
            static const XbmFont n88_graph_chars(
                n88_graph_chars_width, n88_graph_chars_height,
                n88_graph_chars_bits, 256, 1, 8, 16);
            return n88_graph_chars;
        }
        inline const XbmFont& n88_quarter_font()
        {
            // PC-9801 quarter bitmap font
            #include "bitmaps/n88_quarter_chars.xbm"
            static const XbmFont n88_quarter_chars(
                n88_quarter_chars_width, n88_quarter_chars_height,
                n88_quarter_chars_bits, 256, 1, 8, 8);
            return n88_quarter_chars;
        }
    #endif

    ///////////////////////////////////////////////////////////////////////////
    // TextCanvas

    class TextCanvas
    {
    public:
        TextCanvas();
        TextCanvas(coord_t width, coord_t height,
                   color_t fore_color = asterisk, color_t back_color = space);
        TextCanvas(const XbmImage& binary,
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
        void resize(coord_t width_, coord_t height_);
        void resize(coord_t width_, coord_t height_, color_t ch);

        color_t get_pixel(coord_t x, coord_t y) const;
        color_t get_pixel(const Point& p) const;
        void put_pixel(coord_t x, coord_t y, color_t ch);
        void put_pixel(const Point& p, color_t ch);
        void put_pixel(coord_t x, coord_t y);
        void put_pixel(const Point& p);

        bool get_dot(coord_t x, coord_t y) const;
        void put_dot(coord_t x, coord_t y, bool dot);

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
        void get_subimage(TextCanvas& image, Point& p0, Point& p1) const;
        void get_subimage(XbmImage& image, coord_t x0, coord_t y0, coord_t x1, coord_t y1) const;
        void get_subimage(XbmImage& image, Point& p0, Point& p1) const;

        void put_subimage(coord_t x0, coord_t y0, const TextCanvas& image);
        void put_subimage(coord_t x0, coord_t y0, const XbmImage& image);
        template <typename T_PUTTER0, typename T_PUTTER1>
        void put_subimage(coord_t x0, coord_t y0, const TextCanvas& image, T_PUTTER0& fore, T_PUTTER1& back);
        template <typename T_PUTTER0, typename T_PUTTER1>
        void put_subimage(coord_t x0, coord_t y0, const XbmImage& image, T_PUTTER0& fore, T_PUTTER1& back);

        Size put_char(coord_t x0, coord_t y0, const XbmFont& font, coord_t char_code);
        Size put_char(coord_t x0, coord_t y0, const XbmFont& font, coord_t iColumn, coord_t iRow);

        template <typename T_CONVERTER>
        Size put_char(coord_t x0, coord_t y0, const XbmFont& font, coord_t char_code, T_CONVERTER& conv);
        template <typename T_CONVERTER>
        Size put_char(coord_t x0, coord_t y0, const XbmFont& font, coord_t iColumn, coord_t iRow, T_CONVERTER& conv);
        template <typename T_CONVERTER, typename T_PUTTER0, typename T_PUTTER1>
        Size put_char(coord_t x0, coord_t y0, const XbmFont& font, coord_t char_code,
                      T_CONVERTER& conv, T_PUTTER0& fore, T_PUTTER1& back);
        template <typename T_CONVERTER, typename T_PUTTER0, typename T_PUTTER1>
        Size put_char(coord_t x0, coord_t y0, const XbmFont& font, coord_t iColumn, coord_t iRow,
                      T_CONVERTER& conv, T_PUTTER0& fore, T_PUTTER1& back);

        // draw text rightward
        void text_to_right(coord_t x0, coord_t y0, const XbmFont& font, const string_type& text);
        void text_to_right(const Point& p0, const XbmFont& font, const string_type& text);
        template <typename T_CONVERTER>
        void text_to_right(coord_t x0, coord_t y0, const XbmFont& font, const string_type& text, T_CONVERTER& conv);
        template <typename T_CONVERTER>
        void text_to_right(const Point& p0, const XbmFont& font, const string_type& text, T_CONVERTER& conv);

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

        void swap(TextCanvas& other);

        void rotate_left(const TextCanvas& other);
        void rotate_right(const TextCanvas& other);
        void rotate_180(const TextCanvas& other);

        void rotate_left();
        void rotate_right();
        void rotate_180();

        void scale(const TextCanvas& other, coord_t width, coord_t height);
        void scale(coord_t width, coord_t height);

        void scale_cross(const TextCanvas& other, coord_t x_multi, coord_t y_multi);
        void scale_cross(coord_t x_multi, coord_t y_multi);

        void scale_percent(const TextCanvas& other, coord_t x_percent, coord_t y_percent);
        void scale_percent(coord_t x_percent, coord_t y_percent);

        void mirror_h(const TextCanvas& other);
        void mirror_v(const TextCanvas& other);

        void mirror_h();
        void mirror_v();
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
    // image converters

    struct NoneConverter
    {
        void operator()(TextCanvas& tc)
        {
        }
    };
    struct YokoNibaiKaku
    {
        void operator()(TextCanvas& tc)
        {
            tc.scale_cross(2, 1);
        }
    };
    struct TateNibaiKaku
    {
        void operator()(TextCanvas& tc)
        {
            tc.scale_cross(1, 2);
        }
    };
    struct YonBaiKaku
    {
        void operator()(TextCanvas& tc)
        {
            tc.scale_cross(2, 2);
        }
    };
    struct LeftRotator
    {
        void operator()(TextCanvas& tc)
        {
            tc.rotate_left();
        }
    };
    struct RightRotator
    {
        void operator()(TextCanvas& tc)
        {
            tc.rotate_right();
        }
    };
    template <typename T_LEFT_CONV, typename T_RIGHT_CONV>
    struct PairConverter
    {
        T_LEFT_CONV& m_left;
        T_RIGHT_CONV& m_right;
        PairConverter(T_LEFT_CONV& left, T_RIGHT_CONV& right)
            : m_left(left), m_right(right)
        {
        }
        void operator()(TextCanvas& tc)
        {
            m_left(tc);
            m_right(tc);
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

    ///////////////////////////////////////////////////////////////////////////

    bool XbmImage::in_range(coord_t x, coord_t y) const
    {
        return 0 <= x && x < width() && 0 <= y && y < height();
    }

    inline bool XbmImage::get_dot(coord_t x, coord_t y) const
    {
        if (!in_range(x, y))
            return false;

        value_type byte = data()[stride() * y + x / 8];
        return (byte & (1 << (x & 7))) != 0;
    }
    inline void XbmImage::put_dot(coord_t x, coord_t y, bool dot)
    {
        if (!in_range(x, y))
            return;

        value_type byte = data()[stride() * y + x / 8];
        coord_t shift = x & 7;
        if (dot)
            byte |= (1 << shift);
        else
            byte &= ~(1 << shift);
        data()[stride() * y + x / 8] = byte;
    }

    inline XbmImage& XbmImage::operator=(const XbmImage& image)
    {
        if (this != &image)
        {
            reset(image.width(), image.height(), NULL, image.m_alloc);
        }
        return *this;
    }

    inline void
    XbmImage::reset(coord_t width, coord_t height, const void *data, void *alloc)
    {
        assert(width);
        assert(height);
        assert(sizeof(value_type) == 1);

        clear();

        m_width = width;
        m_height = height;
        m_stride = (width + 7) / 8;
        coord_t total_size = m_stride * m_height;
        if (total_size == 0)
        {
            m_data = m_alloc = NULL;
        }
        else if (!data && !alloc)
        {
            m_data = m_alloc = new value_type[total_size];
            memset(m_data, 0, total_size);
        }
        else if (!data && alloc)
        {
            m_data = m_alloc = new value_type[total_size];
            memcpy(m_data, alloc, total_size);
        }
        else if (data && !alloc)
        {
            m_data = cast(data);
            m_alloc = NULL;
        }
        else if (data && alloc)
        {
            m_data = cast(data);
            m_alloc = cast(alloc);
        }
    }

    inline std::string XbmImage::to_str(const char *name) const
    {
        std::string str;
        char buf[32];

        str += "#define ";
        str += name;
        str += "_width ";
        sprintf(buf, "%u", (int)width());
        str += buf;
        str += "\n";

        str += "#define ";
        str += name;
        str += "_height ";
        sprintf(buf, "%u", (int)height());
        str += buf;
        str += "\n";

        str += "static const unsigned char ";
        str += name;
        str += "_bits[] = {\n";

        size_t i, k = 0;
        for (i = 0; i < stride() * height() - 1; ++i)
        {
            if (k == 0)
                str += "   ";
            sprintf(buf, "0x%02x, ", data()[i]);
            str += buf;
            ++k;
            if (k == 12)
            {
                k = 0;
                str += "\n";
            }
        }
        if (k == 0)
            str += "  ";
        sprintf(buf, "0x%02x };\n", data()[i]);
        str += buf;
        return str;
    }

    inline void XbmImage::get_subimage(TextCanvas& image, coord_t x0, coord_t y0, coord_t x1, coord_t y1) const
    {
        if (x0 > x1)
            std::swap(x0, x1);
        if (y0 > y1)
            std::swap(y0, y1);

        coord_t width = x1 - x0 + 1;
        coord_t height = y1 - y0 + 1;
        if (width != image.width() || height != image.height())
        {
            image.reset(width, height);
        }

        coord_t py = 0;
        for (coord_t y = y0; y <= y1; ++y)
        {
            coord_t px = 0;
            for (coord_t x = x0; x <= x1; ++x)
            {
                image.put_dot(px, py, get_dot(x, y));
                ++px;
            }
            ++py;
        }
    }
    inline void XbmImage::get_subimage(XbmImage& image, coord_t x0, coord_t y0, coord_t x1, coord_t y1) const
    {
        if (x0 > x1)
            std::swap(x0, x1);
        if (y0 > y1)
            std::swap(y0, y1);

        coord_t width = x1 - x0 + 1;
        coord_t height = y1 - y0 + 1;
        if (width != image.width() || height != image.height())
        {
            image.reset(width, height);
        }

        coord_t py = 0;
        for (coord_t y = y0; y <= y1; ++y)
        {
            coord_t px = 0;
            for (coord_t x = x0; x <= x1; ++x)
            {
                image.put_dot(px, py, get_dot(x, y));
                ++px;
            }
            ++py;
        }
    }
    inline void XbmImage::put_subimage(coord_t x0, coord_t y0, const TextCanvas& image)
    {
        for (coord_t y = 0; y < image.height(); ++y)
        {
            for (coord_t x = 0; x < image.width(); ++x)
            {
                put_dot(x0 + x, y0 + y, image.get_dot(x, y));
            }
        }
    }
    inline void XbmImage::put_subimage(coord_t x0, coord_t y0, const XbmImage& image)
    {
        for (coord_t y = 0; y < image.height(); ++y)
        {
            for (coord_t x = 0; x < image.width(); ++x)
            {
                put_dot(x0 + x, y0 + y, image.get_dot(x, y));
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // internal methods

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
    inline TextCanvas::TextCanvas(const XbmImage& binary,
                                  color_t fore_color, color_t back_color)
        : m_width(binary.width()), m_height(binary.height()),
          m_text(binary.width() * binary.height(), 0),
          m_fore_color(fore_color), m_back_color(back_color), m_pos(0, 0)
    {
        for (coord_t y = 0; y < height(); ++y)
        {
            for (coord_t x = 0; x < width(); ++x)
            {
                if (binary.get_dot(x, y))
                    put_pixel(x, y, fore_color);
                else
                    put_pixel(x, y, back_color);
            }
        }
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
        return data()[index];
    }
    inline const color_t& TextCanvas::operator[](size_t index) const
    {
        return data()[index];
    }
    inline string_type TextCanvas::to_str() const
    {
        string_type ret;
        for (coord_t y = 0; y < height(); ++y)
        {
            ret += data().substr(y * width(), width());
            ret += newline;
        }
        return ret;
    }

    inline string_type& TextCanvas::data()
    {
        return m_text;
    }
    inline const string_type& TextCanvas::data() const
    {
        return m_text;
    }
    inline void TextCanvas::clear(color_t ch)
    {
        m_text.assign(width() * height(), ch);
    }

    inline void TextCanvas::reset(coord_t width, coord_t height, color_t ch)
    {
        m_width = width;
        m_height = height;
        m_text.assign(width * height, ch);
        m_pos.x = m_pos.y = 0;
    }

    inline void TextCanvas::resize(coord_t width_, coord_t height_, color_t ch)
    {
        const coord_t min_width = std::min(width(), width_);
        const coord_t min_height = std::min(height(), height_);
        string_type str(width_ * height_, ch);
        for (coord_t y = 0; y < min_height; ++y)
        {
            for (coord_t x = 0; x < min_width; ++x)
            {
                str[y * width_ + x] = m_text[y * width() + x];
            }
        }
        std::swap(str, m_text);
        m_width = width_;
        m_height = height_;
    }

    inline color_t TextCanvas::get_pixel(coord_t x, coord_t y) const
    {
        if (in_range(x, y))
        {
            return m_text[y * width() + x];
        }
        return m_back_color;
    }
    inline color_t TextCanvas::get_pixel(const Point& p) const
    {
        return get_pixel(p.x, p.y);
    }
    inline void TextCanvas::put_pixel(coord_t x, coord_t y, color_t ch)
    {
        if (in_range(x, y))
        {
            m_text[y * width() + x] = ch;
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

    inline void TextCanvas::swap(TextCanvas& other)
    {
        std::swap(m_width, other.m_width);
        std::swap(m_height, other.m_height);
        std::swap(m_text, other.m_text);
        std::swap(m_fore_color, other.m_fore_color);
        std::swap(m_back_color, other.m_back_color);
        std::swap(m_pos, other.m_pos);
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

    inline void TextCanvas::move_to(coord_t x, coord_t y)
    {
        m_pos.x = x;
        m_pos.y = y;
    }
    inline void TextCanvas::move_to(const Point& p)
    {
        m_pos = p;
    }

    ///////////////////////////////////////////////////////////////////////////

    inline bool TextCanvas::get_dot(coord_t x, coord_t y) const
    {
        return get_pixel(x, y) == fore_color();
    }
    inline void TextCanvas::put_dot(coord_t x, coord_t y, bool dot)
    {
        if (dot)
            put_pixel(x, y, fore_color());
        else
            put_pixel(x, y, back_color());
    }

    inline bool TextCanvas::in_range(coord_t x, coord_t y) const
    {
        return (0 <= x && x < width() && 0 <= y && y < height());
    }
    inline bool TextCanvas::in_range(const Point& p) const
    {
        return in_range(p.x, p.y);
    }

    inline bool TextCanvas::same_size(const TextCanvas& tc) const
    {
        return width() == tc.width() && height() == tc.height();
    }

    inline void TextCanvas::clear()
    {
        clear(back_color());
    }
    inline void TextCanvas::reset(coord_t width, coord_t height)
    {
        reset(width, height, back_color());
    }

    inline void TextCanvas::resize(coord_t width_, coord_t height_)
    {
        resize(width_, height_, back_color());
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
                data()[i] = fore_color();
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
                data()[i] = image[i];
            }
        }
    }

    inline void TextCanvas::get_subimage(TextCanvas& image, coord_t x0, coord_t y0, coord_t x1, coord_t y1) const
    {
        if (x0 > x1)
            std::swap(x0, x1);
        if (y0 > y1)
            std::swap(y0, y1);

        coord_t width = x1 - x0 + 1;
        coord_t height = y1 - y0 + 1;
        if (width != image.width() || height != image.height())
        {
            image.reset(width, height);
        }

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
    inline void TextCanvas::get_subimage(XbmImage& image, coord_t x0, coord_t y0, coord_t x1, coord_t y1) const
    {
        if (x0 > x1)
            std::swap(x0, x1);
        if (y0 > y1)
            std::swap(y0, y1);

        coord_t width = x1 - x0 + 1;
        coord_t height = y1 - y0 + 1;
        if (image.width() != width || image.height() != height)
        {
            image.reset(width, height);
        }

        coord_t py = 0;
        for (coord_t y = y0; y <= y1; ++y)
        {
            coord_t px = 0;
            for (coord_t x = x0; x <= x1; ++x)
            {
                image.put_dot(px, py, get_dot(x, y));
                ++px;
            }
            ++py;
        }
    }

    inline void TextCanvas::get_subimage(TextCanvas& image, Point& p0, Point& p1) const
    {
        get_subimage(image, p0.x, p0.y, p1.x, p1.y);
    }
    inline void TextCanvas::get_subimage(XbmImage& image, Point& p0, Point& p1) const
    {
        get_subimage(image, p0.x, p0.y, p1.x, p1.y);
    }

    inline void TextCanvas::put_subimage(coord_t x0, coord_t y0, const TextCanvas& image)
    {
        for (coord_t py = 0; py < image.height(); ++py)
        {
            for (coord_t px = 0; px < image.width(); ++px)
            {
                put_pixel(x0 + px, y0 + py, image.get_pixel(px, py));
            }
        }
    }
    inline void TextCanvas::put_subimage(coord_t x0, coord_t y0, const XbmImage& image)
    {
        for (coord_t py = 0; py < image.height(); ++py)
        {
            for (coord_t px = 0; px < image.width(); ++px)
            {
                put_dot(x0 + px, y0 + py, image.get_dot(px, py));
            }
        }
    }

    template <typename T_PUTTER0, typename T_PUTTER1>
    inline void TextCanvas::put_subimage(coord_t x0, coord_t y0, const TextCanvas& image, T_PUTTER0& fore, T_PUTTER1& back)
    {
        for (coord_t py = 0; py < image.height(); ++py)
        {
            for (coord_t px = 0; px < image.width(); ++px)
            {
                if (image.get_dot(px, py))
                    fore(x0 + px, y0 + py);
                else
                    back(x0 + px, y0 + py);
            }
        }
    }
    template <typename T_PUTTER0, typename T_PUTTER1>
    inline void TextCanvas::put_subimage(coord_t x0, coord_t y0, const XbmImage& image, T_PUTTER0& fore, T_PUTTER1& back)
    {
        for (coord_t py = 0; py < image.height(); ++py)
        {
            for (coord_t px = 0; px < image.width(); ++px)
            {
                if (image.get_dot(px, py))
                    fore(x0 + px, y0 + py);
                else
                    back(x0 + px, y0 + py);
            }
        }
    }

    template <typename T_CONVERTER, typename T_PUTTER0, typename T_PUTTER1>
    inline Size
    TextCanvas::put_char(coord_t x0, coord_t y0, const XbmFont& font, coord_t iColumn, coord_t iRow,
                         T_CONVERTER& conv, T_PUTTER0& fore, T_PUTTER1& back)
    {
        assert(0 <= iColumn && iColumn < font.columns());
        assert(0 <= iRow && iRow < font.rows());
        coord_t qx0 = iColumn * font.cell_width();
        coord_t qy0 = iRow * font.cell_height();
        coord_t qx1 = qx0 + font.cell_width() - 1;
        coord_t qy1 = qy0 + font.cell_height() - 1;
        TextCanvas other;
        font.get_subimage(other, qx0, qy0, qx1, qy1);
        conv(other);
        put_subimage(x0, y0, other, fore, back);
        return Size(other.width(), other.height());
    }
    template <typename T_CONVERTER, typename T_PUTTER0, typename T_PUTTER1>
    inline Size
    TextCanvas::put_char(coord_t x0, coord_t y0, const XbmFont& font, coord_t char_code,
                         T_CONVERTER& conv, T_PUTTER0& fore, T_PUTTER1& back)
    {
        coord_t iColumn = coord_t(char_code % font.columns());
        coord_t iRow = coord_t(char_code / font.columns());
        return put_char(x0, y0, font, iColumn, iRow, conv, fore, back);
    }
    template <typename T_CONVERTER>
    inline Size TextCanvas::put_char(coord_t x0, coord_t y0, const XbmFont& font, coord_t iColumn, coord_t iRow, T_CONVERTER& conv)
    {
        ColorPutter fore(*this, fore_color());
        ColorPutter back(*this, back_color());
        return put_char(x0, y0, font, iColumn, iRow, conv, fore, back);
    }
    template <typename T_CONVERTER>
    inline Size TextCanvas::put_char(coord_t x0, coord_t y0, const XbmFont& font, coord_t char_code, T_CONVERTER& conv)
    {
        coord_t iColumn = coord_t(char_code % font.columns());
        coord_t iRow = coord_t(char_code / font.columns());
        return put_char(x0, y0, font, iColumn, iRow, conv);
    }
    inline Size TextCanvas::put_char(coord_t x0, coord_t y0, const XbmFont& font, coord_t iColumn, coord_t iRow)
    {
        NoneConverter conv;
        return put_char(x0, y0, font, iColumn, iRow, conv);
    }
    inline Size TextCanvas::put_char(coord_t x0, coord_t y0, const XbmFont& font, coord_t char_code)
    {
        NoneConverter conv;
        return put_char(x0, y0, font, char_code, conv);
    }

    template <typename T_CONVERTER>
    inline void TextCanvas::text_to_right(coord_t x0, coord_t y0, const XbmFont& font, const string_type& text, T_CONVERTER& conv)
    {
        for (size_t i = 0; i < text.size(); ++i)
        {
            x0 += put_char(x0, y0, font, text[i], conv).x;
        }
    }
    template <typename T_CONVERTER>
    inline void TextCanvas::text_to_right(const Point& p0, const XbmFont& font, const string_type& text, T_CONVERTER& conv)
    {
        text_to_right(p0.x, p0.y, font, text, conv);
    }
    inline void TextCanvas::text_to_right(coord_t x0, coord_t y0, const XbmFont& font, const string_type& text)
    {
        for (size_t i = 0; i < text.size(); ++i)
        {
            x0 += put_char(x0, y0, font, text[i]).x;
        }
    }
    inline void TextCanvas::text_to_right(const Point& p0, const XbmFont& font, const string_type& text)
    {
        text_to_right(p0.x, p0.y, font, text);
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
        Point p(x, y);
        if (!in_range(p))
            return;

        Points points;
        points.push_back(p);

        std::set<Point, point_less> point_set;
        point_set.insert(p);
        for (size_t i = 0; i < points.size(); ++i)
        {
            p = points[i];
            if (get_pixel(p) == border_ch)
                continue;

            put_pixel(p);

            --p.x;  // left
            if (get_pixel(p) != border_ch && !point_set.count(p))
            {
                points.push_back(p);
                point_set.insert(p);
            }
            ++p.x; --p.y;  // up
            if (get_pixel(p) != border_ch && !point_set.count(p))
            {
                points.push_back(p);
                point_set.insert(p);
            }
            ++p.x; ++p.y;  // right
            if (get_pixel(p) != border_ch && !point_set.count(p))
            {
                points.push_back(p);
                point_set.insert(p);
            }
            --p.x; ++p.y;  // down
            if (get_pixel(p) != border_ch && !point_set.count(p))
            {
                points.push_back(p);
                point_set.insert(p);
            }
        }

        move_to(x, y);
    }
    inline void TextCanvas::flood_fill_surface(coord_t x, coord_t y, color_t surface_ch)
    {
        Point p(x, y);
        if (!in_range(p))
            return;

        Points points;
        points.push_back(p);
        for (size_t i = 0; i < points.size(); ++i)
        {
            p = points[i];
            if (get_pixel(p) != surface_ch)
                continue;

            put_pixel(p);

            --p.x;  // left
            if (get_pixel(p) == surface_ch)
                points.push_back(p);
            ++p.x; --p.y;  // up
            if (get_pixel(p) == surface_ch)
                points.push_back(p);
            ++p.x; ++p.y;  // right
            if (get_pixel(p) == surface_ch)
                points.push_back(p);
            --p.x; ++p.y;  // down
            if (get_pixel(p) == surface_ch)
                points.push_back(p);
        }

        move_to(x, y);
    }
    inline void TextCanvas::flood_fill_bordered(const Point& p, color_t border_ch)
    {
        flood_fill_bordered(p.x, p.y, border_ch);
    }
    inline void TextCanvas::flood_fill_surface(const Point& p, color_t surface_ch)
    {
        flood_fill_surface(p.x, p.y, surface_ch);
    }

    inline void TextCanvas::line_to(coord_t x, coord_t y)
    {
        line(pos().x, pos().y, x, y);
    }
    inline void TextCanvas::line_to(const Point& p)
    {
        line(pos(), p);
    }
    template <typename T_PUTTER>
    inline void TextCanvas::line_to(coord_t x, coord_t y, T_PUTTER& putter)
    {
        line(pos().x, pos().y, x, y, putter);
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

        move_to(x1, y1);
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

        move_to(x1, y1);
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
        if (x1 > width())
            x1 = width() - 1;
        if (y0 < 0)
            y0 = 0;
        if (y1 > height())
            y1 = height() - 1;
        for (coord_t y = y0; y <= y1; ++y)
        {
            for (coord_t x = x0; x <= x1; ++x)
            {
                putter(x, y);
            }
        }

        move_to(x2, y2);
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

        move_to(x0, y0);
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

        move_to(x0, y0);
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

            move_to(do_round(px), do_round(py));
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

        move_to(do_round(px), do_round(py));
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

            move_to(do_round(px), do_round(py));
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

        move_to(do_round(px), do_round(py));
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

    inline void TextCanvas::rotate_left(const TextCanvas& other)
    {
        reset(other.height(), other.width());
        for (coord_t y = 0; y < other.width(); ++y)
        {
            for (coord_t x = 0; x < other.height(); ++x)
            {
                color_t ch = other.get_pixel(other.width() - (y + 1), x);
                put_pixel(x, y, ch);
            }
        }
    }
    inline void TextCanvas::rotate_right(const TextCanvas& other)
    {
        reset(other.height(), other.width());
        for (coord_t y = 0; y < other.width(); ++y)
        {
            for (coord_t x = 0; x < other.height(); ++x)
            {
                color_t ch = other.get_pixel(y, other.height() - (x + 1));
                put_pixel(x, y, ch);
            }
        }
    }
    inline void TextCanvas::rotate_180(const TextCanvas& other)
    {
        reset(other.width(), other.height());
        for (coord_t y = 0; y < other.height(); ++y)
        {
            for (coord_t x = 0; x < other.width(); ++x)
            {
                color_t ch = other.get_pixel(other.width() - (x + 1), other.height() - (y + 1));
                put_pixel(x, y, ch);
            }
        }
    }

    inline void TextCanvas::rotate_left()
    {
        TextCanvas other;
        other.rotate_left(*this);
        swap(other);
    }
    inline void TextCanvas::rotate_right()
    {
        TextCanvas other;
        other.rotate_right(*this);
        swap(other);
    }
    inline void TextCanvas::rotate_180()
    {
        TextCanvas other;
        other.rotate_180(*this);
        swap(other);
    }

    inline void TextCanvas::mirror_h(const TextCanvas& other)
    {
        reset(other.width(), other.height());
        for (coord_t y = 0; y < other.height(); ++y)
        {
            for (coord_t x = 0; x < other.width(); ++x)
            {
                color_t ch = other.get_pixel(other.width() - (x + 1), y);
                put_pixel(x, y, ch);
            }
        }
    }
    inline void TextCanvas::mirror_v(const TextCanvas& other)
    {
        reset(other.width(), other.height());
        for (coord_t y = 0; y < other.height(); ++y)
        {
            for (coord_t x = 0; x < other.width(); ++x)
            {
                color_t ch = other.get_pixel(x, other.height() - (y + 1));
                put_pixel(x, y, ch);
            }
        }
    }

    inline void TextCanvas::mirror_h()
    {
        TextCanvas other;
        other.mirror_h(*this);
        swap(other);
    }
    inline void TextCanvas::mirror_v()
    {
        TextCanvas other;
        other.mirror_v(*this);
        swap(other);
    }

    inline void TextCanvas::scale(const TextCanvas& other, coord_t width, coord_t height)
    {
        reset(width, height);

        for (coord_t py = 0; py < height; ++py)
        {
            for (coord_t px = 0; px < width; ++px)
            {
                coord_t qx = px * other.width() / width;
                coord_t qy = py * other.height() / height;
                color_t ch = other.get_pixel(qx, qy);
                put_pixel(px, py, ch);
            }
        }
    }
    inline void TextCanvas::scale_cross(const TextCanvas& other, coord_t x_multi, coord_t y_multi)
    {
        coord_t new_width = other.width() * x_multi;
        coord_t new_height = other.height() * y_multi;
        reset(new_width, new_height);

        for (coord_t qy = 0; qy < other.height(); ++qy)
        {
            for (coord_t qx = 0; qx < other.width(); ++qx)
            {
                color_t ch = other.get_pixel(qx, qy);
                for (coord_t y = 0; y < y_multi; ++y)
                {
                    for (coord_t x = 0; x < x_multi; ++x)
                    {
                        put_pixel(qx * x_multi + x, qy * y_multi + y, ch);
                    }
                }
            }
        }
    }
    inline void TextCanvas::scale_percent(const TextCanvas& other, coord_t x_percent, coord_t y_percent)
    {
        coord_t new_width = other.width() * x_percent / 100;
        coord_t new_height = other.height() * y_percent / 100;
        reset(new_width, new_height);

        for (coord_t py = 0; py < new_height; ++py)
        {
            for (coord_t px = 0; px < new_width; ++px)
            {
                coord_t qx = px * other.width() / new_width;
                coord_t qy = py * other.height() / new_height;
                color_t ch = other.get_pixel(qx, qy);
                put_pixel(px, py, ch);
            }
        }
    }

    inline void TextCanvas::scale(coord_t width, coord_t height)
    {
        TextCanvas other;
        other.scale(*this, width, height);
        swap(other);
    }
    inline void TextCanvas::scale_cross(coord_t x_multi, coord_t y_multi)
    {
        TextCanvas other;
        other.scale_cross(*this, x_multi, y_multi);
        swap(other);
    }
    inline void TextCanvas::scale_percent(coord_t x_percent, coord_t y_percent)
    {
        TextCanvas other;
        other.scale_percent(*this, x_percent, y_percent);
        swap(other);
    }

} // namespace textcanvas

namespace std
{
    template <>
    inline void swap(textcanvas::TextCanvas& left, textcanvas::TextCanvas& right)
    {
        left.swap(right);
    }
} // namespace std

///////////////////////////////////////////////////////////////////////////

#endif  // ndef TEXT_CANVAS_HPP_
