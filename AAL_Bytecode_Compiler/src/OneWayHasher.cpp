#include "OneWayHasher.h"

OneWayHasher::OneWayHasher()
{
    a.setUp(128);
    b.setUp(128);
    primeMap[2]=1;
    primeMap[3]=1;
    primeMap[5]=1;
    primeMap[7]=1;
    primeMap[11]=1;
    primeMap[13]=1;
    primeMap[17]=1;
    primeMap[19]=1;
    primeMap[23]=1;
    primeMap[29]=1;
    primeMap[31]=1;
    primeMap[37]=1;
    primeMap[41]=1;
    primeMap[43]=1;
    primeMap[47]=1;
    primeMap[53]=1;
    primeMap[59]=1;
    primeMap[61]=1;
    primeMap[67]=1;
    primeMap[71]=1;
    primeMap[73]=1;
    primeMap[79]=1;
    primeMap[83]=1;
    primeMap[89]=1;
    primeMap[97]=1;
    primeMap[101]=1;
    primeMap[103]=1;
    primeMap[107]=1;
    primeMap[109]=1;
    primeMap[113]=1;
    primeMap[127]=1;
    primeMap[131]=1;
    primeMap[137]=1;
    primeMap[139]=1;
    primeMap[149]=1;
    primeMap[151]=1;
    primeMap[157]=1;
    primeMap[163]=1;

    unsigned long long seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator.seed(seed*seed);

    p=getNextPrime(generator()%4294900296ULL+66000ULL);

    int tmp=0;
    while(tmp > 20 || tmp < 2)
    {
        q=getNextPrime(generator()%4294900296ULL+66000ULL);
        tmp=log2(p)-log2(q);
    }

    n=p*q;
    unsigned long long k=(p-1ULL)*(q-1ULL);
    unsigned long long modNum = std::min(6356ULL,k-1ULL);

    e=generator()%(modNum-4ULL)+2ULL;
    while(!isRelativelyPrime(e,k))
    {
        e=generator()%(modNum-4ULL)+2ULL;
    }
}

OneWayHasher::~OneWayHasher()
{
    //dtor
}

unsigned long long OneWayHasher::powMod( unsigned long long num, unsigned long long pow, unsigned long long mod)
{
    unsigned long long test=1;
    while(pow)
    {
        if (pow & 1)
            test = (test * num) % mod;
        num = (num * num) % mod;
		pow >>= 1;
    }
    return test;
}

unsigned long long OneWayHasher::powMod128( unsigned long long num, unsigned long long pow, unsigned long long mod)
{
	a.reset();
	a|=num;
	b.reset();
	b|=1;

    while(pow)
    {
        if (pow & 1)
		{
			b*=a;
			b%=mod;
		}
		a*=a;
		a%=mod;
		pow >>= 1;
    }
    return b.element[0];
}


bool OneWayHasher::isPrime(unsigned long long num)
{
    for(auto it=primeMap.begin();it != primeMap.end();++it)
    {
        if(num==it->first)
        {
            return true;
        }
        else if(num%(it->first)==0)
        {
            return false;
        }
    }

    if(miller(num,16))
        return true;

    return false;
}

bool OneWayHasher::miller(unsigned long long num, int k)
{
    int s = 0;
    for (unsigned long long m = num-1ULL; m % 2ULL==0ULL; s++)
        m >>= 1;

    unsigned long long d = (num-1ULL) / (1ULL<<s);

    for (int i = 0; i < k; i++)
    {
        unsigned long long x = powMod(generator()%(num-5ULL) + 2ULL,d,num);

        if (x != 1ULL && x != num-1ULL)
        {
            bool continues=false;
            for (int r = 1; r < s; r++)
            {
                x = powMod(x, 2ULL, num);
                if (x == 1ULL)
                {
                    continues=false;
                    break;
                }
                if (x == num - 1ULL)
                {
                    continues=true;
                    break;
                }
            }
            if(continues==false)
                return false;
        }
    }

    return true;
}

unsigned long long OneWayHasher::getNextPrime(unsigned long long num)
{
    if(num%2==0)
        num++;

    while(!isPrime(num))
    {
        num+=2;

    }
    return num;
}

unsigned long long OneWayHasher::oneWayFunction(unsigned long long num)
{
    return powMod128(num,e,n);
}

std::string OneWayHasher::oneWayFunction(std::string str)
{
    int shifting = 0;
    int lStr=str.length();
    unsigned long long tmpNum=0;
    std::string tmpStr="";
    for(int i=0;i<lStr;i++)
    {
        if(shifting==32)
        {
            tmpStr+=toCompressedString(oneWayFunction(tmpNum));
            tmpNum=0;
            shifting=0;
        }
        tmpNum|=str[i]<<shifting;
        shifting+=8;
    }

    if(shifting!=0)
    {
        tmpStr+=toCompressedString(tmpNum);
    }
    return tmpStr;
}


