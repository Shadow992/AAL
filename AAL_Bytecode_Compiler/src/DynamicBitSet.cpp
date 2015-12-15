#include "DynamicBitSet.h"
#include "LookUpTable.hpp"


DynamicBitSet::DynamicBitSet(uint64_t length)
{
    setUp(length);
}

DynamicBitSet::DynamicBitSet(const std::vector<uint64_t>& bits)
{
    setUp(bits);
}

DynamicBitSet::DynamicBitSet(const uint64_t* bits, int_fast32_t length)
{
    setUp(bits,length);
}

void DynamicBitSet::setUp(uint64_t length)
{
    if(element!=nullptr)
        delete[] element;

    cBits=length;
    cElements=length/sizeOfElement+1;
    element=new uint64_t[cElements];
    int_fast32_t bitsLeft=sizeOfElement-((cElements*sizeOfElement)-cBits);
    clearMask=0;
    for(int_fast32_t i=0; i<bitsLeft; i++)
    {
        clearMask|=(1LL<<i);
    }
    reset();
	currBitToSet=length;
}

void DynamicBitSet::setUp(const std::vector<uint64_t>& bits)
{
    if(element!=nullptr)
        delete[] element;

    int_fast32_t length=bits.size()*sizeOfElement;
    cBits=length;
    cElements=bits.size()+1;
    element=new uint64_t[cElements];
    int_fast32_t bitsLeft=sizeOfElement-((cElements*sizeOfElement)-cBits);
    clearMask=0;
    for(int_fast32_t i=0; i<bitsLeft; i++)
    {
        clearMask|=(1LL<<i);
    }

    for(int_fast32_t i=0; i<cElements-1; i++)
    {
        element[i]=bits[i];
    }
	currBitToSet=cBits;
}

void DynamicBitSet::setUp(const uint64_t* bits, int_fast32_t length)
{
    if(element!=nullptr)
        delete[] element;

    cBits=length*sizeOfElement;
    cElements=length+1;
    element=new uint64_t[cElements];
    int_fast32_t bitsLeft=sizeOfElement-((cElements*sizeOfElement)-cBits);
    clearMask=0;
    for(int_fast32_t i=0; i<bitsLeft; i++)
    {
        clearMask|=(1LL<<i);
    }

    for(int_fast32_t i=0; i<length; i++)
    {
        element[i]=bits[i];
    }
	currBitToSet=cElements;
}

DynamicBitSet::~DynamicBitSet()
{
    delete[] element;
}

DynamicBitSet::DynamicBitSet(const DynamicBitSet& other)
{
    *this=other;
}

void DynamicBitSet::resize(uint_fast32_t newSize)
{
    element[cElements-1]&=clearMask;
    if(cBits!=newSize)
    {
        cBits=newSize;

        int_fast32_t cTmpElements=cBits/sizeOfElement+1;
        uint64_t* newElementArray=new uint64_t[cTmpElements];

        for(int_fast32_t i=0; i<cElements && i<cTmpElements; i++)
        {
            newElementArray[i]=element[i];
        }

        delete[] element;
        element=newElementArray;

        cElements=cTmpElements;
        int_fast32_t bitsLeft=sizeOfElement-((cElements*sizeOfElement)-cBits);
        clearMask=0;
        for(int_fast32_t i=0; i<bitsLeft; i++)
        {
            clearMask|=(1LL<<i);
        }
        lazyCount=true;
		currBitToSet=cBits;
    }
}

void DynamicBitSet::reset()
{
    lazyCount=true;
    for(int_fast32_t i=0; i<cElements; i++)
    {
        element[i]=0LL;
    }
}

void DynamicBitSet::set()
{
    lazyCount=true;
    for(int_fast32_t i=0; i<cElements; i++)
    {
        element[i]=0xFFFFFFFFFFFFFFFFLL;
    }
}

uint_fast32_t DynamicBitSet::getSetBitCount()
{
    if(lazyCount==0)
        return countSetBits;
    lazyCount=false;
    element[cElements-1]&=clearMask;
    uint64_t total=0;
    uint64_t n;
    for(int_fast32_t i=0; i<cElements; i++)
    {
        n=element[i];
        register uint64_t tmp;
        tmp = n - ((n >> 1LL) & 0x7777777777777777LL)
              - ((n >> 2LL) & 0x3333333333333333LL)
              - ((n >> 3LL) & 0x1111111111111111LL);
        total+=((tmp + (tmp >> 4) ) & 0x0F0F0F0F0F0F0F0FLL) % 255LL;
    }
    countSetBits=total;
    return (unsigned int)total;
}

