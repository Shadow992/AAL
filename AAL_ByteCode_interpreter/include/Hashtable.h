#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include "defines.h"

/**
* \brief Class which is similar to unordered_map but improved some details to speed code up.
*/

template <typename type>
class Hashtable
{
    public:
        Hashtable();
        virtual ~Hashtable();

        type* set(unsigned long long, type*);
        type* get(unsigned long long);
		unsigned int count();
		unsigned int capacity();
		std::vector<int>* getSetIdx();
		type* getByIndex(unsigned int i);
		unsigned long long getNumByIndex(unsigned int i);
        void clear();

    private:
        static const unsigned long long max_long=~0LL;
		struct Entry{unsigned long long num=max_long;type* obj=nullptr;};
        static NO_INLINE std::vector<Entry>* allocateVector()
        {
            return new std::vector<Entry>;
        }
		std::vector<Entry>* hashtable;
		std::vector<int> setIndex;
		unsigned int cItems=0;
		unsigned int scalingFactor=4;
};

#endif // HASHTABLE_H
