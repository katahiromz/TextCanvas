#include "TextCanvas.hpp"
#include <iostream>

int main(void)
{
    using namespace textcanvas;
    TextCanvas canvas(8, 16);
    canvas.put_text(0, 0, n88_quality_font(), "P");
    canvas.rotate_left();
    std::cout << canvas.to_str();
    canvas.rotate_right();
    std::cout << canvas.to_str();
    canvas.rotate_180();
    std::cout << canvas.to_str();
    return 0;
}
