#include "BMP.h"
#include "_BMP.hpp"
#include <vector>

using namespace  std;
using namespace _BMP;

int main()
{
    int radius = 250/2,
		x = 256,
		y = 256;
	
    string pathName = "out/";
	
    // sizeX, sizeY, FileName, BackgroundColor
    _BMP::Image img(500, 500, pathName + "sample1.bmp", COLOR_WHITE);

    img.DrawCircle(x, y, radius, COLOR_BLACK, 5, true, COLOR_CYAN);
    img.DrawRectangle(30, 30, 200, 200, COLOR_BLUE, 5, true, COLOR_YELLOW);
    img.Write();
    return 0;
}