#include "TextCanvas.hpp"
#include <iostream>

int main(void)
{
    using namespace textcanvas;
    TextCanvas canvas(8 * 9, 16);
    canvas.text_to_right(0, 0, kh_dot_hankaku_font(), "ABCabc123");
    std::cout << canvas.to_str();
    return 0;
}
