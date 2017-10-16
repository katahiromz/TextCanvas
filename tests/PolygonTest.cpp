#include "TextCanvas.hpp"
#include <iostream>

int main(void)
{
    using namespace textcanvas;
    TextCanvas canvas(100 + 1, 100 + 1);
    Point points[5];
    for (coord_t n = 0; n < 5; ++n)
    {
        points[n].x = do_round(100 / 2 + 100 / 2 * cos(n * 72 * 2 * 3.14 / 180));
        points[n].y = do_round(100 / 2 + 100 / 2 * sin(n * 72 * 2 * 3.14 / 180));
    }
    canvas.color('*');
    canvas.circle(100 / 2, 100 / 2, 100 / 2);
    canvas.fill_polygon(5, points, true);
    canvas.color('#');
    canvas.polygon(5, points);
    std::cout << canvas.to_str();
    return 0;
}
