#include "BMP.h"
#include "_BMP.hpp"
#include <vector>

using namespace  std;
using namespace BMPFormat;
using namespace _BMP;

int main()
{
    int radius = 250,
		x = 256,
		y = 256;
	
    // sizeX, sizeY, FileName, BackgroundColor
    _BMP::Image img(512, 512, "sample1.bmp", COLOR_WHITE);

    img.DrawCircle(x, y,radius, COLOR_CYAN);
    img.Write();
    return 0;
}