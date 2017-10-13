#include "TextCanvas.hpp"
#include <iostream>

int main(void)
{
    using namespace textcanvas;
    for (double r = -3 * pi; r <= 3 * pi; r += 1)
    {
        double c0 = std::cos(r);
        double c1 = std::cos(normalize_radian(r));
        if (!(fabs(c0 - c1) < 0.01))
        {
            printf("cos(%f): error\n", r);
            return 1;
        }
        double s0 = std::sin(r);
        double s1 = std::sin(normalize_radian(r));
        if (!(fabs(s0 - s1) < 0.01))
        {
            printf("sin(%f): error\n", r);
            return 2;
        }
    }
    return 0;
}
