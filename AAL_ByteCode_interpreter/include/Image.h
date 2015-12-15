#ifndef IMAGE_H
#define IMAGE_H
#include <iostream>
#include <fstream>
#include <iomanip>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <vector>
#include <cmath>
#include <algorithm>

struct Agent
{
    int x=0;
    int y=0;
    float currFitness=100000000.0;
    bool recalculationNeeded=true;

    void mutateRandomPixel(int minX, int minY, int maxX, int maxY)
    {
        x+=rand()%9-4;
        y+=rand()%9-4;
        if(x<minX)
        {
            x=minX+1;
        }

        if(y<minY)
        {
            y=minY+1;
        }

        if(x>=maxX)
        {
            x=maxX-1;
        }

        if(y>=maxY)
        {
            y=maxY-1;
        }
    }

    void mutateSwap(int minX, int minY, int maxX, int maxY)
    {
        int t=x;
        x=y;
        y=t;

        if(x<minX)
        {
            x=minX+1;
        }

        if(y<minY)
        {
            y=minY+1;
        }

        if(x>=maxX)
        {
            x=maxX-1;
        }

        if(y>=maxY)
        {
            y=maxY-1;
        }
    }

    void checkXYValues(int minX, int minY, int maxX, int maxY)
    {
        if(x<minX)
        {
            x=minX+1;
        }

        if(y<minY)
        {
            y=minY+1;
        }

        if(x>=maxX)
        {
            x=maxX-1;
        }

        if(y>=maxY)
        {
            y=maxY-1;
        }
    }

    bool operator < (const Agent& str) const
    {
        return (currFitness < str.currFitness);
    }
};

struct Color
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;

    unsigned char alphaChannel; //unused at the moment

    bool equals(const Color& a, const unsigned char variance) const
    {
        return equals(a.red,a.green,a.blue,variance);
    }

    bool equals(unsigned char r,unsigned char g,unsigned char b, const unsigned char variance) const
    {
        return abs(red-r) <= variance && abs(blue-b) <= variance && abs(green-g) <= variance;
    }
};

struct Pixel
{
    int x;
    int y;
    Color color;

    bool colorEquals(const Pixel* a, const unsigned char variance) const
    {
        return color.equals(a->color,variance);
    }

    bool colorEquals(const Color& a, const unsigned char variance) const
    {
        return color.equals(a,variance);
    }

    bool colorEquals(unsigned char r,unsigned char g,unsigned char b, const unsigned char variance) const
    {
        return color.equals(r, g, b, variance);
    }
};

class Image
{
    public:
        Image();
        Image(const std::string& imageFile);
        virtual ~Image();

        int readAndParseBMPFile(const std::string& imageFile);
        int parseBMPFile(char* memblock, unsigned int size);
        void parseHBitmap(HDC hdcWindow, HBITMAP hBmp);
        void parseBMPData(unsigned char* currData, int bmpWidth, int bmpHeight, unsigned int totalSize, bool bottomUpImage,int bmpType);
        void captureWindow(const std::string& str);
        void captureWindowFromHandle(HWND hWnd);
        void saveAsBMP(const std::string& fileName);

        Pixel* getPixel(int x, int y) const;

        void setPixelColor(int x, int y, unsigned char red,unsigned char green,unsigned char blue);
        void setPixelColor(int x, int y, const Color& newPixel);

        Pixel* searchForImage(int startX, int startY, const Image& searchImage, unsigned char variance);
        bool equalsImage(int x, int y, const Image& searchImage, unsigned char variance);

        Pixel* searchForPixel(int startX, int startY, const Color& searchPixel, unsigned char variance);
        Pixel* searchForPixel(int startX, int startY, unsigned char red,unsigned char green,unsigned char blue, unsigned char variance);

        Pixel* searchPixelNextToPosition(int x, int y, const Color& searchPixel, unsigned char variance);
        Pixel* searchPixelNextToPosition(int x, int y, unsigned char red,unsigned char green,unsigned char blue, unsigned char variance);
        Pixel* searchForImageNextToPosition(int x, int y, const Image& searchImage, unsigned char variance);

        void calcHistogram(int accuracy,int xStart, int yStart, int w, int h);
        float checkHistogramDifferences(Image& checkImage);
        float findSimilarImageHillClimbing(int startX, int startY,int w, int h,Image& checkImage, int accuracy , float overlap, int &bestX, int &bestY);
        float findSimilarImageEA(int x, int y,int w, int h,Image& checkImage, int accuracy, int agentsUsed, int iterations,int &bestX, int &bestY);
        void findBetterSolution(int x, int y,int w, int h,Image& checkImage, int accuracy, int &bestX, int &bestY, float& fitness);

        Pixel* imgData=nullptr;
        int width;
        int height;
        int histogramAccuracy;
        int histogramWidth;
        int histogramHeight;
        unsigned int paddigPerLine;
        std::vector<float> histogram;
    protected:
    private:
};

#endif // IMAGE_H
