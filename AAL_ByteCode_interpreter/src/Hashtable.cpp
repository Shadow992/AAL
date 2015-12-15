#include "Hashtable.h"

template <typename type> Hashtable<type>::Hashtable()
{
	hashtable = new std::vector<Entry>;
    hashtable->resize(16);
}

template <typename type> Hashtable<type>::~Hashtable()
{
    delete hashtable;
}

template <typename type> std::vector<int>* Hashtable<type>::getSetIdx()
{
	return &setIndex;
}

template <typename type> inline unsigned int Hashtable<type>::capacity()
{
	return 1<<scalingFactor;
}

template <typename type> inline type* Hashtable<type>::getByIndex(unsigned int i)
{
	return (*hashtable)[i].obj;
}

template <typename type> inline unsigned long long Hashtable<type>::getNumByIndex(unsigned int i)
{
	return (*hashtable)[i].num;
}

template <typename type> type* Hashtable<type>::set(unsigned long long num, type* obj)
{
	if(cItems > hashtable->size()>>1)
	{
		std::vector<Entry>* hashtableTmp = allocateVector();
		unsigned long long oldSize=hashtable->size()-1;
		scalingFactor+=2;
		unsigned long long newSize=(1<<scalingFactor);
		hashtableTmp->resize(newSize);
		newSize--;
		for(unsigned int i=0;i<=oldSize;i++)
		{
			if((*hashtable)[i].obj != nullptr)
			{
				unsigned long long tmp=(*hashtable)[i].num & newSize;
				while((*hashtableTmp)[tmp].obj!=nullptr)
				{
					tmp=(tmp<newSize) ? tmp+1 : 0;
				}
				(*hashtableTmp)[tmp].obj=(*hashtable)[i].obj;
				(*hashtableTmp)[tmp].num=(*hashtable)[i].num;
			}
		}
		delete hashtable;
		hashtable=hashtableTmp;
	}

	unsigned long long newSize=hashtable->size()-1;
	unsigned long long tmp=num & newSize;
	while((*hashtable)[tmp].obj!=nullptr)
	{
		if((*hashtable)[tmp].num==num)
		{
			(*hashtable)[tmp].num=num;
			type* objBefore=(*hashtable)[tmp].obj;
			(*hashtable)[tmp].obj=obj;
			return objBefore;
		}
		(tmp<newSize) ? tmp++ : tmp=0;
	}
	(*hashtable)[tmp].num=num;
	type* objBefore=(*hashtable)[tmp].obj;
	(*hashtable)[tmp].obj=obj;
	setIndex.push_back(tmp);
	cItems++;
	return objBefore;
}

template <typename type> type* Hashtable<type>::get(unsigned long long num)
{
	unsigned long long newSize=hashtable->size()-1;
	unsigned long long tmp=num & newSize;
	if((*hashtable)[tmp].obj==nullptr)
    {
        return nullptr;
    }

	unsigned long long startingPoint=tmp;
	while((*hashtable)[tmp].num!=num)
	{
		(tmp<newSize) ? tmp++ : tmp=0;
		if(startingPoint==tmp || (*hashtable)[tmp].obj==nullptr)
		{
			return nullptr;
		}
	}
	return (*hashtable)[tmp].obj;
}

template <typename type> unsigned int Hashtable<type>::count()
{
	return cItems;
}

template <typename type> void Hashtable<type>::clear()
{
	if(hashtable->size()!=16)
		hashtable->resize(16);

	int hashSize=setIndex.size();
	for(int i=0;i<hashSize;i++)
	{
	    if(setIndex[i]>=16)
        {
            continue;
        }
		(*hashtable)[setIndex[i]].obj=nullptr;
		(*hashtable)[setIndex[i]].num=max_long;
	}
	setIndex.clear();
	cItems=0;
	scalingFactor=4;
}
