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
            canvas.pie(i * 13, k * 13, i * 13 + i, k * 13 + k, 3.14 / 4, 3.14 * 3 / 2);
        }
    }
    std::cout << canvas.to_str();
    return 0;
}
