#ifndef ONEWAYHASHER_H
#define ONEWAYHASHER_H

#include <map>
#include <chrono>
#include <random>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "commonFuncs.h"
#include "DynamicBitSet.h"

class OneWayHasher
{
public:
    OneWayHasher();
    virtual ~OneWayHasher();
    unsigned long long powMod( unsigned long long num, unsigned long long exponent, unsigned long long modulo);
	unsigned long long powMod128( unsigned long long num, unsigned long long pow, unsigned long long mod);
    bool isPrime(unsigned long long num);
    bool miller(unsigned long long n, int k);
    unsigned long long getNextPrime(unsigned long long num);
    unsigned long long oneWayFunction(unsigned long long num);
    std::string oneWayFunction(std::string str);

    unsigned long long n;
	unsigned long long e;
protected:
private:
    std::map<unsigned long long,char> primeMap;
    std::mt19937_64 generator;
    unsigned long long p;
    unsigned long long q;
	DynamicBitSet a;
	DynamicBitSet b;
};

#endif // ONEWAYHASHER_H
