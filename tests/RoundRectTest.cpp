#include "TextCanvas.hpp"
#include <iostream>

int main(void)
{
    using namespace textcanvas;
    TextCanvas canvas(100 + 1, 100 + 1);
    coord_t k = 0;
    for (coord_t i = 0; i < 50; i += 3)
    {
        canvas.color('0' + k % 10);
        canvas.round_rect(i, i, 100 - i, 100 - i, 10, 10);
        ++k;
    }
    std::cout << canvas.to_str();
    return 0;
}