int_fast32_t DynamicBitSet::getLeftestSetBitIndex(uint_fast32_t idxStart)
{
    element[cElements-1]&=clearMask;
    uint64_t n=0;
    uint_fast32_t i=0;
    uint_fast32_t tmp=0;

    uint_fast32_t idxElement=idxStart/sizeOfElement;
    uint_fast32_t idxBit=idxStart%sizeOfElement;
    uint_fast32_t shiftLeft=sizeOfElement-idxBit;

    uint64_t oldElement=element[idxElement];
    element[idxElement]=element[idxElement]&((0xFFFFFFFFFFFFFFFFLL<<shiftLeft)>>shiftLeft);


    for(i=idxElement; i>=0; i--)
    {
        if(element[i]!=0)
        {
            n=element[i];
            break;
        }
    }

    if(n==0LL)
        return -1;

    if (n & 0xFFFFFFFF00000000)
    {
        if (n & 0xFFFF000000000000)
        {
            if (n & 0xFF00000000000000)
            {
                if (n & 0xF000000000000000)
                {
                    if (n & 0xC000000000000000)
                        tmp= (n & 0x8000000000000000) ? 63 : 62;
                    else
                        tmp=  (n & 0x2000000000000000) ? 61 : 60;
                }
                else
                {
                    if (n & 0x0C00000000000000)
                        tmp=  (n & 0x0800000000000000) ? 59 : 58;
                    else
                        tmp=  (n & 0x0200000000000000) ? 57 : 56;
                }
            }
            else
            {
                if (n & 0x00F0000000000000)
                {
                    if (n & 0x00C0000000000000)
                        tmp=  (n & 0x0080000000000000) ? 55 : 54;
                    else
                        tmp=  (n & 0x0020000000000000) ? 53 : 52;
                }
                else
                {
                    if (n & 0x000C000000000000)
                        tmp=  (n & 0x0008000000000000) ? 51 : 50;
                    else
                        tmp=  (n & 0x0002000000000000) ? 49 : 48;
                }
            }
        }
        else
        {
            if (n & 0x0000FF0000000000)
            {
                if (n & 0x0000F00000000000)
                {
                    if (n & 0x0000C00000000000)
                        tmp=  (n & 0x0000800000000000) ? 47 : 46;
                    else
                        tmp=  (n & 0x0000200000000000) ? 45 : 44;
                }
                else
                {
                    if (n & 0x00000C0000000000)
                        tmp=  (n & 0x0000080000000000) ? 43 : 42;
                    else
                        tmp=  (n & 0x0000020000000000) ? 41 : 40;
                }
            }
            else
            {
                if (n & 0x000000F000000000)
                {
                    if (n & 0x000000C000000000)
                        tmp=  (n & 0x0000008000000000) ? 39 : 38;
                    else
                        tmp=  (n & 0x0000002000000000) ? 37 : 36;
                }
                else
                {
                    if (n & 0x0000000C00000000)
                        tmp=  (n & 0x0000000800000000) ? 35 : 34;
                    else
                        tmp=  (n & 0x0000000200000000) ? 33 : 32;
                }
            }
        }
    }
    else
    {
        if (n & 0x00000000FFFF0000)
        {
            if (n & 0x00000000FF000000)
            {
                if (n & 0x00000000F0000000)
                {
                    if (n & 0x00000000C0000000)
                        tmp=  (n & 0x0000000080000000) ? 31 : 30;
                    else
                        tmp=  (n & 0x0000000020000000) ? 29 : 28;
                }
                else
                {
                    if (n & 0x000000000C000000)
                        tmp=  (n & 0x0000000008000000) ? 27 : 26;
                    else
                        tmp=  (n & 0x0000000002000000) ? 25 : 24;
                }
            }
            else
            {
                if (n & 0x0000000000F00000)
                {
                    if (n & 0x0000000000C00000)
                        tmp=  (n & 0x0000000000800000) ? 23 : 22;
                    else
                        tmp=  (n & 0x0000000000200000) ? 21 : 20;
                }
                else
                {
                    if (n & 0x00000000000C0000)
                        tmp=  (n & 0x0000000000080000) ? 19 : 18;
                    else
                        tmp=  (n & 0x0000000000020000) ? 17 : 16;
                }
            }
        }
        else
        {
            if (n & 0x000000000000FF00)
            {
                if (n & 0x000000000000F000)
                {
                    if (n & 0x000000000000C000)
                        tmp=  (n & 0x0000000000008000) ? 15 : 14;
                    else
                        tmp=  (n & 0x0000000000002000) ? 13 : 12;
                }
                else
                {
                    if (n & 0x0000000000000C00)
                        tmp=  (n & 0x0000000000000800) ? 11 : 10;
                    else
                        tmp=  (n & 0x0000000000000200) ? 9 : 8;
                }
            }
            else
            {
                if (n & 0x00000000000000F0)
                {
                    if (n & 0x00000000000000C0)
                        tmp=  (n & 0x0000000000000080) ? 7 : 6;
                    else
                        tmp=  (n & 0x0000000000000020) ? 5 : 4;
                }
                else
                {
                    if (n & 0x000000000000000C)
                        tmp=  (n & 0x0000000000000008) ? 3 : 2;
                    else
                        tmp=  (n & 0x0000000000000002) ? 1 : 0;
                }
            }
        }
    }

    tmp+=i*sizeOfElement;

    element[idxElement]=oldElement;

    if(tmp<cBits)
        return tmp;
    return -1;
}

int_fast32_t DynamicBitSet::getLeftestSetBitIndex()
{
    element[cElements-1]&=clearMask;
    uint64_t n=0;
    uint_fast32_t i=0;
    uint_fast32_t tmp=0;

    for(i=cElements-1; i>=0; i--)
    {
        if(element[i]!=0)
        {
            n=element[i];
            break;
        }
    }

    if(n==0LL)
        return -1;

    if (n & 0xFFFFFFFF00000000)
    {
        if (n & 0xFFFF000000000000)
        {
            if (n & 0xFF00000000000000)
            {
                if (n & 0xF000000000000000)
                {
                    if (n & 0xC000000000000000)
                        tmp= (n & 0x8000000000000000) ? 63 : 62;
                    else
                        tmp=  (n & 0x2000000000000000) ? 61 : 60;
                }
                else
                {
                    if (n & 0x0C00000000000000)
                        tmp=  (n & 0x0800000000000000) ? 59 : 58;
                    else
                        tmp=  (n & 0x0200000000000000) ? 57 : 56;
                }
            }
            else
            {
                if (n & 0x00F0000000000000)
                {
                    if (n & 0x00C0000000000000)
                        tmp=  (n & 0x0080000000000000) ? 55 : 54;
                    else
                        tmp=  (n & 0x0020000000000000) ? 53 : 52;
                }
                else
                {
                    if (n & 0x000C000000000000)
                        tmp=  (n & 0x0008000000000000) ? 51 : 50;
                    else
                        tmp=  (n & 0x0002000000000000) ? 49 : 48;
                }
            }
        }
        else
        {
            if (n & 0x0000FF0000000000)
            {
                if (n & 0x0000F00000000000)
                {
                    if (n & 0x0000C00000000000)
                        tmp=  (n & 0x0000800000000000) ? 47 : 46;
                    else
                        tmp=  (n & 0x0000200000000000) ? 45 : 44;
                }
                else
                {
                    if (n & 0x00000C0000000000)
                        tmp=  (n & 0x0000080000000000) ? 43 : 42;
                    else
                        tmp=  (n & 0x0000020000000000) ? 41 : 40;
                }
            }
            else
            {
                if (n & 0x000000F000000000)
                {
                    if (n & 0x000000C000000000)
                        tmp=  (n & 0x0000008000000000) ? 39 : 38;
                    else
                        tmp=  (n & 0x0000002000000000) ? 37 : 36;
                }
                else
                {
                    if (n & 0x0000000C00000000)
                        tmp=  (n & 0x0000000800000000) ? 35 : 34;
                    else
                        tmp=  (n & 0x0000000200000000) ? 33 : 32;
                }
            }
        }
    }
    else
    {
        if (n & 0x00000000FFFF0000)
        {
            if (n & 0x00000000FF000000)
            {
                if (n & 0x00000000F0000000)
                {
                    if (n & 0x00000000C0000000)
                        tmp=  (n & 0x0000000080000000) ? 31 : 30;
                    else
                        tmp=  (n & 0x0000000020000000) ? 29 : 28;
                }
                else
                {
                    if (n & 0x000000000C000000)
                        tmp=  (n & 0x0000000008000000) ? 27 : 26;
                    else
                        tmp=  (n & 0x0000000002000000) ? 25 : 24;
                }
            }
            else
            {
                if (n & 0x0000000000F00000)
                {
                    if (n & 0x0000000000C00000)
                        tmp=  (n & 0x0000000000800000) ? 23 : 22;
                    else
                        tmp=  (n & 0x0000000000200000) ? 21 : 20;
                }
                else
                {
                    if (n & 0x00000000000C0000)
                        tmp=  (n & 0x0000000000080000) ? 19 : 18;
                    else
                        tmp=  (n & 0x0000000000020000) ? 17 : 16;
                }
            }
        }
        else
        {
            if (n & 0x000000000000FF00)
            {
                if (n & 0x000000000000F000)
                {
                    if (n & 0x000000000000C000)
                        tmp=  (n & 0x0000000000008000) ? 15 : 14;
                    else
                        tmp=  (n & 0x0000000000002000) ? 13 : 12;
                }
                else
                {
                    if (n & 0x0000000000000C00)
                        tmp=  (n & 0x0000000000000800) ? 11 : 10;
                    else
                        tmp=  (n & 0x0000000000000200) ? 9 : 8;
                }
            }
            else
            {
                if (n & 0x00000000000000F0)
                {
                    if (n & 0x00000000000000C0)
                        tmp=  (n & 0x0000000000000080) ? 7 : 6;
                    else
                        tmp=  (n & 0x0000000000000020) ? 5 : 4;
                }
                else
                {
                    if (n & 0x000000000000000C)
                        tmp=  (n & 0x0000000000000008) ? 3 : 2;
                    else
                        tmp=  (n & 0x0000000000000002) ? 1 : 0;
                }
            }
        }
    }

    tmp+=i*sizeOfElement;

    if(tmp<cBits)
        return tmp;
    return -1;
}

