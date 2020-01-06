#ifndef _GRAPHICAL_CHARTS_H_
#define _GRAPHICAL_CHARTS_H_

#include "BMP.hpp"
#include <time.h>
#include <stdlib.h>
#include <cstdint>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>

namespace _GraphicalCharts
{
    using std::string;
    using std::int64_t;
    using std::uint32_t;
    using std::uint8_t;
    using std::ofstream;
    using namespace _BMP;

    class DataNode
    {
    public:
    	~DataNode(){}
    	DataNode() : title("Lorem"), value(0), fillColor(RGBColor{ 0, 0, 0 }), percent(0) {}
    	DataNode(string _title) : title(_title), value(0), fillColor(RGBColor{0, 0, 0}), percent(0) {}
    	DataNode(string _title, double _value) : title(_title), value(_value), fillColor(RGBColor{0, 0, 0}), percent(0) {}
    	DataNode(string _title, double _value, RGBColor _fillColor) : title(_title), value(_value), fillColor(_fillColor), percent(0) {}
    	DataNode(string _title, double _value, RGBColor _fillColor, int64_t _percent) : title(_title), value(_value), fillColor(_fillColor), percent(_percent) {}
    public:
        string title;
        int64_t percent;
        double value;
        RGBColor fillColor;

        void GenerateColor();
    };

    void DataNode::GenerateColor()
    {
        srand(time(NULL));
        uint8_t r = rand() % 255 + 1, g = rand() % 255 + 1, b = rand() % 255 + 1;

        RGBColor newColor = {r, g ,b};

        if (this->fillColor != newColor)
            this->fillColor = newColor;
    }

    class TableNode
    {
    public:
        ~TableNode() {}
        TableNode() : filePath("input.csv"), nodes(NULL) {}
        TableNode(string _filePath) : filePath(_filePath), nodes(NULL) {}
        TableNode(string _filePath, std::vector<DataNode> _nodes) : filePath(_filePath), nodes(_nodes) {}
    	
    public:
        bool CheckFile();
        void ReadFile();
        void UpdateFile();
        void UpdateNodes();

        inline void SetFilePath(string newFilePath) { this->filePath = newFilePath;  UpdateFile(); }
        inline string GetFilePath() { return this->filePath; }
        inline DataNode GetFirst() { return this->nodes[0]; }
        inline DataNode GetLast() { return this->nodes[this->nodes.size() - 1]; }
    	
        inline int64_t GetSize() { return this->nodes.size(); }

        bool operator == (const TableNode&);
        bool operator != (const TableNode&);
        bool operator > (const TableNode&);
        bool operator >= (const TableNode&);
    	
    protected:
        string filePath;
        std::vector<DataNode> nodes;
    };

    /*void TableNode::ReadFile()
    {
	    std::fstream fin;

	    fin.open(this->GetFilePath(), std::ios::in);

	    int rollnum, roll2, count = 0;
	    std::cout << "Enter the roll number "
		    << "of the student to display details: ";
	    std::cin >> rollnum;

	    std::vector<string> row;
	    string line, word, temp;

	    while (fin >> temp)
	    {
		    row.clear();
	    	
		    getline(fin, line);

		    std::stringstream s(line);

		    while ( getline(s, word, ', ') )
		    {
			    row.push_back(word);
		    }

		    roll2 = std::stoi(row[0]);

		    if (roll2 == rollnum)
		    {
			    count = 1;
			    std::cout << "Details of Roll " << row[0] << " : \n";
			    std::cout << "Name: " << row[1] << "\n";
			    std::cout << "Maths: " << row[2] << "\n";
			    std::cout << "Physics: " << row[3] << "\n";
			    std::cout << "Chemistry: " << row[4] << "\n";
			    std::cout << "Biology: " << row[5] << "\n";
			    break;
		    }
	    }
	    if (count == 0)
		    std::cout << "Record not found\n";
    }

	
	/*
	 *   []-----[]-----[]
	 *   |             |
	 *  []            []
	 *  |             |
	 * []-----[]-----[]
	 */
    class Frame
    {
    public:
        Point Top;
        Point TopLeft;
        Point TopRight;

