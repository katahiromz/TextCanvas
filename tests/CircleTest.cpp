#include "TextCanvas.hpp"
#include <iostream>

int main(void)
{
    textcanvas::TextCanvas canvas(100, 100);
    for (int i = 0; i < 100 / 2; ++i)
    {
        canvas.fore_color('0' + i % 10);
        canvas.circle(100 / 2, 100 / 2, i);
    }
    std::cout << canvas.to_str();
    return 0;
}
