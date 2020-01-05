#ifndef _GRAPHICAL_CHARTS_H_
#define _GRAPHICAL_CHARTS_H_

#include "BMP.hpp"
#include <time.h>
#include <stdlib.h>
#include <cstdint>
#include <cmath>
#include <vector>

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
    	DataNode() : title("Lorem"), value(0), fillColor(RGBColor{ 0, 0, 0 }) {}
    	DataNode(string _title) : title(_title), value(0), fillColor(RGBColor{0, 0, 0}){}
    	DataNode(string _title, double _value) : title(_title), value(_value), fillColor(RGBColor{0, 0, 0}){}
    	DataNode(string _title, double _value, RGBColor _fillColor) : title(_title), value(_value), fillColor(_fillColor){}
    public:
        string title;
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

        int64_t spacing;

        Frame(Image img)
        {
            spacing = 10;
        	
            TopLeft.x = spacing;
            TopLeft.y = spacing;
            BottomRight.x = img.w() - spacing;
            BottomRight.y = img.h() - spacing;

            InitFrame();
        }
    	
        Frame(Point _TopLeft, Point _BottomRight)
        {
            spacing = 10;
        	
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
	    	
			img.DrawRectangle(frame.TopLeft.x, frame.TopLeft.y, frame.BottomRight.x, frame.BottomRight.y, outlineColor, 1, true, img.GetBGColor());
	    }
        else {
            img.DrawRectangle(frame.TopLeft.x, frame.TopLeft.y, frame.BottomRight.x, frame.BottomRight.y, img.GetBGColor(), 1, true, img.GetBGColor());
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

            img.DrawRectangle(frame.TopLeft.x - frame.spacing / 2, frame.TopLeft.y - frame.spacing / 2, frame.TopLeft.x + frame.spacing / 2, frame.TopLeft.y + frame.spacing / 2, outlineColor, 1, true, img.GetBGColor());
            img.DrawRectangle(frame.Top.x - frame.spacing / 2, frame.Top.y - frame.spacing / 2, frame.Top.x + frame.spacing / 2, frame.Top.y + frame.spacing / 2, outlineColor, 1, true, img.GetBGColor());
            img.DrawRectangle(frame.TopRight.x - frame.spacing / 2, frame.TopRight.y - frame.spacing / 2, frame.TopRight.x + frame.spacing / 2, frame.TopRight.y + frame.spacing / 2, outlineColor, 1, true, img.GetBGColor());
	    	
            img.DrawRectangle(frame.Left.x- frame.spacing / 2, frame.Left.y- frame.spacing / 2, frame.Left.x+ frame.spacing / 2, frame.Left.y + frame.spacing / 2, outlineColor, 1, true, img.GetBGColor());
            img.DrawRectangle(frame.Right.x- frame.spacing / 2, frame.Right.y- frame.spacing / 2, frame.Right.x+ frame.spacing / 2, frame.Right.y + frame.spacing / 2, outlineColor, 1, true, img.GetBGColor());
	    	
            img.DrawRectangle(frame.BottomLeft.x- frame.spacing / 2, frame.BottomLeft.y- frame.spacing / 2, frame.BottomLeft.x+ frame.spacing / 2, frame.BottomLeft.y+ frame.spacing / 2, outlineColor, 1, true, img.GetBGColor());
	    	img.DrawRectangle(frame.Bottom.x- frame.spacing / 2, frame.Bottom.y- frame.spacing / 2, frame.Bottom.x+ frame.spacing / 2, frame.Bottom.y+ frame.spacing / 2, outlineColor, 1, true, img.GetBGColor());
	    	img.DrawRectangle(frame.BottomRight.x- frame.spacing / 2, frame.BottomRight.y- frame.spacing / 2, frame.BottomRight.x+ frame.spacing / 2, frame.BottomRight.y+ frame.spacing / 2, outlineColor, 1, true, img.GetBGColor());
	    }
        else
        {
            img.DrawRectangle(frame.TopLeft.x - frame.spacing / 2, frame.TopLeft.y - frame.spacing / 2, frame.TopLeft.x + frame.spacing / 2, frame.TopLeft.y +  frame.spacing / 2, img.GetBGColor(), 1, true, img.GetBGColor());
            img.DrawRectangle(frame.Top.x - frame.spacing / 2, frame.Top.y - frame.spacing / 2, frame.Top.x + frame.spacing / 2, frame.Top.y +  frame.spacing / 2, img.GetBGColor(), 1, true, img.GetBGColor());
            img.DrawRectangle(frame.TopRight.x - frame.spacing / 2, frame.TopRight.y - frame.spacing / 2, frame.TopRight.x + frame.spacing / 2, frame.TopRight.y +  frame.spacing / 2, img.GetBGColor(), 1, true, img.GetBGColor());

            img.DrawRectangle(frame.Left.x -  frame.spacing / 2, frame.Left.y -  frame.spacing / 2, frame.Left.x +  frame.spacing / 2, frame.Left.y +  frame.spacing / 2, img.GetBGColor(), 1, true, img.GetBGColor());
            img.DrawRectangle(frame.Right.x -  frame.spacing / 2, frame.Right.y -  frame.spacing / 2, frame.Right.x +  frame.spacing / 2, frame.Right.y +  frame.spacing / 2, img.GetBGColor(), 1, true, img.GetBGColor());

            img.DrawRectangle(frame.BottomLeft.x -  frame.spacing / 2, frame.BottomLeft.y -  frame.spacing / 2, frame.BottomLeft.x +  frame.spacing / 2, frame.BottomLeft.y +  frame.spacing / 2, img.GetBGColor(), 1, true, img.GetBGColor());
            img.DrawRectangle(frame.Bottom.x -  frame.spacing / 2, frame.Bottom.y -  frame.spacing / 2, frame.Bottom.x +  frame.spacing / 2, frame.Bottom.y +  frame.spacing / 2, img.GetBGColor(), 1, true, img.GetBGColor());
            img.DrawRectangle(frame.BottomRight.x -  frame.spacing / 2, frame.BottomRight.y -  frame.spacing / 2, frame.BottomRight.x +  frame.spacing / 2, frame.BottomRight.y +  frame.spacing / 2, img.GetBGColor(), 1, true, img.GetBGColor());
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
            img.DrawRectangle(frame.BottomLeft.x + ceil(width/4), frame.Bottom.y, frame.TopRight.x - ceil(width/4), maxHeight + frame.spacing, data_nodes[0].fillColor, 1, true, img.GetBGColor());
    	}
        else if(data_nodes.size() > 1 && data_nodes.size() < 4){}
        else{}
    }
	
}

#endif