        Point Bottom;
        Point BottomLeft;
        Point BottomRight;

        Point Left;
        Point Right;

        Point Spacing;

        Frame(Image img)
        {
            Spacing = { 10,10 };
        	
            TopLeft = Spacing;
            BottomRight.x = img.w() - Spacing.x;
            BottomRight.y = img.h() - Spacing.y;

            InitFrame();
        }
    	
        Frame(Point _TopLeft, Point _BottomRight)
        {
            Spacing = { 10,10 };
        	
            TopLeft = _TopLeft;
            BottomRight = _BottomRight;
        	
            InitFrame();
        }

        inline int64_t GetDistance(const Point from, const Point to)
        {
            return sqrt((to.x - from.x) * (to.x - from.x) + (to.y - from.y) * (to.y - from.y));
        }
    	
        void InitFrame();
    };

    inline void Frame::InitFrame()
    {
        TopRight.x = BottomRight.x;
        TopRight.y = TopLeft.y;
    	
        BottomLeft.x = TopLeft.x;
        BottomLeft.y = BottomRight.y;

        Top.x = GetDistance(TopLeft, TopRight) / 2;
        Top.y = TopLeft.y;

    	Bottom.x = GetDistance(BottomLeft,BottomRight) / 2;
        Bottom.y = BottomRight.y;

        Left.x = TopLeft.x;
        Left.y = GetDistance(TopLeft, BottomLeft) / 2;

        Right.x = TopRight.x;
        Right.y = GetDistance(TopRight, BottomRight) / 2;;
    }
	
    class Charts
    {
    public:
        ~Charts() {};
        Charts() : img(Image(256, 256)), frame(img) {};
        Charts(Image& _img) : img(_img), frame(_img) {};
        Charts(Image& _img, Frame _frame) : img(_img), frame(_frame){};
    	
    private:
        std::vector<int64_t> data;
    	
    public:
        void ShowFrame(const bool& show);
        void ShowFrameCorners(const bool& show);

        void BuildTowers(const std::vector<DataNode>& data_nodes);
        void DrawRectangleByLines(const Point& TopLeft, const Point& BottomRight, const RGBColor& outlineColor);
        int64_t GetPositionByPercent(const double& value, const int64_t& maxHeight);
        void DrawLineChart(const std::vector<DataNode>& data_nodes);
    	
        inline Image GetImage() { return this->img; }
    	
        Frame frame;
        Image img;

        inline void Update() { this->img.Write(); }
    };

    void Charts::ShowFrame(const bool& show = true)
    {
	    if(show)
	    {
            RGBColor outlineColor;
            if (img.GetBGColor() == COLOR_BLACK)
                outlineColor = COLOR_WHITE;
            else if (img.GetBGColor() == COLOR_WHITE)
                outlineColor = COLOR_GREY;
            else
                outlineColor = COLOR_FRAME_DARK;
	    	
			DrawRectangleByLines(frame.TopLeft, frame.BottomRight, outlineColor);
	    }
        else {
            DrawRectangleByLines(frame.TopLeft, frame.BottomRight, img.GetBGColor());
        }
    }

