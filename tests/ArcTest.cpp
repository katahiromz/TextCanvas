#include "TextCanvas.hpp"
#include <iostream>

int main(void)
{
    using namespace textcanvas;
    TextCanvas canvas(13 * 13, 13 * 13);
    for (int k = 0; k < 13; ++k)
    {
        for (int i = 0; i < 13; ++i)
        {
            canvas.fore_color('-');
            canvas.rectangle(i * 13, k * 13, i * 13 + i, k * 13 + k);
            canvas.fore_color('#');
            canvas.arc(i * 13, k * 13, i * 13 + i, k * 13 + k, pi / 4, pi * 3 / 2);
        }
    }
    std::cout << canvas.to_str();
    return 0;
}
