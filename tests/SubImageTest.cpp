#include "TextCanvas.hpp"
#include <iostream>

int main(void)
{
    textcanvas::TextCanvas canvas(100, 100), subimage, subimage2;
    canvas.rectangle(10, 10, 25, 25);
    canvas.line(10, 10, 25, 25);
    canvas.line(25, 10, 10, 25);
    canvas.get_subimage(subimage, 10, 10, 25, 25);
    subimage2.stretch_cross(subimage, 2, 3);
    canvas.put_subimage(35, 35, subimage2);
    canvas.put_subimage(50, 50, subimage);
    canvas.put_subimage(60, 60, textcanvas::n88_normal_font());
    std::cout << canvas.to_str();
    return 0;
}
