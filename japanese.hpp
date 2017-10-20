// japanese.hpp --- Japanese code related
///////////////////////////////////////////////////////////////////////////

#ifndef JAPANESE_HPP_
#define JAPANESE_HPP_       1   // Version 1

#if __cplusplus >= 201103L
    #include <cstdint>      // uint8_t, uint16_t, ...
#else
    #include "pstdint.h"    // uint8_t, uint16_t, ...
#endif

///////////////////////////////////////////////////////////////////////////

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

#endif  // ndef JAPANESE_HPP_
