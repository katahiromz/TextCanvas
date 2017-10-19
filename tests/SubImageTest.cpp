#include "TextCanvas.hpp"
#include <iostream>

int main(void)
{
    textcanvas::TextCanvas canvas(100, 100), subimage;
    canvas.rectangle(10, 10, 25, 25);
    canvas.line(10, 10, 25, 25);
    canvas.line(25, 10, 10, 25);
    canvas.get_subimage(subimage, 10, 10, 25, 25);
    canvas.put_subimage(35, 35, subimage, 2, 3);
    canvas.put_subimage(50, 50, subimage, 0, 0, 25 - 10, 25 - 10, 1, 1);
    canvas.put_subimage(60, 60, textcanvas::n88_normal_font(), 1, 1);
    std::cout << canvas.to_str();
    return 0;
}
