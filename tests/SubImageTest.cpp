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
    std::cout << canvas.to_str();
    return 0;
}
