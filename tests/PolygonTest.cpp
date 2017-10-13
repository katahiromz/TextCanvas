#include "TextCanvas.hpp"
#include <iostream>

int main(void)
{
    textcanvas::TextCanvas canvas(100 + 1, 100 + 1);
    textcanvas::Point points[5];
    for (size_t n = 0; n < 5; ++n)
    {
        points[n].x = 100 / 2 + 100 / 2 * cos(n * 72 * 2 * 3.14 / 180);
        points[n].y = 100 / 2 + 100 / 2 * sin(n * 72 * 2 * 3.14 / 180);
    }
    canvas.pixel('*');
    canvas.circle(100 / 2, 100 / 2, 100 / 2);
    canvas.fill_polygon(5, points, true);
    canvas.pixel('#');
    canvas.polygon(5, points);
    std::cout << canvas.to_str();
    return 0;
}
