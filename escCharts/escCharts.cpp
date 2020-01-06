#include "GraphicalChartsDS.hpp"
#include "BMP.hpp"
#include <vector>

using namespace  std;
using namespace _BMP;
using namespace _GraphicalCharts;

int main()
{
	std::vector<DataNode> node;
	DataNode n = DataNode{ "test", 5, {100,0,68},10 };
	n.GenerateColor();
	node.push_back(n);
	node.push_back(DataNode{ "test", 5, {100,0,68}, 5 });
	node.push_back(DataNode{ "test", 5, {100,0,68}, 6 });
	node.push_back(DataNode{ "test", 5, {100,0,68},4 });
	node.push_back(DataNode{ "test", 5, {100,0,68},15 });
	node.push_back(DataNode{ "test", 5, {100,0,68}, 13 });
	node.push_back(DataNode{ "test", 5, {100,0,68}, 7 });
	node.push_back(DataNode{ "test", 5, {100,0,68}, 22 });
	node.push_back(DataNode{ "test", 5, {100,0,68}, 18 });
    string pathName = "out/";
	
    Image img(720, 480, pathName + "sample1.bmp", COLOR_WHITE);
    Image img2(720, 480, pathName + "sample2.bmp", COLOR_BLACK);
	
	Charts chart(img);
	Charts chart2(img2);

	chart.ShowFrame();
	chart.ShowFrameCorners();
	chart.DrawTowerChart(node);
	chart2.DrawLineChart(node);
	chart.ShowFrame();
	chart.ShowFrameCorners();
	
	chart.Update();
	chart2.Update();
	
    return 0;
}