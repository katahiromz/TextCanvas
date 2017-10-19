#include "TextCanvas.hpp"
#include <iostream>

int main(void)
{
    using namespace textcanvas;
    TextCanvas canvas(8, 16);
    canvas.put_text(0, 0, n88_normal_font(), "A");
    canvas.rotate_left();
    std::cout << canvas.to_str();
    canvas.rotate_right();
    std::cout << canvas.to_str();
    return 0;
}
