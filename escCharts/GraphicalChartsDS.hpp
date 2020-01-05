#ifndef _GRAPHICAL_CHARTS_H_
#define _GRAPHICAL_CHARTS_H_

#include "_BMP.hpp"
#include <array>
#include <cassert>
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

        Frame(Image img)
        {
            TopLeft.x = 10;
            TopLeft.y = 10;
            BottomRight.x = img.w() - 10;
            BottomRight.y = img.h() - 10;

            InitFrame();
        }
    	
        Frame(Point _TopLeft, Point _BottomRight)
        {
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
        Charts(Image& _img) : img(_img), frame(_img) {};
        Charts(Image& _img, Frame _frame) : img(_img), frame(_frame){};
    private:
        std::vector<int64_t> data;
    	
    public:
        void DisplayFrame(const bool& show);
        inline Image GetImage() { return this->img; }
    	
        Frame frame;
        Image img;
    };

    void Charts::DisplayFrame(const bool& show = true)
    {
	    if(show)
	    {
            RGBColor outlineColor;
            if (img.GetBGColor() == COLOR_BLACK)
                outlineColor = COLOR_WHITE;
            else if (img.GetBGColor() == COLOR_WHITE)
                outlineColor = COLOR_FRAME_LIGHT;
            else
                outlineColor = COLOR_FRAME_DARK;
	    	
			img.DrawRectangle(frame.TopLeft.x, frame.TopLeft.y, frame.BottomRight.x, frame.BottomRight.y, outlineColor, 1, true, img.GetBGColor());
	    }
        else {
            img.DrawRectangle(frame.TopLeft.x, frame.TopLeft.y, frame.BottomRight.x, frame.BottomRight.y, img.GetBGColor(), 1, true, img.GetBGColor());
        }
    }
	
}

#endif