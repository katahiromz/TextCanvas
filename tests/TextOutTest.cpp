#include "TextCanvas.hpp"
#include <iostream>

int main(void)
{
    using namespace textcanvas;
    TextCanvas canvas(8 * 9, 16 * 2, '#', ' ');
    canvas.text_to_right(0, 0, kh_dot_hankaku_font(), "ABCabc123");
    //canvas.jis_put_char(0, 16, kh_dot_zenkaku_font(), japanese::sjis2jis(0x93FA));
    canvas.sjis_text_to_right(0, 16, kh_dot_zenkaku_font(), kh_dot_hankaku_font(), "\x93\xFA\x96\x7B\x8C\xEA\xB1\xB2" "AB"); // Shift_JIS: “ú–{Œê±²AB
    std::cout << canvas.to_str();
    return 0;
}