int_fast32_t DynamicBitSet::getRightestSetBitIndex()
{
    element[cElements-1]&=clearMask;
    uint_fast32_t c;
    uint64_t v=0;
    int_fast32_t i=0;

    for(i=0; i<cElements; i++)
    {
        if(element[i]!=0)
        {
            v=element[i];
            break;
        }
    }

    if (v & 0x1LL)
    {
        c = 0;
    }
    else
    {
        c = 1;
        if ((v & 0xffffffffLL) == 0)
        {
            v >>= 32LL;
            c += 32;
        }
        if ((v & 0xffffLL) == 0)
        {
            v >>= 16LL;
            c += 16;
        }
        if ((v & 0xffLL) == 0)
        {
            v >>= 8LL;
            c += 8;
        }
        if ((v & 0xfLL) == 0)
        {
            v >>= 4LL;
            c += 4;
        }
        if ((v & 0x3LL) == 0)
        {
            v >>= 2LL;
            c += 2;
        }
        c -= v & 0x1LL;
    }
    uint_fast32_t tmp=c+i*sizeOfElement;
    if(tmp<cBits)
        return tmp;
    return -1;
}

int_fast32_t DynamicBitSet::getRightestSetBitIndex(uint_fast32_t idxStart)
{
    idxStart++;
    element[cElements-1]&=clearMask;
    uint_fast32_t c;
    uint64_t v=0;
    int_fast32_t i=0;

    uint_fast32_t idxElement=idxStart/sizeOfElement;
    uint_fast32_t idxBit=idxStart%sizeOfElement;
    uint_fast32_t shiftRight=idxBit;

    uint64_t oldElement=element[idxElement];
    element[idxElement]=element[idxElement]&((0xFFFFFFFFFFFFFFFFLL>>shiftRight)<<shiftRight);

    for(i=idxElement; i<cElements; i++)
    {
        if(element[i]!=0)
        {
            v=element[i];
            break;
        }
    }

    if(v==0)
        return -1;

    if (v & 0x1LL)
    {
        c = 0;
    }
    else
    {
        c = 1;
        if ((v & 0xffffffffLL) == 0)
        {
            v >>= 32LL;
            c += 32;
        }
        if ((v & 0xffffLL) == 0)
        {
            v >>= 16LL;
            c += 16;
        }
        if ((v & 0xffLL) == 0)
        {
            v >>= 8LL;
            c += 8;
        }
        if ((v & 0xfLL) == 0)
        {
            v >>= 4LL;
            c += 4;
        }
        if ((v & 0x3LL) == 0)
        {
            v >>= 2LL;
            c += 2;
        }
        c -= v & 0x1LL;
    }

    element[idxElement]=oldElement;

    uint_fast32_t tmp=c+i*sizeOfElement;
    if(tmp<cBits)
        return tmp;
    return -1;
}

void DynamicBitSet::set(uint_fast32_t idx,bool val)
{
    lazyCount=true;

    if(val==true)
        element[idx/sizeOfElement]|=(1LL<<(idx%sizeOfElement));
    else
        element[idx/sizeOfElement]&=~(1LL<<(idx%sizeOfElement));
}

void DynamicBitSet::setRange(uint_fast32_t idxFrom,uint_fast32_t idxTo,bool val)
{
    lazyCount=true;
    uint_fast32_t idxElementFrom=idxFrom/sizeOfElement;
    uint_fast32_t idxElementTo=idxTo/sizeOfElement;
    uint_fast32_t idxBitFrom=(idxFrom%sizeOfElement);
    uint_fast32_t idxBitTo=(idxTo%sizeOfElement);

    if(val==true)
    {
        if(idxElementFrom==idxElementTo)
        {
            for(uint_fast32_t i=idxBitFrom; i<idxElementTo; i++)
            {
                element[idxElementFrom]|=(1LL<<i);
            }
        }
        else
        {
            for(uint_fast32_t i=idxBitFrom; i<sizeOfElement; i++)
            {
                element[idxElementFrom]|=(1LL<<i);
            }

            for(uint_fast32_t i=0; i<=idxBitTo; i++)
            {
                element[idxElementTo]|=(1LL<<i);
            }

            for(uint_fast32_t i=idxElementFrom+1; i<idxElementTo; i++)
            {
                element[i]=0xFFFFFFFFFFFFFFFFLL;
            }

        }
    }
    else
    {
        if(idxElementFrom==idxElementTo)
        {
            for(uint_fast32_t i=idxBitFrom; i<idxElementTo; i++)
            {
                element[idxElementFrom]&=~(1LL<<i);
            }
        }
        else
        {
            for(uint_fast32_t i=idxBitFrom; i<sizeOfElement; i++)
            {
                element[idxElementFrom]&=~(1LL<<i);
            }

            for(uint_fast32_t i=0; i<=idxBitTo; i++)
            {
                element[idxElementTo]&=~(1LL<<i);
            }

            for(uint_fast32_t i=idxElementFrom+1; i<idxElementTo; i++)
            {
                element[i]=0x0000000000000000LL;
            }
        }
    }
}