    void Charts::ShowFrameCorners(const bool& show = true)
    {
	    if(show)
	    {
            RGBColor outlineColor;
            if (img.GetBGColor() == COLOR_BLACK)
                outlineColor = COLOR_WHITE;
            else if (img.GetBGColor() == COLOR_WHITE)
                outlineColor = COLOR_GREY;
            else
                outlineColor = COLOR_FRAME_DARK;

            DrawRectangleByLines(frame.TopLeft - frame.Spacing / 2, frame.TopLeft + frame.Spacing / 2, outlineColor);
            DrawRectangleByLines(frame.Top - frame.Spacing / 2, frame.Top + frame.Spacing / 2, outlineColor);
            DrawRectangleByLines(frame.TopRight - frame.Spacing / 2, frame.TopRight + frame.Spacing / 2, outlineColor);

            DrawRectangleByLines(frame.Left - frame.Spacing / 2, frame.Left + frame.Spacing / 2, outlineColor);
            DrawRectangleByLines(frame.Right - frame.Spacing / 2, frame.Right + frame.Spacing / 2, outlineColor);

            DrawRectangleByLines(frame.BottomLeft - frame.Spacing / 2, frame.BottomLeft + frame.Spacing / 2, outlineColor);
            DrawRectangleByLines(frame.Bottom - frame.Spacing / 2, frame.Bottom + frame.Spacing / 2, outlineColor);
            DrawRectangleByLines(frame.BottomRight - frame.Spacing / 2, frame.BottomRight + frame.Spacing / 2, outlineColor);
        }
        else
        {
            DrawRectangleByLines(frame.TopLeft - frame.Spacing / 2, frame.TopLeft + frame.Spacing / 2, img.GetBGColor());
            DrawRectangleByLines(frame.Top - frame.Spacing / 2, frame.Top + frame.Spacing / 2, img.GetBGColor());
            DrawRectangleByLines(frame.TopRight - frame.Spacing / 2, frame.TopRight + frame.Spacing / 2, img.GetBGColor());

            DrawRectangleByLines(frame.Left - frame.Spacing / 2, frame.Left + frame.Spacing / 2, img.GetBGColor());
            DrawRectangleByLines(frame.Right - frame.Spacing / 2, frame.Right + frame.Spacing / 2, img.GetBGColor());

            DrawRectangleByLines(frame.BottomLeft - frame.Spacing / 2, frame.BottomLeft + frame.Spacing / 2, img.GetBGColor());
            DrawRectangleByLines(frame.Bottom - frame.Spacing / 2, frame.Bottom + frame.Spacing / 2, img.GetBGColor());
            DrawRectangleByLines(frame.BottomRight - frame.Spacing / 2, frame.Bottom + frame.Spacing / 2, img.GetBGColor());
        }
    }

    void Charts::BuildTowers(const std::vector<DataNode>& data_nodes)
    {
        if (data_nodes.empty())
            return;
    	
        int64_t maxHeight = frame.Top.y;
        int64_t width = frame.GetDistance(frame.BottomLeft, frame.BottomRight);        

    	if(data_nodes.size() == 1)
    	{
            img.DrawRectangle(frame.BottomLeft.x + ceil(width/4), frame.Bottom.y, frame.TopRight.x - ceil(width/4), maxHeight + frame.Spacing.y, data_nodes[0].fillColor, 1, true, img.GetBGColor());
    	}
        else if(data_nodes.size() > 1 && data_nodes.size() < 4){}
        else{}
    }

    void Charts::DrawRectangleByLines(const Point& TopLeft, const Point& BottomRight, const RGBColor& outlineColor) {
        img.DrawLine(TopLeft.x, TopLeft.y, TopLeft.x, BottomRight.y, outlineColor);
        img.DrawLine(TopLeft.x, BottomRight.y, BottomRight.x, BottomRight.y, outlineColor);
        img.DrawLine(BottomRight.x, BottomRight.y, BottomRight.x, TopLeft.y, outlineColor);
        img.DrawLine(BottomRight.x, TopLeft.y, TopLeft.x, TopLeft.y, outlineColor);
    }

    int64_t Charts::GetPositionByPercent(const double& value, const int64_t& maxHeight) {
        return ceil((value * maxHeight) / 100);
    }

    void Charts::DrawLineChart(const std::vector<DataNode>& data_nodes) {
        if (data_nodes.empty())
            return;

        int64_t maxHeight = img.h() -frame.Top.y + frame.Spacing.y;
        int64_t partialPoint = ceil(frame.GetDistance(frame.BottomLeft, frame.BottomRight) / data_nodes.size());

        Point pointCenter = frame.BottomLeft;

        for (DataNode node : data_nodes) {
            img.DrawCircle(pointCenter.x, pointCenter.y, 4, COLOR_BLUE_LIGHT);
            img.DrawLine(pointCenter.x, pointCenter.y, pointCenter.x + partialPoint, GetPositionByPercent(node.percent, maxHeight), COLOR_RED);
            pointCenter.x += partialPoint;
            pointCenter.y = GetPositionByPercent(node.percent, maxHeight);
            img.DrawCircle(pointCenter.x, pointCenter.y, 4, COLOR_BLUE_LIGHT);
        }
    }
}

#endif