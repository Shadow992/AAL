#include "Image.h"

#define SQUARE_OF_2 1.41421356237
#define NUM_1_DIVIDED_SQUARE_OF_2 0.70710678118

const unsigned int NORMAL_BMP_HEADER_SIZE=54;

Image::Image()
{

}

Image::Image(const std::string& imageFile)
{
    readAndParseBMPFile(imageFile);
}

Image::~Image()
{
    //dtor
    if(imgData!=nullptr)
    {
        delete[] imgData;
    }
}

void Image::captureWindow(const std::string& str)
{
    HWND hWnd = FindWindow((LPCSTR)str.c_str(),(LPCSTR)str.c_str());

    captureWindowFromHandle(hWnd);
}

void Image::captureWindowFromHandle(HWND hWnd)
{
    HDC hScreenDC  = GetDC(hWnd);
    int w = GetDeviceCaps(hScreenDC, HORZRES);
    int h = GetDeviceCaps(hScreenDC, VERTRES);

    HDC hdc2 = CreateCompatibleDC(hScreenDC);

    BITMAPINFO bmi;
    memset(&bmi, 0, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = w;
    bmi.bmiHeader.biHeight = h; // bottomUp
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;

    paddigPerLine=4-((3*w)%4);
    if(paddigPerLine==4)
    {
        paddigPerLine=0;
    }

    char* memblock;
    HBITMAP hBitmap  = CreateDIBSection(hdc2, &bmi, DIB_RGB_COLORS, (void**)&memblock, 0, 0);
    SelectObject(hdc2,hBitmap);
    BitBlt(hdc2,0,0,w,h,hScreenDC,0,0,SRCCOPY);


    if(imgData!=nullptr)
    {
        delete[] imgData;
    }

    imgData=new Pixel[w*h];
    parseBMPData((unsigned char*)memblock,w,h,w*h, true,24);

    ReleaseDC(hWnd,hScreenDC);
    DeleteObject(hBitmap);
    if (hdc2)
    {
        DeleteDC(hdc2);
    }
}

int Image::readAndParseBMPFile(const std::string& imageFile)
{
    std::streampos size;
    char * memblock;

    std::ifstream file (imageFile, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        memblock = new char[size];
        file.seekg (0, std::ios::beg);
        file.read (memblock, size);
        file.close();

        if(size>NORMAL_BMP_HEADER_SIZE)
        {
            if(memblock[0]=='B' && memblock[1]=='M' )
            {
                return parseBMPFile(memblock,size);
            }
            else
            {
                return -10;
                // no bmp
            }
        }
        else
        {
             return -11;
            // no bmp
        }


        delete[] memblock;
    }
    else
    {
         return -12;
        // Unable to open file
    }
}

int Image::parseBMPFile(char* memblock, unsigned int size)
{
    bool bottomUpImage=true;

    unsigned char* currData=(unsigned char*)memblock;

    currData+=10;
    unsigned int bmpOffset=*((unsigned int*)currData);
    currData+=8;
    int bmpWidth=*((int*)currData);
    currData+=4;
    int bmpHeight=*((int*)currData);
    if(bmpHeight<0)
    {
        bottomUpImage=false;
        bmpHeight=abs(bmpHeight);
    }
    currData+=4;
    while(*currData!=1)
    {
        currData++;
    }
    currData+=2;
    unsigned short bmpType=*((unsigned short*)currData); // 16, 24 or 32 bpp

    if(bmpType!=16 && bmpType!=24 && bmpType!=32)
    {
        bmpType=*((unsigned int*)currData); // 16, 24 or 32 bpp
        currData+=2;
        if(bmpType!=16 && bmpType!=24 && bmpType!=32)
        {
            return -1;
        }
    }
    currData+=2;
    unsigned int compression=*((unsigned int*)currData); // should be 0
    if(compression!=0)
    {
        return -2;
    }

    currData=(unsigned char*)memblock+bmpOffset;

    if(imgData!=nullptr)
    {
        delete[] imgData;
    }
    imgData=new Pixel[bmpWidth*bmpHeight];

    paddigPerLine=4-(((bmpType>>3)*bmpWidth)%4);
    if(paddigPerLine==4)
    {
        paddigPerLine=0;
    }

    unsigned int totalSize=bmpHeight*bmpWidth;

    parseBMPData(currData,bmpWidth,bmpHeight,totalSize,bottomUpImage,bmpType);

    return 0;
}

void Image::parseBMPData(unsigned char* currData, int bmpWidth, int bmpHeight, unsigned int totalSize, bool bottomUpImage,int bmpType)
{
    unsigned int idx=0;
    int x=0;
    int y=0;
    int addY=1;

    if(bottomUpImage==true)
    {
        y=bmpHeight-1;
        addY=-1;
    }

    while(1)
    {
        idx=x+y*bmpWidth;
        if(idx>=totalSize)
        {
            break;
        }
        if(bmpType==24)
        {
            imgData[idx].color.blue=*currData;
            currData++;
            imgData[idx].color.green=*currData;
            currData++;
            imgData[idx].color.red=*currData;
            currData++;
        }
        else if(bmpType==32)
        {
            imgData[idx].color.blue=*currData;
            currData++;
            imgData[idx].color.green=*currData;
            currData++;
            imgData[idx].color.red=*currData;
            currData++;
            imgData[idx].color.alphaChannel=*currData;
            currData++;
        }
        else if(bmpType==16)
        {
            imgData[idx].color.blue=*((unsigned int*)currData) & 0x0000001F;
            imgData[idx].color.green=*((unsigned int*)currData) & 0x000003E0;
            imgData[idx].color.red=*((unsigned int*)currData) & 0x00007C00;
            currData+=2;
        }
        imgData[idx].x=x;
        imgData[idx].y=y;

        x++;
        if(x>=bmpWidth)
        {
            y+=addY;
            x=0;
            currData+=paddigPerLine;
        }
    }

    width=bmpWidth;
    height=bmpHeight;
}

Pixel* Image::getPixel(int x, int y) const
{
    return &(imgData[x+y*width]);
}

void Image::setPixelColor(int x, int y, unsigned char red,unsigned char green,unsigned char blue)
{
    register int idx=x+y*width;
    imgData[idx].color.red=red;
    imgData[idx].color.green=green;
    imgData[idx].color.blue=blue;
}

void Image::setPixelColor(int x, int y, const Color& newPixel)
{
    setPixelColor(x, y, newPixel.red, newPixel.green, newPixel.blue);
}

Pixel* Image::searchForImage(int startX, int startY, const Image& searchImage, unsigned char variance)
{
    int searchImageWidth=searchImage.width;
    int searchImageHeight=searchImage.height;

    for (int y=0;y < height; y++)
    {
        for (int x=0;x < width; x++)
        {
            bool match=true;

            for (int searchY=0;searchY <searchImageHeight;searchY++)
            {
                for (int searchX=0;searchX < searchImageWidth;searchX++)
                {
                    if(x + searchX<width && y + searchY<height && getPixel(x + searchX, y + searchY)->colorEquals(searchImage.getPixel(searchX, searchY), variance)==false)
                    {
                        match=false;
                        break;
                    }
                }
                if(match==false)
                {
                    break;
                }
            }
            if(match==true)
            {
                return getPixel(x,y);
            }
        }
    }
    return nullptr;
}

bool Image::equalsImage(int x, int y, const Image& searchImage, unsigned char variance)
{
    for (int searchY=0;searchY <searchImage.height;searchY++)
    {
        for (int searchX=0;searchX < searchImage.width;searchX++)
        {
            if(x + searchX<width && y + searchY<height && getPixel(x + searchX, y + searchY)->colorEquals(searchImage.getPixel(searchX, searchY), variance)==false)
            {
                return false;
            }
        }
    }
    return true;
}

Pixel* Image::searchForImageNextToPosition(int x, int y, const Image& searchImage, unsigned char variance)
{
    for(int radius=0;;radius++)
    {
        if(y+radius>=height && y-radius<0 && x+radius>=width && x-radius<0)
            break;

        for (int nearestX=x-radius; nearestX <= x+radius; nearestX++)
        {
            if(y-radius>=0)
            {
                if(nearestX>=0 && nearestX<width && equalsImage(nearestX,y-radius,searchImage,variance)==true)
                {
                    return getPixel(nearestX,y-radius);
                }
            }

            if(y+radius<height)
            {
                if(nearestX>=0 && nearestX<width && equalsImage(nearestX,y+radius,searchImage,variance)==true)
                {

                    return getPixel(nearestX,y+radius);
                }
            }
        }

        for (int nearestY=y-radius; nearestY <= y+radius; nearestY++)
        {
            if(x+radius<width)
            {
                if(nearestY>=0 && nearestY<height && equalsImage(x+radius,nearestY,searchImage,variance)==true)
                {
                    return getPixel(x+radius,nearestY);
                }
            }

            if(x-radius>=0)
            {
                if(nearestY>=0 && nearestY<height && equalsImage(x-radius,nearestY,searchImage,variance)==true)
                {
                    return getPixel(x-radius,nearestY);
                }
            }
        }
    }

    return nullptr;
}

Pixel* Image::searchForPixel(int startX, int startY, const Color& searchPixel, unsigned char variance)
{
    return searchForPixel(startX, startY, searchPixel.red, searchPixel.green, searchPixel.blue,variance);
}

Pixel* Image::searchForPixel(int startX, int startY, unsigned char red,unsigned char green,unsigned char blue, unsigned char variance)
{
    for(int y=0;y<height;y++)
    {
        for(int x=0;x<width;x++)
        {
            if(getPixel(x,y)->colorEquals(red,green,blue,variance))
            {
                return getPixel(x,y);
            }
        }
    }
    return nullptr;
}

Pixel* Image::searchPixelNextToPosition(int x, int y, const Color& searchPixel, unsigned char variance)
{
    return searchPixelNextToPosition(x, y, searchPixel.red, searchPixel.green, searchPixel.blue, variance);
}

Pixel* Image::searchPixelNextToPosition(int x, int y, unsigned char red, unsigned char green, unsigned char blue, unsigned char variance)
{
    for(int radius=0;;radius++)
    {
        if(y+radius>=height && y-radius<0 && x+radius>=width && x-radius<0)
            break;

        for (int nearestX=x-radius; nearestX <= x+radius; nearestX++)
        {
            if(y-radius>=0)
            {
                if(nearestX>=0 && nearestX<width && getPixel(nearestX,y-radius)->colorEquals(red,green,blue,variance))
                {
                    return getPixel(nearestX,y-radius);
                }
            }

            if(y+radius<height)
            {
                if(nearestX>=0 && nearestX<width && getPixel(nearestX,y+radius)->colorEquals(red,green,blue,variance))
                {

                    return getPixel(nearestX,y+radius);
                }
            }
        }

        for (int nearestY=y-radius; nearestY <= y+radius; nearestY++)
        {
            if(x+radius<width)
            {
                if(nearestY>=0 && nearestY<height && getPixel(x+radius,nearestY)->colorEquals(red,green,blue,variance))
                {
                    return getPixel(x+radius,nearestY);
                }
            }

            if(x-radius>=0)
            {
                if(nearestY>=0 && nearestY<height && getPixel(x-radius,nearestY)->colorEquals(red,green,blue,variance))
                {
                    return getPixel(x-radius,nearestY);
                }
            }
        }
    }

    return nullptr;
}

void Image::calcHistogram(int accuracy,int xStart, int yStart, int w, int h)
{
    if(accuracy<=4)
    {
        histogramAccuracy=2;
    }
    else if(accuracy<=8)
    {
        histogramAccuracy=3;
    }
    else if(accuracy<=16)
    {
        histogramAccuracy=4;
    }
    else
    {
        histogramAccuracy=5;
    }

    if(w==-1)
    {
        w=width-xStart;
    }

    if(h==-1)
    {
        h=height-yStart;
    }

    if(yStart+h>height)
    {
        h=height-yStart;
    }

    if(w+xStart>width)
    {
        w=width-xStart;
    }

    int sizeOfHistogram=((1<<histogramAccuracy)<<histogramAccuracy)<<histogramAccuracy;
    histogram.resize(sizeOfHistogram);
    for(int i=0;i<sizeOfHistogram;i++)
    {
        histogram[i]=0;
    }

    int toShift=8-histogramAccuracy;
    int blueShift=histogramAccuracy+histogramAccuracy;

    for(int y=yStart;y<yStart+h;y++)
    {
        for(int x=xStart;x<xStart+w;x++)
        {
            Pixel* p=getPixel(x,y);
            int redBins=p->color.red>>toShift;
            int greenBins=(p->color.green>>toShift)<<histogramAccuracy;
            int blueBins=(p->color.blue>>toShift)<<blueShift;

            histogram[redBins + greenBins + blueBins]++;
        }
    }

    float sampledPixels=w*h;

    for(int i=0;i<sizeOfHistogram;i++)
    {
        if(histogram[i]!=0)
        {
            histogram[i]=sqrt(histogram[i]/sampledPixels);
        }
    }

    histogramWidth=w;
    histogramHeight=h;
}

float Image::checkHistogramDifferences(Image& checkImage)
{
    int sizeOfHistogram=((1<<histogramAccuracy)<<histogramAccuracy)<<histogramAccuracy;

    // use Hellinger distance to calculate differences of 2 given datasets
    float differences=0.0;
    for(int i=0;i<sizeOfHistogram;i++)
    {
        float squareRootDif=histogram[i] - checkImage.histogram[i];
        squareRootDif*=squareRootDif;
        differences+=squareRootDif;
    }
    differences=sqrt(differences)/SQUARE_OF_2;

    return differences;
}

float Image::findSimilarImageHillClimbing(int startX, int startY,int w, int h,Image& checkImage, int accuracy, float overlap, int &bestX, int &bestY)
{
    if(w==-1)
    {
        w=width;
    }

    if(h==-1)
    {
        h=height;
    }

    int widthToCheck=checkImage.width;
    int heightToCheck=checkImage.height;
    if(checkImage.histogramHeight!=checkImage.height || checkImage.histogramWidth!=checkImage.width)
    {
        checkImage.calcHistogram(accuracy,0,0,-1,-1);
    }

    int freeSpaceX=(float)checkImage.histogramWidth/overlap;
    int freeSpaceY=(float)checkImage.height/overlap;

	float currFitness=1000000000000000;
    float bestFitness=100000;

    for(int y=startY+freeSpaceY;y<h;y+=freeSpaceY)
    {
        for(int x=freeSpaceX+startX;x<w;x+=freeSpaceX)
        {
            // calculate fitness
            calcHistogram(accuracy,x,y,widthToCheck,heightToCheck);
            currFitness=checkHistogramDifferences(checkImage);

            if(bestFitness>currFitness)
            {
                bestFitness=currFitness;
                bestX=x;
                bestY=y;
            }
        }
    }

    findBetterSolution(bestX,bestY,w,h,checkImage, accuracy, bestX, bestY, bestFitness);

    return bestFitness;
}

void Image::findBetterSolution(int x, int y,int w, int h,Image& checkImage, int accuracy, int &bestX, int &bestY, float& fitness)
{
    int widthToCheck=checkImage.width;
    int heightToCheck=checkImage.height;
    float currFitness=111111;
    bool foundBetter=true;
    int radius=1;

    while(foundBetter==true)
    {
        foundBetter=false;
        if(y+radius>=h && y-radius<0 && x+radius>=w && x-radius<0)
            break;

        for (int nearestX=x-radius; nearestX <= x+radius; nearestX++)
        {
            if(y-radius>=0)
            {
                if(nearestX>=0 && nearestX<w)
                {
                    calcHistogram(accuracy,nearestX,y-radius,widthToCheck,heightToCheck);
                    currFitness=checkHistogramDifferences(checkImage);
                    if(currFitness<fitness)
                    {
                        fitness=currFitness;
                        bestX=nearestX;
                        bestY=y-radius;
                        x=bestX;
                        y=bestY;
                        foundBetter=true;
                    }
                }
            }

            if(y+radius<h)
            {
                if(nearestX>=0 && nearestX<w)
                {
                    calcHistogram(accuracy,nearestX,y+radius,widthToCheck,heightToCheck);
                    currFitness=checkHistogramDifferences(checkImage);
                    if(currFitness<fitness)
                    {
                        fitness=currFitness;
                        bestX=nearestX;
                        bestY=y+radius;
                        x=bestX;
                        y=bestY;
                        foundBetter=true;
                    }
                }
            }
        }

        for (int nearestY=y-radius; nearestY <= y+radius; nearestY++)
        {
            if(x+radius<w)
            {
                if(nearestY>=0 && nearestY<h)
                {
                    calcHistogram(accuracy,x+radius,nearestY,widthToCheck,heightToCheck);
                    currFitness=checkHistogramDifferences(checkImage);
                    if(currFitness<fitness)
                    {
                        fitness=currFitness;
                        bestX=x+radius;
                        bestY=nearestY;
                        x=bestX;
                        y=bestY;
                        foundBetter=true;
                    }
                }
            }

            if(x-radius>=0)
            {
                if(nearestY>=0 && nearestY<h)
                {
                    calcHistogram(accuracy,x-radius,nearestY,widthToCheck,heightToCheck);
                    currFitness=checkHistogramDifferences(checkImage);
                    if(currFitness<fitness)
                    {
                        fitness=currFitness;
                        bestX=x-radius;
                        bestY=nearestY;
                        x=bestX;
                        y=bestY;
                        foundBetter=true;
                    }
                }
            }
        }
    }
}

float Image::findSimilarImageEA(int x, int y,int w, int h,Image& checkImage, int accuracy, int agentsUsed, int iterations,int &bestX, int &bestY)
{
    if(w==-1)
    {
        w=width;
    }

    if(h==-1)
    {
        h=height;
    }

    int maxX=x+w;
    int maxY=y+h;

    if(maxX>=width)
    {
        maxX=width;
    }

    if(maxY>=height)
    {
        maxY=height;
    }

    int widthToCheck=checkImage.width;
    int heightToCheck=checkImage.height;

    int doubledWidthToCheck=widthToCheck*2;
    int doubledHeightToCheck=heightToCheck*2;

    if(checkImage.histogramHeight!=checkImage.height || checkImage.histogramWidth!=checkImage.width)
    {
        checkImage.calcHistogram(accuracy,0,0,-1,-1);
    }

	std::vector<Agent> agents;
	agents.resize(agentsUsed);

    for(int i=0;i<agentsUsed;i++)
    {
        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        agents[i].x=x+w*r;
        r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        agents[i].y=y+h*r;
    }


    int bestAgentsToleave=agentsUsed/8;
    for(;iterations>0;iterations--)
    {
        for(int i=0;i<agentsUsed;i++)
        {
            // Mutation with 33% possibility
            if(rand()%3==0)
            {
                if(rand()%10==0)
                {
                    agents[i].mutateSwap(x,y,maxX,maxY);
                }
                else
                {
                    agents[i].mutateRandomPixel(x,y,maxX,maxY);
                }
                agents[i].recalculationNeeded=true;
            }

            // calculate fitness
            if(agents[i].recalculationNeeded==true)
            {
                calcHistogram(accuracy,agents[i].x,agents[i].y,widthToCheck,heightToCheck);
                agents[i].currFitness=checkHistogramDifferences(checkImage);
            }

        }

        std::sort(agents.begin(),agents.end());

        for(int i=bestAgentsToleave;i<agentsUsed;i++)
        {
            if((i+1)<rand()%agentsUsed)
            {
                int ran=rand()%4;
                if(ran==0)
                {
                    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                    agents[i].x=x+w*r;
                    r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                    agents[i].y=y+h*r;
                }
                else if(ran==1)
                {
                    int ran1=rand()%agentsUsed;
                    int ran2=rand()%agentsUsed;
                    agents[i].x=(agents[ran1].x+agents[ran2].x)>>1;
                    agents[i].y=(agents[ran1].y+agents[ran2].y)>>1;
                }
                else
                {
                    int ran1=rand()%agentsUsed;

                    agents[i].x=agents[ran1].x+rand()%doubledWidthToCheck-widthToCheck;
                    agents[i].y=agents[ran1].y+rand()%doubledHeightToCheck-heightToCheck;
                    agents[i].checkXYValues(x,y,maxX,maxY);
                }
                agents[i].recalculationNeeded=true;
            }
        }
    }

    std::sort(agents.begin(),agents.end());

    bestX=agents[0].x;
    bestY=agents[0].y;

    findBetterSolution(bestX,bestY,w,h,checkImage, accuracy, bestX, bestY, agents[0].currFitness);

    return agents[0].currFitness;
}

void Image::saveAsBMP(const std::string& fileName)
{
    std::ofstream bmpFile;
    bmpFile.open(fileName, std::ios::binary | std::ios::out | std::ios::trunc);
    int tmpInt;
    if (bmpFile.is_open())
    {
        bmpFile << 'B'<< 'M';
        tmpInt=width*height*3+paddigPerLine*height+54;
        bmpFile.write((char*) &tmpInt,4);
        tmpInt=0;
        bmpFile.write((char*) &tmpInt,4);
        tmpInt=54;
        bmpFile.write((char*) &tmpInt,4);
        tmpInt=40;
        bmpFile.write((char*) &tmpInt,4);
        tmpInt=width;
        bmpFile.write((char*) &tmpInt,4);
        tmpInt=-height;
        bmpFile.write((char*) &tmpInt,4);
        tmpInt=1;
        bmpFile.write((char*) &tmpInt,2);
        tmpInt=24;
        bmpFile.write((char*) &tmpInt,2);
        tmpInt=0;
        bmpFile.write((char*) &tmpInt,4);
        tmpInt=width*height*3+paddigPerLine*height;
        bmpFile.write((char*) &tmpInt,4);
        tmpInt=0;
        bmpFile.write((char*) &tmpInt,4);
        tmpInt=0;
        bmpFile.write((char*) &tmpInt,4);
        tmpInt=0;
        bmpFile.write((char*) &tmpInt,4);
        tmpInt=0;
        bmpFile.write((char*) &tmpInt,4);

        std::vector<unsigned char> imageData(width*height*3+paddigPerLine*height+5);

        int overallPos=0;
        unsigned char* imageDataPtr=imageData.data();
        for(int y=0;y<height;y++)
        {
            for(int x=0;x<width;x++)
            {
                memcpy(imageDataPtr,(unsigned char*) &(imgData[overallPos].color),3);
                overallPos++;
                imageDataPtr+=3;
            }

            for(unsigned int i=0;i<paddigPerLine;i++)
            {
                *imageDataPtr=0;
                imageDataPtr++;
            }
        }

        bmpFile.write((char*) imageData.data(),width*height*3+paddigPerLine*height);

        bmpFile.flush();
        bmpFile.close();
    }
}