DynamicBitSet& DynamicBitSet::operator=(const DynamicBitSet& rhs)
{
    lazyCount=true;
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    if(cBits!=rhs.cBits)
    {
        if(cElements!=rhs.cElements)
        {
            if(element!=nullptr)
                delete[] element;

            cElements=rhs.cElements;
            element=new uint64_t[cElements];
        }
        cBits=rhs.cBits;
        clearMask=rhs.clearMask;
    }

    rhs.element[rhs.cElements-1]&=rhs.clearMask;
    uint64_t *ptr=rhs.element;

    for(int_fast32_t i=0; i<cElements; i++)
    {
        element[i]=ptr[i];
    }
	currBitToSet=cBits;
    return *this;
}

DynamicBitSet& DynamicBitSet::operator=(const char* bits)
{
    lazyCount=true;
    for(int_fast32_t i=0; bits[i]!='\0'; i++)
    {
        if(bits[i]=='1')
            set(i);
        else
            set(i,0);
    }
    return *this;
}

DynamicBitSet& DynamicBitSet::operator^=(const uint64_t num)
{
    lazyCount=true;
    element[0]^=num;
    return *this;
}

DynamicBitSet& DynamicBitSet::operator|=(const uint64_t num)
{
    lazyCount=true;
    element[0]|=num;
    return *this;
}

DynamicBitSet& DynamicBitSet::operator&=(const uint64_t num)
{
    lazyCount=true;
    for(int_fast32_t i=1; i<cElements; i++)
    {
        element[i]=0LL;
    }
    element[0]&=num;
    return *this;
}

DynamicBitSet& DynamicBitSet::operator|=(const DynamicBitSet& rhs)
{
    lazyCount=true;
    uint64_t *ptr=rhs.element;

    if(cElements==rhs.cElements)
    {
        for(int_fast32_t i=0; i<cElements; i++)
        {
            element[i]|=ptr[i];
        }
    }
    else
    {
        int_fast32_t rhsCElements=rhs.cElements;
        for(int_fast32_t i=0; i<cElements; i++)
        {
            if(i<rhsCElements)
                element[i]|=ptr[i];
            else
                break;
        }
    }
    return *this;
}

DynamicBitSet& DynamicBitSet::operator&=(const DynamicBitSet& rhs)
{
    lazyCount=true;
    uint64_t *ptr=rhs.element;
    rhs.element[rhs.cElements-1]&=rhs.clearMask;
    if(cElements==rhs.cElements)
    {
        for(int_fast32_t i=0; i<cElements; i++)
        {
            element[i]&=ptr[i];
        }
    }
    else
    {
        int_fast32_t rhsCElements=rhs.cElements;
        for(int_fast32_t i=0; i<cElements; i++)
        {
            if(i>=rhsCElements)
                element[i]=0LL;
            else
                element[i]&=ptr[i];
        }
    }

    return *this;
}

DynamicBitSet& DynamicBitSet::operator^=(const DynamicBitSet& rhs)
{
    lazyCount=true;
    uint64_t *ptr=rhs.element;
    rhs.element[rhs.cElements-1]&=rhs.clearMask;
    if(cElements==rhs.cElements)
    {
        for(int_fast32_t i=0; i<cElements; i++)
        {
            element[i]^=ptr[i];
        }
    }
    else
    {
        int_fast32_t rhsCElements=rhs.cElements;
        for(int_fast32_t i=0; i<cElements; i++)
        {
            if(i<rhsCElements)
                element[i]^=ptr[i];
            else
                break;
        }
    }

    return *this;
}

DynamicBitSet& DynamicBitSet::operator<<=(uint_fast32_t shifted)
{
    lazyCount=true;
    element[cElements-1]&=clearMask;

    uint64_t toShift=shifted%sizeOfElement;
    uint_fast32_t toSkip=shifted/sizeOfElement;
    uint64_t shiftWrongWay=sizeOfElement-toShift;

    if(toSkip>0)
    {
        for(uint_fast32_t i=cElements-1-toSkip; i>0; i--)
        {
            element[i+toSkip]=(element[i]<<toShift)|(element[i-1]>>shiftWrongWay);
            element[i]=0LL;
        }
        element[toSkip]=element[0]<<toShift;
        for(uint_fast32_t i=0; i<toSkip; i++)
            element[i]=0LL;
    }
    else
    {
        for(uint_fast32_t i=cElements-1; i>0; i--)
        {
            element[i]=(element[i]<<toShift)|(element[i-1]>>shiftWrongWay);
        }
        element[0]=element[0]<<toShift;
    }

    return *this;
}

DynamicBitSet& DynamicBitSet::operator>>=(uint_fast32_t shifted)
{
    lazyCount=true;
    element[cElements-1]&=clearMask;

    uint64_t toShift=shifted%sizeOfElement;
    uint_fast32_t toSkip=shifted/sizeOfElement;
    uint64_t shiftWrongWay=sizeOfElement-toShift;

    if(toSkip>0)
    {
        for(int_fast32_t i=toSkip; i<cElements-1; i++)
        {
            element[i-toSkip]=(element[i]>>toShift)|(element[i+1]<<shiftWrongWay);
            element[i]=0LL;
        }
        element[cElements-1-toSkip]=element[cElements-1]>>toShift;
        for(uint_fast32_t i=1; i<=toSkip; i++)
            element[cElements-toSkip]=0LL;
    }
    else
    {
        for(int_fast32_t i=0; i<cElements-1; i++)
        {
            element[i]=(element[i]>>toShift)|(element[i+1]<<shiftWrongWay);
        }
        element[cElements-1]=element[cElements-1]>>toShift;
    }

    return *this;
}

