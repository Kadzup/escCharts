#include "GraphicalChartsDS.hpp"
#include "BMP.hpp"
#include <vector>

using namespace  std;
using namespace _BMP;
using namespace _GraphicalCharts;

int main()
{
	std::vector<DataNode> node;
	node.push_back(DataNode{ "test", 5, {100,0,68},25 });
	node.push_back(DataNode{ "test", 5, {100,0,68},5 });
	node.push_back(DataNode{ "test", 5, {100,0,68},7 });
	node.push_back(DataNode{ "test", 5, {100,0,68},23 });
	node.push_back(DataNode{ "test", 5, {100,0,68},10 });
	node.push_back(DataNode{ "test", 5, {100,0,68},12 });
	node.push_back(DataNode{ "test", 5, {100,0,68},18 });
	/*node.push_back(DataNode{ "test", 5, {100,0,68}, 22 });
	node.push_back(DataNode{ "test", 5, {100,0,68}, 18 });*/
    string pathName = "out/";
	
    Image img(720, 480, pathName + "sample1.bmp", COLOR_WHITE);
    Image img2(720, 480, pathName + "sample2.bmp", COLOR_WHITE);
    Image img3(720, 480, pathName + "sample3.bmp", COLOR_WHITE);
	
	Charts chart(img);
	Charts chart2(img2);
	Charts chart3(img3);

	chart.ShowFrame();
	chart.ShowFrameCorners();
	chart.DrawTowerChart(node);
	chart2.ShowFrame();
	chart2.ShowXY();
	chart2.DrawLineChart(node);
	chart3.SplitCircle(node);
	chart3.ShowFrame();
	chart.ShowFrame();
	chart.ShowFrameCorners();
	
	chart.Update();
	chart2.Update();
	chart3.Update();
	
    return 0;
}