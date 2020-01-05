#include "BMP.h"
#include "GraphicalChartsDS.hpp"
#include "_BMP.hpp"
#include <vector>

using namespace  std;
using namespace _BMP;
using namespace _GraphicalCharts;

int main()
{
    int radius = 250/2,
		x = 256,
		y = 256;
	
    string pathName = "out/";
	
    Image img(500, 500, pathName + "sample1.bmp", COLOR_BLACK);
	
	Charts chart(img);

	chart.DisplayFrame(true);

	chart.img.Write();
	
    return 0;
}