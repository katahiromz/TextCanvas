#include "TextCanvas.hpp"
#include <iostream>

int main(void)
{
    using namespace textcanvas;
    TextCanvas canvas(8 * 9, 16 * 4 + 8);
    canvas.text_to_right(0, 0, n88_normal_font(), "ABCabc123");
    canvas.text_to_right(0, 16, n88_quality_font(), "ABCabc123");
    canvas.text_to_right(0, 32, n88_graph_font(), "ABCabc123");
    canvas.text_to_right(0, 48, n88_quarter_font(), "ABCabc123");
    canvas.text_to_right(0, 56, qbasic8x16_font(), "ABCabc123");
    std::cout << canvas.to_str();
    return 0;
}