DynamicBitSet DynamicBitSet::operator<<(uint_fast32_t shifted) const
{
    element[cElements-1]&=clearMask;
    DynamicBitSet newSet(*this);
    if(shifted==0)
        return newSet;

    uint64_t *ptr=newSet.element;
    ptr[cElements-1]&=clearMask;

    uint64_t toShift=shifted%sizeOfElement;
    uint_fast32_t toSkip=shifted/sizeOfElement;
    uint64_t shiftWrongWay=sizeOfElement-toShift;

    if(toSkip>0)
    {
        for(uint_fast32_t i=cElements-1-toSkip; i>0; i--)
        {
            ptr[i+toSkip]=(ptr[i]<<toShift)|(ptr[i-1]>>shiftWrongWay);
            ptr[i]=0LL;
        }
        ptr[toSkip]=ptr[0]<<toShift;
        for(uint_fast32_t i=0; i<toSkip; i++)
            ptr[i]=0LL;
        ptr[0]=0LL;
    }
    else
    {
        for(uint_fast32_t i=cElements-1-toSkip; i>0; i--)
        {
            ptr[i]=(ptr[i]<<toShift)|(ptr[i-1]>>shiftWrongWay);
        }
        ptr[0]=ptr[0]<<toShift;
    }

    return newSet;
}

DynamicBitSet DynamicBitSet::operator>>(uint_fast32_t shifted) const
{
    element[cElements-1]&=clearMask;
    DynamicBitSet newSet(*this);
    uint64_t *ptr=newSet.element;
    ptr[cElements-1]&=clearMask;

    uint64_t toShift=shifted%sizeOfElement;
    uint_fast32_t toSkip=shifted/sizeOfElement;
    uint64_t shiftWrongWay=sizeOfElement-toShift;

    if(toSkip>0)
    {
        for(int_fast32_t i=toSkip; i<cElements-1; i++)
        {
            ptr[i-toSkip]=(ptr[i]>>toShift)|(ptr[i+1]<<shiftWrongWay);
            ptr[i]=0;
        }
        ptr[cElements-1-toSkip]=ptr[cElements-1]>>toShift;
        for(uint_fast32_t i=1; i<=toSkip; i++)
            ptr[cElements-toSkip]=0LL;
    }
    else
    {
        for(int_fast32_t i=toSkip; i<cElements-1; i++)
        {
            ptr[i-toSkip]=(ptr[i]>>toShift)|(ptr[i+1]<<shiftWrongWay);
        }
        ptr[cElements-1-toSkip]=ptr[cElements-1]>>toShift;
    }
    return newSet;
}

DynamicBitSet&  DynamicBitSet::operator+=(uint64_t num)
{
    lazyCount=true;
    element[cElements-1]&=clearMask;

    for(int_fast32_t i=0; i<cElements; i++)
    {
        if(element[i] <= OVERFLOW_MAX-num)
        {
            element[i]+=num;
            break;
        }
        else
        {
            element[i]+=num;
            num=1;
        }
    }
    return *this;
}

DynamicBitSet& DynamicBitSet::operator-=(uint64_t num)
{
    lazyCount=true;
    element[cElements-1]&=clearMask;

    for(int_fast32_t i=0; i<cElements; i++)
    {
        if(element[i] >= num)
        {
            element[i]-=num;
            break;
        }
        else
        {
            element[i]-=num;
            num=1;
        }
    }
    return *this;
}

DynamicBitSet&  DynamicBitSet::operator+=(const DynamicBitSet& other)
{
    lazyCount=true;
    element[cElements-1]&=clearMask;
    other.element[other.cElements-1]&=other.clearMask;
    uint64_t num=0;
    uint64_t tmpElement=0;
    uint64_t *ptr=other.element;

    int_fast32_t cMaxElements=std::max(other.cElements,cElements);

    for(int_fast32_t i=0; i<cMaxElements; i++)
    {
        tmpElement=element[i];
        if(tmpElement <= OVERFLOW_MAX-num)
        {
            tmpElement+=num;
            num=0;
        }
        else
        {
            tmpElement+=num;
            num=1;
        }

        if(i<other.cElements)
        {
            if(tmpElement <= OVERFLOW_MAX-ptr[i])
            {
                tmpElement+=ptr[i];
            }
            else
            {
                tmpElement+=ptr[i];
                num++;
            }
        }
        element[i]=tmpElement;
    }
    return *this;
}

DynamicBitSet& DynamicBitSet::operator%=(const DynamicBitSet& other)
{
	if(other>*this)
		return *this;

	DynamicBitSet X(this->size());
	X.reset();
	X|=other;

	DynamicBitSet tmpA=*this;
	tmpA>>=1;

	int_fast32_t leftestBitA=tmpA.getLeftestSetBitIndex();
	int_fast32_t leftestBitX=X.getLeftestSetBitIndex();

	int_fast32_t dif=leftestBitA-leftestBitX;

	if(dif>1)
	{
		X <<= dif-1;
	}

	while (X <= tmpA)
	{
		X <<= 1;
	}

	while (*this >= other)
	{
		if (*this >= X)
		{
			*this -= X;
		}
		X >>= 1;
	}
	return *this;
}

DynamicBitSet& DynamicBitSet::operator*=(const DynamicBitSet& other)
{
	std::deque<uint32_t> tmpBits=getAllSetBits();
	DynamicBitSet b=other;
    this->reset();

	for(auto it=tmpBits.begin();it!=tmpBits.end();++it)
	{
		*this+= b << (*it);
	}

    return *this;
}

DynamicBitSet& DynamicBitSet::operator*=(uint64_t num)
{
	std::deque<uint32_t> tmpBits=getAllSetBits();
	DynamicBitSet b(this->size());
	b|=num;
	this->reset();

	for(auto it=tmpBits.begin();it!=tmpBits.end();++it)
	{
		*this+= b << (*it);
	}

    return *this;
}

DynamicBitSet& DynamicBitSet::operator%=(uint64_t num)
{
	DynamicBitSet other(64);
	other.reset();
	other|=num;
	*this%=other;

	return *this;
}

DynamicBitSet DynamicBitSet::operator%(const DynamicBitSet& other) const
{
    DynamicBitSet newSet=*this;
	newSet%=other;
	return newSet;
}

uint64_t DynamicBitSet::operator%(uint64_t num) const
{
    DynamicBitSet newSet=*this;
	newSet%=num;
	return newSet.element[0];
}

