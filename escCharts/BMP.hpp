#ifndef _BMP_H_
#define _BMP_H_

#include <array>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <cmath>

#define COLOR_WHITE RGBColor{255, 255, 255}
#define COLOR_BLACK RGBColor{0, 0, 0}
#define COLOR_RED RGBColor{255, 0, 0}
#define COLOR_GREEN RGBColor{0, 255, 0}
#define COLOR_BLUE RGBColor{0, 0, 255}
#define COLOR_ORANGE RGBColor{255, 128, 0}
#define COLOR_YELLOW RGBColor{255, 255, 0}
#define COLOR_GREEN_LIGHT RGBColor{128, 255, 0}
#define COLOR_CYAN RGBColor{0, 255, 128}
#define COLOR_BLUE_LIGHT RGBColor{0, 255, 255}
#define COLOR_BLUE_SKY RGBColor{0, 128, 255}
#define COLOR_MAGENTA RGBColor{153, 51, 255}
#define COLOR_PINK RGBColor{255, 102, 178}
#define COLOR_GREY RGBColor{96, 96, 96}
#define COLOR_BROWN RGBColor{51, 25, 0}
#define COLOR_FRAME_LIGHT RGBColor{153, 204, 255}
#define COLOR_FRAME_DARK RGBColor{226, 226, 226}

namespace _BMP
{
    using std::string;
    using std::int64_t;
    using std::uint32_t;
    using std::uint8_t;
    using std::ofstream;

    struct Point
    {
        int64_t x, y;
    	
        Point(int64_t _x, int64_t _y) : x(_x), y(_y) { }
        Point() : x(NULL), y(NULL) { }
    	
        double distanceTo(const Point& o) {
            int64_t dx = (x - o.x);
            int64_t dy = (y - o.y);
            return sqrt(dx * dx + dy * dy);
        }
    	
        inline Point operator + (const Point& rObj) { return Point{ x + rObj.x, y + rObj.y }; }
        inline Point operator - (const Point& rObj) { return Point{ x - rObj.x, y - rObj.y }; }
        inline Point operator / (double value) { return Point{ int64_t(ceil(x / value)), int64_t(ceil(y / value)) }; }
        inline bool operator > (const Point& rhs) { return (this->x > rhs.x)&&(this->y > rhs.y); }
    };
	
    class RGBColor
    {
    public:
        RGBColor() {}; // Don't use void constructor, just for variable definition
        ~RGBColor() {};
        RGBColor(uint8_t _r, uint8_t _g, uint8_t _b);
        void SetColor(uint8_t _r, uint8_t _g, uint8_t _b);

    	bool IsEmpty()
    	{
            return (r && g && b);
    	}

        inline bool operator == (const RGBColor& in) { return (r == in.r) && (g == in.g) && (b == in.b); }
        inline bool operator != (const RGBColor& in) { return (r != in.r) || (g != in.g) || (b != in.b); }
    	
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    RGBColor::RGBColor(uint8_t _r, uint8_t _g, uint8_t _b)
    {
        SetColor(_r, _g, _b);
    }

    void RGBColor::SetColor(uint8_t _r, uint8_t _g, uint8_t _b)
    {
        r = _r;
        g = _g;
        b = _b;
    }

    /*
     * Image object with graphical functions
     */
    class Image
    {
    public:
        ~Image();
        Image() { width = height = 0; };
        Image(int64_t _width, int64_t _height);
        Image(int64_t _width, int64_t _height, const string& _outFileName);
        Image(int64_t _width, int64_t _height, const RGBColor& _backgroundColor);
        Image(int64_t _width, int64_t _height, const string& _outFileName, const RGBColor& _backgroundColor);
        Image(const Image& _image);
    	
        void SetPixel(int64_t x, int64_t y, const RGBColor& color, bool ignore_err);
    	
        void DrawLine(int64_t x0, int64_t y0, int64_t x1, int64_t y1, const RGBColor& color);
        void DrawBezier(int64_t x1, int64_t y1, int64_t x2, int64_t y2, int64_t x3, int64_t y3, const RGBColor& color);
    	
        void DrawCircle(int64_t x0, int64_t y0, int64_t r, const RGBColor& outlineColor, int64_t outlineWidth, bool fill, const RGBColor& fillColor);
        void FillCircle(int64_t x0, int64_t y0, int64_t r, const RGBColor& fillColor);
    	
        void DrawRectangle(int64_t x1, int64_t y1, int64_t x2, int64_t y2, const RGBColor& outlineColor, int64_t outlineWidth, bool fill, const RGBColor& fillColor);
        void FillRectangle(int64_t x1, int64_t y1, int64_t x2, int64_t y2, const RGBColor& fillColor);
    	
