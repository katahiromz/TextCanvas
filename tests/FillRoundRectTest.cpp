#include "TextCanvas.hpp"
#include <iostream>

int main(void)
{
    textcanvas::TextCanvas canvas(100 + 1, 100 + 1);
    textcanvas::coord_t k = 0;
    for (textcanvas::coord_t i = 0; i < 50; i += 3)
    {
        canvas.pixel('0' + k % 10);
        canvas.fill_round_rect(i, i, 100 - i, 100 - i, 10, 10);
        ++k;
    }
    std::cout << canvas.to_str();
    return 0;
}