DynamicBitSet&  DynamicBitSet::operator-=(const DynamicBitSet& other)
{
    lazyCount=true;
    element[cElements-1]&=clearMask;
    other.element[other.cElements-1]&=other.clearMask;
    uint64_t num=0;
    uint64_t tmpElement=0;
    uint64_t *ptr=other.element;

    int_fast32_t cMaxElements=std::max(other.cElements,cElements);

    for(int_fast32_t i=0; i<cMaxElements; i++)
    {
        tmpElement=element[i];
        if(tmpElement >= num)
        {
            tmpElement-=num;
            num=0;
        }
        else
        {
            tmpElement-=num;
            num=1;
        }

        if(i<other.cElements)
        {
            if(tmpElement >= ptr[i])
            {
                tmpElement-=ptr[i];
            }
            else
            {
                tmpElement-=ptr[i];
                num++;
            }
        }
        element[i]=tmpElement;
    }
    return *this;
}

DynamicBitSet DynamicBitSet::operator-(const DynamicBitSet& other) const
{
    uint64_t *ptr;
    uint64_t num=0;
    int_fast32_t cMaxElements;
    int_fast32_t cMinElements;
    DynamicBitSet newSet;

    if(other.cElements>cElements)
    {
        newSet=other;
        ptr=element;
        cMaxElements=other.cElements;
        cMinElements=cElements;
    }
    else
    {
        newSet=*this;
        ptr=other.element;
        cMaxElements=cElements;
        cMinElements=other.cElements;
    }

    uint64_t *newPtr=newSet.element;

    for(int_fast32_t i=0; i<cMaxElements; i++)
    {
        if(newPtr[i] >= num)
        {
            newPtr[i]-=num;
            num=0;
        }
        else
        {
            newPtr[i]-=num;
            num=1;
        }

        if(i<cMinElements)
        {
            if(newPtr[i] >= ptr[i])
            {
                newPtr[i]-=ptr[i];
            }
            else
            {
                newPtr[i]-=ptr[i];
                num++;
            }
        }
    }
    return newSet;
}

DynamicBitSet DynamicBitSet::operator+(const DynamicBitSet& other) const
{
    uint64_t *ptr;
    uint64_t num=0;
    int_fast32_t cMaxElements;
    int_fast32_t cMinElements;
    DynamicBitSet newSet;

    if(other.cElements>cElements)
    {
        newSet=other;
        ptr=element;
        cMaxElements=other.cElements;
        cMinElements=cElements;
    }
    else
    {
        newSet=*this;
        ptr=other.element;
        cMaxElements=cElements;
        cMinElements=other.cElements;
    }

    uint64_t *newPtr=newSet.element;

    for(int_fast32_t i=0; i<cMaxElements; i++)
    {
        if(newPtr[i] <= OVERFLOW_MAX-num)
        {
            newPtr[i]+=num;
            num=0;
        }
        else
        {
            newPtr[i]+=num;
            num=1;
        }

        if(i<cMinElements)
        {
            if(newPtr[i] <= OVERFLOW_MAX-ptr[i])
            {
                newPtr[i]+=ptr[i];
            }
            else
            {
                newPtr[i]+=ptr[i];
                num++;
            }
        }
    }
    return newSet;
}

DynamicBitSet DynamicBitSet::operator-(uint64_t num) const
{
    DynamicBitSet newSet;

    newSet=*this;

    uint64_t *newPtr=newSet.element;

    for(int_fast32_t i=0; i<cElements; i++)
    {
        if(newPtr[i] >= num)
        {
            newPtr[i]-=num;
            num=0;
        }
        else
        {
            newPtr[i]-=num;
            num=1;
        }
    }
    return newSet;
}

DynamicBitSet DynamicBitSet::operator+(uint64_t num) const
{
    DynamicBitSet newSet;

    newSet=*this;

    uint64_t *newPtr=newSet.element;

    for(int_fast32_t i=0; i<cElements; i++)
    {
        if(newPtr[i] <= OVERFLOW_MAX-num)
        {
            newPtr[i]+=num;
            num=0;
        }
        else
        {
            newPtr[i]+=num;
            num=1;
        }

        if(num==0)
            break;
    }
    return newSet;
}

DynamicBitSet DynamicBitSet::operator|(const DynamicBitSet& rhs) const
{
    element[cElements-1]&=clearMask;
    rhs.element[rhs.cElements-1]&=rhs.clearMask;
    DynamicBitSet newSet(*this);
    uint64_t *newPtr=newSet.element;
    uint64_t *ptr=rhs.element;

    if(cElements==rhs.cElements)
    {
        for(int_fast32_t i=0; i<cElements; i++)
        {
            newPtr[i]|=ptr[i];
        }
    }
    else
    {
        int_fast32_t rhsCElements=rhs.cElements;
        for(int_fast32_t i=0; i<cElements; i++)
        {
            if(i<rhsCElements)
                newPtr[i]|=ptr[i];
        }
    }
    return newSet;
}

DynamicBitSet DynamicBitSet::operator~() const
{
    element[cElements-1]&=clearMask;
    DynamicBitSet newSet(*this);
    uint64_t *newPtr=newSet.element;

    for(int_fast32_t i=0; i<cElements; ++i)
    {
        newPtr[i]=~newPtr[i];
    }

    return newSet;
}

DynamicBitSet DynamicBitSet::operator&(const DynamicBitSet& rhs) const
{
    element[cElements-1]&=clearMask;
    rhs.element[rhs.cElements-1]&=rhs.clearMask;
    DynamicBitSet newSet(*this);
    uint64_t *newPtr=newSet.element;
    uint64_t *ptr=rhs.element;
    if(cElements==rhs.cElements)
    {
        for(int_fast32_t i=0; i<cElements; i++)
        {
            newPtr[i]&=ptr[i];
        }
    }
    else
    {
        int_fast32_t rhsCElements=rhs.cElements;
        for(int_fast32_t i=0; i<cElements; i++)
        {
            if(i>=rhsCElements)
                newPtr[i]=0L;
            else
                newPtr[i]&=rhs.element[i];
        }
    }

    return newSet;
}

DynamicBitSet DynamicBitSet::operator^(const DynamicBitSet& rhs) const
{
    element[cElements-1]&=clearMask;
    rhs.element[rhs.cElements-1]&=rhs.clearMask;
    DynamicBitSet newSet(*this);
    uint64_t *newPtr=newSet.element;
    int_fast32_t rhsCElements=rhs.cElements;

    uint64_t *ptr=rhs.element;
    if(cElements==rhsCElements)
    {
        for(int_fast32_t i=0; i<cElements; i++)
        {
            newPtr[i]^=ptr[i];
        }
    }
    else
    {
        for(int_fast32_t i=0; i<cElements; i++)
        {
            if(i<rhsCElements)
                newPtr[i]^=ptr[i];
            else
                break;
        }
    }

    return newSet;
}