        void SetFileName(const string& _outFileName);

        void ConvertToPPM();
        void VerticalFlip(int64_t width, int64_t height, unsigned char* ucmat);
    	
        void Write(const string& _outFileName);
        void Write();
    	
        inline int64_t w() const { return width; }
        inline int64_t h() const { return height; }

        inline RGBColor GetBGColor() {
            return this->backgroundColor;
        }

        Image& operator = (const Image& img)
        {
            if (this == &img) {
                return *this;
            }
            this->width = img.width;
            this->height = img.height;
            this->buffer = img.buffer;
            this->outFileName = img.outFileName;
            this->backgroundColor = img.backgroundColor;

            return *this;
        }
    	
    private:
        void Init(int64_t _width, int64_t _height);
        void Setup();
    	
        void DrawLineLow(int64_t x0, int64_t y0, int64_t x1, int64_t y1, const RGBColor& color);
        void DrawLineHigh(int64_t x0, int64_t y0, int64_t x1, int64_t y1, const RGBColor& color);

        int64_t width;
        int64_t height;
        ofstream outFile;
        string outFileName;
        RGBColor** buffer;
    	
    public:
        RGBColor backgroundColor;
    };

    // Free and close used classes
    Image::~Image()
    {
        if (buffer != NULL) {
            for (int64_t i = 0; i < height; ++i) {
                delete[] buffer[i];
            }
            delete[] buffer;
            buffer = NULL;
        }
    }
    Image::Image(const Image& _image)
    {
        Init(_image.width, _image.height);
        SetFileName(_image.outFileName);
        backgroundColor = _image.backgroundColor;
        Setup();
    }
	
    Image::Image(int64_t _width, int64_t _height)
    {
        Init(_width, _height);
        Setup();
    }

    Image::Image(int64_t _width, int64_t _height, const string& _outFileName)
    {
        Init(_width, _height);
        SetFileName(_outFileName);
        Setup();
    }

    Image::Image(int64_t _width, int64_t _height, const RGBColor& _backgroundColor)
    {
        Init(_width, _height);
        backgroundColor = _backgroundColor;
        Setup();
    }

    Image::Image(int64_t _width, int64_t _height, const string& _outFileName, const RGBColor& _backgroundColor)
    {
        Init(_width, _height);
        SetFileName(_outFileName);
        backgroundColor = _backgroundColor;
        Setup();
    }

    // Load constant values
    void Image::Init(int64_t _width, int64_t _height)
    {
        assert(_width > 0 and _height > 0);
        width = _width;
        height = _height;
        buffer = NULL;
        outFileName = "image.bmp";
        backgroundColor = RGBColor(255, 255, 255);
    }

    // Setup all classes before start using
    void Image::Setup()
    {
        try {
            buffer = new RGBColor * [height];
            for (int64_t y = 0; y < height; ++y) {
                buffer[y] = new RGBColor[width];
                // Fill with background color:
                for (int64_t x = 0; x < width; ++x) {
                    buffer[y][x] = backgroundColor;
                }
            }
        }
        catch (std::bad_alloc & ba) {
            std::cerr << "_BMP ERROR: bad_alloc error (Can't create image buffer) -> " << ba.what() << std::endl;
            assert(false);
        }
    }

    void Image::SetPixel(int64_t x, int64_t y, const RGBColor& color, bool ignore_err = false)
    {
        if (ignore_err and not(x >= 0 and y >= 0 and x < width and y < height)) return;
        assert(x >= 0 and y >= 0 and x < width and y < height);
        buffer[y][x] = color;
    }

    void Image::DrawLine(int64_t x0, int64_t y0, int64_t x1, int64_t y1, const RGBColor& color)
    {
        if (abs(y1 - y0) < abs(x1 - x0)) {
            if (x0 > x1) {
                DrawLineLow(x1, y1, x0, y0, color);
            }
            else {
                DrawLineLow(x0, y0, x1, y1, color);
            }
        }
        else {
            if (y0 > y1) {
                DrawLineHigh(x1, y1, x0, y0, color);
            }
            else {
                DrawLineHigh(x0, y0, x1, y1, color);
            }
        }
    }

    void Image::DrawBezier(int64_t x1, int64_t y1, int64_t x2, int64_t y2, int64_t x3, int64_t y3, const RGBColor& color)
    {
        float xa, ya, xb, yb, x, y;
        for (float i = 0; i < 1; i += 0.01)
        {
            xa = x1 + ((x2 - x1) * i);
            ya = y1 + ((y2 - y1) * i);
            xb = x2 + ((x3 - x2) * i);
            yb = y2 + ((y3 - y2) * i);
            
            // The Black Dot
            x = ceil(xa) + ceil((xb - xa) * i);
            y = ceil(ya) + ceil((yb - ya) * i);

            SetPixel(ceil(x), ceil(y), color);
        }
    }
	
