#include "GraphicalChartsDS.hpp"
#include "BMP.hpp"
#include <vector>

using namespace  std;
using namespace _BMP;
using namespace _GraphicalCharts;

int main()
{
	std::vector<DataNode> node;
	DataNode n = { "test", 5, {100,0,68} };
	n.GenerateColor();
	node.push_back(n);
    string pathName = "out/";
	
    Image img(720, 480, pathName + "sample1.bmp", COLOR_BLACK);
	
	Charts chart(img);

	chart.ShowFrame();
	chart.ShowFrameCorners();
	chart.BuildTowers(node);
	
	chart.Update();
	
    return 0;
}