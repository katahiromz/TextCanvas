#include "TextCanvas.hpp"
#include <iostream>

int main(void)
{
    textcanvas::TextCanvas canvas(13 * 13, 13 * 13);
    for (int k = 0; k < 13; ++k)
    {
        for (int i = 0; i < 13; ++i)
        {
            canvas.pixel('-');
            canvas.rectangle(i * 13, k * 13, i * 13 + i, k * 13 + k);
            canvas.pixel('#');
            canvas.ellipse(i * 13, k * 13, i * 13 + i, k * 13 + k);
        }
    }
    std::cout << canvas.to_str();
    return 0;
}