    void Image::DrawLineLow(int64_t x0, int64_t y0, int64_t x1, int64_t y1, const RGBColor& color)
    {
        int64_t dx = x1 - x0;
        int64_t dy = y1 - y0;
        int64_t yi = 1;
        if (dy < 0) {
            yi = -1;
            dy = -dy;
        }
        int64_t D = (dy << 1) - dx;
        int64_t y = y0;
        for (int64_t x = x0; x <= x1; ++x) {
            SetPixel(x, y, color, true);
            if (D > 0) {
                y += yi;
                D -= (dx << 1);
            }
            D += (dy << 1);
        }
    }

    void Image::DrawLineHigh(int64_t x0, int64_t y0, int64_t x1, int64_t y1, const RGBColor& color)
    {
        int64_t dx = x1 - x0;
        int64_t dy = y1 - y0;
        int64_t xi = 1;
        if (dx < 0) {
            xi = -1;
            dx = -dx;
        }
        int64_t D = (dx << 1) - dy;
        int64_t x = x0;
        for (int64_t y = y0; y <= y1; ++y) {
            SetPixel(x, y, color, true);
            if (D > 0) {
                x += xi;
                D -= (dy << 1);
            }
            D += (dx << 1);
        }
    }

    void Image::DrawCircle(int64_t x0, int64_t y0, int64_t r, const RGBColor& outlineColor, int64_t outlineWidth = 1, bool fill = false, const RGBColor& fillColor = COLOR_WHITE)
    {
        assert(x0 >= 0 and y0 >= 0 and x0 < width and y0 < height);
        assert(outlineWidth > 0);
       
        int64_t sq_r = r * r;
        for (int64_t dx = -r; dx <= r; ++dx) {
            for (int64_t dy = -r; dy <= r; ++dy) {
                int64_t sq_dist = dx * dx + dy * dy;
                if (sq_dist <= sq_r) {
                    int64_t x = x0 + dx;
                    int64_t y = y0 + dy;
                    if (outlineWidth > 1)
                        SetPixel(x, y, outlineColor, true);
                    else
                        SetPixel(x, y, fillColor, true);
                }
            }
        }
        if (outlineWidth > 1)
        {
            DrawCircle(x0, y0, r - outlineWidth, outlineColor, 1, fill, fillColor);
        }
        
        /*else {
            int64_t x = r - 1;
            int64_t y = 0;
            int64_t dx = 1;
            int64_t dy = 1;
            int64_t err = dx - (r << 1);

            while (x >= y) {
                SetPixel(x0 + x, y0 + y, outlineColor, true);
                SetPixel(x0 + y, y0 + x, outlineColor, true);

                SetPixel(x0 - y, y0 + x, outlineColor, true);
                SetPixel(x0 - x, y0 + y, outlineColor, true);

                SetPixel(x0 - x, y0 - y, outlineColor, true);
                SetPixel(x0 - y, y0 - x, outlineColor, true);

                SetPixel(x0 + y, y0 - x, outlineColor, true);
                SetPixel(x0 + x, y0 - y, outlineColor, true);

                if (err <= 0) {
                    y++;
                    err += dy;
                    dy += 2;
                }

                if (err > 0) {
                    x--;
                    dx += 2;
                    err += dx - (r << 1);
                }
            }
        }
       /* if (fill)
        {
            FillCircle(x0, y0, r, fillColor);
            DrawCircle(x0, y0, r, outlineColor, outlineWidth, false, fillColor);
        }
    	
        
    	if(outlineWidth > 1)
        {
            DrawCircle(x0, y0, r - outlineWidth, outlineColor, outlineWidth - 1, fill, fillColor);
        }
    	*/
    }

    void Image::FillCircle(int64_t x0, int64_t y0, int64_t r, const RGBColor& fillColor)
    {
        int64_t sq_r = r * r;
        for (int64_t dx = -r; dx <= r; ++dx) {
            for (int64_t dy = -r; dy <= r; ++dy) {
                int64_t sq_dist = dx * dx + dy * dy;
                if (sq_dist <= sq_r) {
                    int64_t x = x0 + dx;
                    int64_t y = y0 + dy;
                    SetPixel(x, y, fillColor, true);
                }
            }
        }
    }

