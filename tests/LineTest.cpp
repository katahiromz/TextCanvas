#include "TextCanvas.hpp"
#include <iostream>

int main(void)
{
    textcanvas::TextCanvas canvas(100 + 1, 100 + 1);
    size_t k = 0;
    for (textcanvas::coord_t i = 0; i <= 100; i += 5)
    {
        canvas.color('0' + k % 10);
        canvas.line(i, 100, 100, (100 - i));
        canvas.line(0, (100 - i), i, 0);
        ++k;
    }
    std::cout << canvas.to_str();
    return 0;
}
