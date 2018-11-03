#include "TextCanvas.hpp"
#include <iostream>

int main(void)
{
    using namespace textcanvas;
    TextCanvas canvas(100, 100);
    canvas.rectangle(3, 3, 100 - 3, 100- 3);
    canvas.flood_fill_bordered(100 / 2, 100 / 2, sharp);
    std::cout << canvas.to_str();
    return 0;
}