    void Image::DrawRectangle(int64_t x1, int64_t y1, int64_t x2, int64_t y2, const RGBColor& outlineColor, int64_t outlineWidth = 1, bool fill = false, const RGBColor& fillColor = COLOR_WHITE)
    {
        assert(x1 >= 0 and y1 >= 0 and x1 < width and y1 < height);
        assert(x2 >= 0 and y2 >= 0 and x2 < width and y2 < height);
        assert(outlineWidth > 0);

        if(fill)
        {
            FillRectangle(x1, y1, x2, y2, fillColor);
        }
    	
	    DrawLine(x1, y1, x2, y1, outlineColor);
	    DrawLine(x2, y1, x2, y2, outlineColor);
	    DrawLine(x2, y2, x1, y2, outlineColor);
	    DrawLine(x1, y2, x1, y1, outlineColor);

        if (outlineWidth > 1) {
            DrawRectangle(x1+1, y1+1, x2 - 1, y2 - 1, outlineColor, outlineWidth - 1, fill, fillColor);
        }
    	
    }
	
    void Image::FillRectangle(int64_t x1, int64_t y1, int64_t x2, int64_t y2, const RGBColor& fillColor)
    {
	    while(x1 < x2 && y1  != y2)
	    {
            DrawLine(x1, y1, x2, y1, fillColor);
            DrawLine(x2, y1, x2, y2, fillColor);
            DrawLine(x2, y2, x1, y2, fillColor);
            DrawLine(x1, y2, x1, y1, fillColor);

	    	x1++; x2--;
            y1++; y2--;
	    }

    }
	
    void Image::SetFileName(const string& _outFileName)
    {
        assert(_outFileName.size() > 0);
        outFileName = _outFileName;
    }

    void Image::Write(const string& _outFileName)
    {
        SetFileName(_outFileName);
        Write();
    }

    void Image::Write()
    {

        outFile.open(outFileName, ofstream::binary);
        if (not outFile.is_open()) {
            std::cerr << "_BMP ERROR: Can't open file to write data." << std::endl;
            assert(false);
        }

        unsigned int headers[13];
        int paddedSize, extraBytes;

        extraBytes = 4 - ((width * 3) % 4);         // How many bytes of padding to add to each
                                                    // horizontal line - the size of which must
                                                    // be a multiple of 4 bytes.

        if (extraBytes == 4) {
            extraBytes = 0;
        }

        paddedSize = ((width * 3) + extraBytes) * height;

        // Headers...
        // Note that the "BM" identifier in bytes 0 and 1 is NOT included in these "headers".

        headers[0] = paddedSize + 54;      // bfSize (whole file size)
        headers[1] = 0;                    // bfReserved (both)
        headers[2] = 54;                   // bfOffbits
        headers[3] = 40;                   // biSize
        headers[4] = width;  // biWidth
        headers[5] = height; // biHeight

        // Would have biPlanes and biBitCount in position 6, but they're shorts.
        // It's easier to write them out separately (see below) than pretend
        // they're a single int, especially with endian issues...

        headers[7] = 0;                    // biCompression
        headers[8] = paddedSize;           // biSizeImage
        headers[9] = 0;                    // biXPelsPerMeter
        headers[10] = 0;                    // biYPelsPerMeter
        headers[11] = 0;                    // biClrUsed
        headers[12] = 0;                    // biClrImportant

        //
        // Headers begin...
        // When printing ints and shorts, we write out 1 character at a time to avoid endian issues.
        //

        outFile << 'B' << 'M';

        for (int i = 0; i <= 5; ++i) {
            outFile << (uint8_t)((headers[i] & 0x000000ff));
            outFile << (uint8_t)((headers[i] & 0x0000ff00) >> 8);
            outFile << (uint8_t)((headers[i] & 0x00ff0000) >> 16);
            outFile << (uint8_t)((headers[i] & (uint32_t)0xff000000) >> 24);
        }

        // These next 4 characters are for the biPlanes and biBitCount fields.

        outFile << (uint8_t)1 << (uint8_t)0 << (uint8_t)24 << (uint8_t)0;

        for (int i = 7; i <= 12; ++i) {
            outFile << (uint8_t)((headers[i] & 0x000000ff));
            outFile << (uint8_t)((headers[i] & 0x0000ff00) >> 8);
            outFile << (uint8_t)((headers[i] & 0x00ff0000) >> 16);
            outFile << (uint8_t)((headers[i] & (uint32_t)0xff000000) >> 24);
        }

        //
        // Headers done, now write the data...
        //

        uint8_t colorBuffer[3];

        // BMP image format is written from bottom to top...
        for (int64_t y = height - 1; y >= 0; --y) {
            for (int64_t x = 0; x < width; ++x) {
                // Also, it's written in (b,g,r) format...
                outFile << buffer[y][x].b << buffer[y][x].g << buffer[y][x].r;
            }
            // See above - BMP lines must be of lengths divisible by 4.
            if (extraBytes) {
                for (int i = 1; i <= extraBytes; ++i) {
                    outFile << (uint8_t)0;
                }
            }
        }

        outFile.close();
    }
}

#endif