bool DynamicBitSet::operator<(const DynamicBitSet& rhs) const
{
    uint64_t *ptr=rhs.element;
    int_fast32_t rhsCElements=rhs.cElements;
    element[cElements-1]&=clearMask;
    rhs.element[rhs.cElements-1]&=rhs.clearMask;

    if(cElements==rhsCElements)
    {
        for(int_fast32_t i=cElements-1; i>=0; i--)
        {
            if(element[i]<ptr[i])
                return true;
            if(element[i]>ptr[i])
                return false;
        }
    }
    else
    {
        for(int_fast32_t i=cElements-1; i>=0; i--)
        {
            if(i<rhsCElements)
            {
                if(element[i]<ptr[i])
                    return true;
                if(element[i]>ptr[i])
                    return false;
            }
            else if(element[i]!=0)
                return false;
        }
    }
    return false;
}

bool DynamicBitSet::operator>(const DynamicBitSet& rhs) const
{
    uint64_t *ptr=rhs.element;
    int_fast32_t rhsCElements=rhs.cElements;
    element[cElements-1]&=clearMask;
    rhs.element[rhs.cElements-1]&=rhs.clearMask;

    if(cElements==rhsCElements)
    {
        for(int_fast32_t i=cElements-1; i>=0; i--)
        {
            if(element[i]>ptr[i])
                return true;
            if(element[i]<ptr[i])
                return false;
        }
    }
    else
    {
        for(int_fast32_t i=cElements-1; i>=0; i--)
        {
            if(i<rhsCElements)
            {
                if(element[i]>ptr[i])
                    return true;
                if(element[i]<ptr[i])
                    return false;
            }
            else if(element[i]!=0)
                return true;
        }
    }
    return false;
}

bool DynamicBitSet::operator>=(const DynamicBitSet& rhs) const
{
    uint64_t *ptr=rhs.element;
    int_fast32_t rhsCElements=rhs.cElements;
    element[cElements-1]&=clearMask;
    rhs.element[rhs.cElements-1]&=rhs.clearMask;

    if(cElements==rhsCElements)
    {
        for(int_fast32_t i=cElements-1; i>=0; i--)
        {
            if(element[i]>ptr[i])
                return true;
            if(element[i]<ptr[i])
                return false;
        }
    }
    else
    {
        for(int_fast32_t i=cElements-1; i>=0; i--)
        {
            if(i<rhsCElements)
            {
                if(element[i]>ptr[i])
                    return true;
                if(element[i]<ptr[i])
                    return false;
            }
            else if(element[i]!=0)
                return true;
        }
    }
    return true;
}

bool DynamicBitSet::operator<=(const DynamicBitSet& rhs) const
{
    uint64_t *ptr=rhs.element;
    int_fast32_t rhsCElements=rhs.cElements;
    element[cElements-1]&=clearMask;
    rhs.element[rhs.cElements-1]&=rhs.clearMask;

    if(cElements==rhsCElements)
    {
        for(int_fast32_t i=cElements-1; i>=0; i--)
        {
            if(element[i]<ptr[i])
                return true;
            if(element[i]>ptr[i])
                return false;
        }
    }
    else
    {
        for(int_fast32_t i=cElements-1; i>=0; i--)
        {
            if(i<rhsCElements)
            {
                if(element[i]<ptr[i])
                    return true;
                if(element[i]>ptr[i])
                    return false;
            }
            else if(element[i]!=0)
                return false;
        }
    }
    return true;
}

bool DynamicBitSet::operator==(const DynamicBitSet& rhs) const
{
    uint64_t *ptr=rhs.element;
    element[cElements-1]&=clearMask;
    rhs.element[rhs.cElements-1]&=rhs.clearMask;

    if(rhs.cBits!=cBits)
        return false;

    for(int_fast32_t i=cElements-1; i>=0; i--)
    {
        if(element[i]!=ptr[i])
            return false;
    }
    return true;
}

bool DynamicBitSet::operator!=(const DynamicBitSet& rhs) const
{
    uint64_t *ptr=rhs.element;
    element[cElements-1]&=clearMask;
    ptr[rhs.cElements-1]&=rhs.clearMask;

    if(rhs.cBits!=cBits)
        return true;

    for(int_fast32_t i=0; i<cElements; i++)
    {
        if(element[i]!=ptr[i])
            return true;
    }
    return false;
}

bool DynamicBitSet::operator[](uint_fast32_t idx) const
{
    return element[idx/sizeOfElement]&(1LL<<idx%sizeOfElement);
}

bool DynamicBitSet::any() const
{
    element[cElements-1]&=clearMask;

    for(int_fast32_t i=0; i<cElements; i++)
    {
        if(element[i]!=0)
            return true;
    }
    return false;
}

bool DynamicBitSet::all() const
{
    element[cElements-1]&=clearMask;
    if((0xFFFFFFFFFFFFFFFFLL&clearMask)!=element[cElements-1])
        return false;

    for(int_fast32_t i=1; i<cElements; i++)
    {
        if(element[i]!=0xFFFFFFFFFFFFFFFFLL)
            return false;
    }
    return true;
}

bool DynamicBitSet::none() const
{
    element[cElements-1]&=clearMask;

    for(int_fast32_t i=0; i<cElements; i++)
    {
        if(element[i]!=0)
            return false;
    }
    return true;
}

bool DynamicBitSet::test(uint_fast32_t idx) const
{
    return (*this)[idx];
}

void DynamicBitSet::reset(uint_fast32_t idx)
{
    lazyCount=true;
    set(idx,0);
}

void DynamicBitSet::flip(uint_fast32_t idx)
{
    lazyCount=true;
    element[idx/sizeOfElement]^=(1LL<<(idx%sizeOfElement));
}

void DynamicBitSet::flip()
{
    lazyCount=true;
    for(int_fast32_t i=0; i<cElements; i++)
    {
        element[i]^=0xFFFFFFFFFFFFFFFFLL;
    }
}

int_fast32_t DynamicBitSet::count()
{
    return getSetBitCount();
}

int_fast32_t DynamicBitSet::size() const
{
    return currBitToSet;
}

