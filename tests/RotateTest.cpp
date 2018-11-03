#include "TextCanvas.hpp"
#include <iostream>

int main(void)
{
    using namespace textcanvas;
    TextCanvas canvas(8, 16);
    canvas.text_to_right(0, 0, kh_dot_hankaku_font(), "P");
    canvas.rotate_left();
    std::cout << canvas.to_str();
    canvas.rotate_right();
    std::cout << canvas.to_str();

    canvas.rotate_180();
    std::cout << canvas.to_str();
    canvas.rotate_180();

    canvas.mirror_h();
    std::cout << canvas.to_str();
    canvas.mirror_h();

    canvas.mirror_v();
    std::cout << canvas.to_str();
    canvas.mirror_v();

    return 0;
}