bool DynamicBitSet::intersects(const DynamicBitSet& rhs) const
{
    uint64_t *ptr=rhs.element;
    element[cElements-1]&=clearMask;
    ptr[rhs.cElements-1]&=rhs.clearMask;

    if(cElements==rhs.cElements)
    {
        for(int_fast32_t i=0; i<cElements; i++)
        {
            if((element[i] & ptr[i])!=0LL)
                return true;
        }
    }
    else
    {
        int_fast32_t rhsCElements=rhs.cElements;
        for(int_fast32_t i=0; i<cElements; i++)
        {
            if(i<rhsCElements)
            {
                if((element[i] & ptr[i])!=0LL)
                    return true;
            }
            else
            {
                break;
            }
        }
    }
    return false;
}

std::string DynamicBitSet::to_string() const
{
    std::string tmp;
    for(int_fast32_t i=currBitToSet-1; i>=0; i--)
    {
        tmp+= ((*this)[i]==0) ? '0' : '1';
    }
    return tmp;
}

int_fast32_t DynamicBitSet::hashcode() const
{
    uint64_t h=cBits;
    element[cElements-1]&=clearMask;

    for (int_fast32_t i = 0; i < cElements; i++)
        h = ((213514013LL^h)+2531011LL)^element[i];
    return (int)((h >> 32) ^ h);
}

std::deque<uint32_t> DynamicBitSet::getAllSetBits()
{
    element[cElements-1]&=clearMask;
    std::deque<uint32_t> tmp;
    uint64_t x;
    uint_fast32_t j=0;
    for(uint_fast32_t i=0; i<cBits; i+=64)
    {
        x=element[j];
        if(x!=0LL)
        {
            if(x & 0x00000000FFFFFFFFLL)
            {
                if(x & 0x000000000000FFFFLL)
                {
					setAll8BitSet(tmp,x & 0x00000000000000FFLL,i);
					setAll8BitSet(tmp,(x & 0x000000000000FF00LL)>>8,i+8);
                }
                if(x & 0x00000000FFFF0000LL)
                {
					setAll8BitSet(tmp,(x & 0x0000000000FF0000LL)>>16,i+16);
					setAll8BitSet(tmp,(x & 0x00000000FF000000LL)>>24,i+24);
                }
            }

            if(x & 0xFFFFFFFF00000000LL)
            {
                if(x & 0x0000FFFF00000000LL)
                {
					setAll8BitSet(tmp,(x & 0x000000FF00000000LL)>>32,i+32);
					setAll8BitSet(tmp,(x & 0x0000FF0000000000LL)>>40,i+40);
                }
                if(x & 0xFFFF000000000000LL)
                {
                    setAll8BitSet(tmp,(x & 0x00FF000000000000LL)>>48,i+48);
					setAll8BitSet(tmp,(x & 0xFF00000000000000LL)>>56,i+56);
                }
            }
        }
        j++;
    }
    return tmp;
}

DynamicBitSet DynamicBitSet::createSubBitset(int_fast32_t from, int_fast32_t to)
{
    to++;
    element[cElements-1]&=clearMask;
    DynamicBitSet newSet(to-from);
    uint64_t *newPtr=newSet.element;

    uint64_t elementToCheckFrom=from/sizeOfElement;
    uint64_t elementToCheckTo=to/sizeOfElement;
    uint64_t shiftRight=from%sizeOfElement;

    if(elementToCheckTo==elementToCheckFrom)
    {
        uint64_t shiftLeft=(sizeOfElement-(to%sizeOfElement));
        newPtr[0]=(((element[elementToCheckFrom]<<shiftLeft)>>shiftLeft)>>shiftRight);
    }
    else
    {
        uint64_t invShiftRight=sizeOfElement-shiftRight;

        newPtr[0]=(element[elementToCheckFrom]>>shiftRight);
        int_fast32_t j=1;
        for(uint_fast32_t i=elementToCheckFrom+1; i<=elementToCheckTo; i++)
        {
            newPtr[j-1]|=element[i]<<invShiftRight;
            if(j<newSet.cElements)
                newPtr[j]=element[i]>>shiftRight;
            j++;
        }
        newPtr[newSet.cElements-1]&=newSet.clearMask;
    }
    return newSet;
}

int_fast32_t DynamicBitSet::capacity() const
{
	return cBits;
}

void DynamicBitSet::push_back(bool bit)
{
	uint64_t currBitToSetBefore=currBitToSet;
	if(cBits==0)
	{
		setUp(1);
	}
	else if((unsigned)(sizeOfElement*cElements)<=cBits-1)
	{
		resize(currBitToSet+1);
	}
	set(currBitToSetBefore,bit);
	currBitToSet=currBitToSetBefore;
	currBitToSet++;
}

void DynamicBitSet::append(const DynamicBitSet& other)
{
	if(cBits==0)
	{
		*this=other;
		return;
	}

	uint64_t currBitToSetBefore=currBitToSet;
	int_fast32_t cElementsBefore=cElements;

	currBitToSet+=other.currBitToSet;
	if((uint64_t)sizeOfElement*cElements<=currBitToSet)
	{
		resize(currBitToSet);
	}
	else
	{
		cBits=currBitToSet;
		int_fast32_t bitsLeft=sizeOfElement-((cElements*sizeOfElement)-cBits);
		clearMask=0;
		for(int_fast32_t i=0; i<bitsLeft; i++)
		{
			clearMask|=(1LL<<i);
		}
	}

	int_fast32_t cTmpElements=other.cElements;
	uint64_t *ptr=other.element;
	int_fast32_t j=0;

	uint64_t toShift=currBitToSetBefore%sizeOfElement;
    uint64_t shiftWrongWay=sizeOfElement-toShift;

	element[currBitToSetBefore/sizeOfElement]|=(ptr[0]<<toShift);
	for(int_fast32_t i=cElementsBefore;j<cTmpElements-1 && i<cElements; i++)
	{
		element[i]=(ptr[j]>>toShift)|(ptr[j+1]<<shiftWrongWay);
		j++;
	}
	if(cElements!=cElementsBefore)
		element[cElements-1]=ptr[cTmpElements-1]>>toShift;
}

DynamicBitSet::proxyBit DynamicBitSet::operator[](int_fast32_t idx)
{
    return DynamicBitSet::proxyBit(idx,this);
}

std::ostream& operator<<(std::ostream &out,const DynamicBitSet& dSet)
{
    for(int_fast32_t i=dSet.currBitToSet-1; i>=0; i--)
    {
        out<<dSet[i];
    }
    return out;
}
