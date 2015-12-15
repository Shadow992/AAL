#ifndef DYNAMICBITSET_H
#define DYNAMICBITSET_H

#include <cstdint>
#include <iostream>
#include <cstring>
#include <vector>
#include <deque>
#include <iostream>     // std::cout
#include <functional>   // std::bit_and
#include <algorithm>    // std::transform
#include <iterator>     // std::end

#define sizeOfElement 64

class DynamicBitSet
{
    public:
		struct proxyBit
		{
		    uint_fast32_t bit;
			DynamicBitSet* d;

			proxyBit(uint_fast32_t idx, DynamicBitSet * bits) : bit(idx), d(bits) {}

			operator bool() const
			{
				return d->test(bit);
			}

			bool operator ~() const
			{
				return ~d->test(bit);
			}

			proxyBit& operator = (bool bitValue)
			{
				d->set(bit,bitValue);
				return *this;
			}

			proxyBit&  operator = (const proxyBit & bitValue)
			{
				d->set(bit,bitValue.d->test(bitValue.bit));
				return *this;
			}

			proxyBit& flip()
			{
				d->flip(bit);
				return *this;
			}
		};
        /**
         *  \brief Brief
         *
         *  \return Return_Description
         *
         *  \details Details
         */
        DynamicBitSet(){};
        virtual ~DynamicBitSet();
        /**
         *  \brief Brief
         *
         *  \param [in] length Parameter_Description
         *  \return Return_Description
         *
         *  \details Details
         */
        DynamicBitSet(uint64_t length);
        /**
         *  \brief Brief
         *
         *  \param [in] other Parameter_Description
         *  \return Return_Description
         *
         *  \details Details
         */
		DynamicBitSet(const DynamicBitSet& other);
		/**
		 *  \brief Brief
		 *
		 *  \param [in] bits Parameter_Description
		 *  \param [in] length Parameter_Description
		 *  \return Return_Description
		 *
		 *  \details Details
		 */
		DynamicBitSet(const uint64_t* bits, int_fast32_t length);
		DynamicBitSet(const std::vector<uint64_t>& bits);
        /**
         *  \brief Brief
         *
         *  \param [in] length Parameter_Description
         *  \return Return_Description
         *
         *  \details Details
         */
        void setUp(uint64_t length);
		/**
		 *  \brief Brief
		 *
		 *  \param [in] bits Parameter_Description
		 *  \param [in] length Parameter_Description
		 *  \return Return_Description
		 *
		 *  \details Details
		 */
		void setUp(const uint64_t* bits, int_fast32_t length);
		/**
		 *  \brief Brief
		 *
		 *  \param [in] bits Parameter_Description
		 *  \return Return_Description
		 *
		 *  \details Details
		 */
		void setUp(const std::vector<uint64_t>& bits);

        DynamicBitSet& operator=(const DynamicBitSet& other);
        DynamicBitSet& operator=(const char* bits);

        DynamicBitSet operator^(const DynamicBitSet& other) const;
        DynamicBitSet operator|(const DynamicBitSet& other) const;
        DynamicBitSet operator&(const DynamicBitSet& other) const;
		DynamicBitSet operator+(const DynamicBitSet& other) const;
        DynamicBitSet operator-(const DynamicBitSet& other) const;
		DynamicBitSet operator%(const DynamicBitSet& other) const;

        DynamicBitSet operator+(uint64_t num) const;
		DynamicBitSet operator-(uint64_t num) const;
		uint64_t operator%(uint64_t num) const;
        DynamicBitSet operator<<(uint_fast32_t shifted) const;
        DynamicBitSet operator>>(uint_fast32_t shifted) const;
        DynamicBitSet operator~() const;

        virtual DynamicBitSet& operator^=(const DynamicBitSet& other);
        virtual DynamicBitSet& operator|=(const DynamicBitSet& other);
        virtual DynamicBitSet& operator&=(const DynamicBitSet& other);
		virtual DynamicBitSet& operator+=(const DynamicBitSet& other);
		virtual DynamicBitSet& operator-=(const DynamicBitSet& other);
		virtual DynamicBitSet& operator*=(const DynamicBitSet& other);
		virtual DynamicBitSet& operator%=(const DynamicBitSet& other);

        virtual DynamicBitSet& operator^=(const uint64_t);
        virtual DynamicBitSet& operator|=(const uint64_t);
        virtual DynamicBitSet& operator&=(const uint64_t);
		virtual DynamicBitSet& operator+=(const uint64_t);
		virtual DynamicBitSet& operator-=(const uint64_t);
		virtual DynamicBitSet& operator*=(const uint64_t);
		virtual	DynamicBitSet& operator%=(uint64_t num);

        DynamicBitSet& operator<<=(uint_fast32_t shifted);
        DynamicBitSet& operator>>=(uint_fast32_t shifted);

        bool operator<(const DynamicBitSet& other) const;
        bool operator>(const DynamicBitSet& other) const;
        bool operator<=(const DynamicBitSet& other) const;
        bool operator>=(const DynamicBitSet& other) const;
        bool operator==(const DynamicBitSet& other) const;
        bool operator!=(const DynamicBitSet& other) const;

		proxyBit operator[](int_fast32_t idx);
        bool operator[](uint_fast32_t idx) const;
		/**
		 *  \brief Brief
		 *
		 *  \return Return_Description
		 *
		 *  \details Details
		 */
		int_fast32_t hashcode() const;
		/**
		 *  \brief Brief
		 *
		 *  \param [in] rhs Parameter_Description
		 *  \return Return_Description
		 *
		 *  \details Details
		 */
		bool intersects(const DynamicBitSet& rhs) const;
        /**
         *  \brief Brief
         *
         *  \return Return_Description
         *
         *  \details Details
         */
        int_fast32_t getRightestSetBitIndex();
        /**
         *  \brief Brief
         *
         *  \param [in] idxStart Parameter_Description
         *  \return Return_Description
         *
         *  \details Details
         */
        int_fast32_t getRightestSetBitIndex(uint_fast32_t idxStart);
        /**
         *  \brief Brief
         *
         *  \param [in] idxStart Parameter_Description
         *  \return Return_Description
         *
         *  \details Details
         */
        int_fast32_t getLeftestSetBitIndex(uint_fast32_t idxStart);
        /**
         *  \brief Brief
         *
         *  \return Return_Description
         *
         *  \details Details
         */
        int_fast32_t getLeftestSetBitIndex();
        /**
         *  \brief Brief
         *
         *  \return Return_Description
         *
         *  \details Details
         */
        void reset();
        /**
         *  \brief Brief
         *
         *  \return Return_Description
         *
         *  \details Details
         */
        uint_fast32_t getSetBitCount();
        /**
         *  \brief Brief
         *
         *  \param [in] idx Parameter_Description
         *  \param [in] val Parameter_Description
         *  \return Return_Description
         *
         *  \details Details
         */
        void set(uint_fast32_t idx,bool val=true);
        /**
         *  \brief Brief
         *
         *  \return Return_Description
         *
         *  \details Details
         */
        void set();
		/**
		 *  \brief Brief
		 *
		 *  \param [in] idxFrom Parameter_Description
		 *  \param [in] idxTo Parameter_Description
		 *  \param [in] val Parameter_Description
		 *  \return Return_Description
		 *
		 *  \details Details
		 */
		void setRange(uint_fast32_t idxFrom,uint_fast32_t idxTo,bool val=true);
		/**
		 *  \brief Brief
		 *
		 *  \param [in] from Parameter_Description
		 *  \param [in] to Parameter_Description
		 *  \return Return_Description
		 *
		 *  \details Details
		 */
		DynamicBitSet createSubBitset(int_fast32_t from, int_fast32_t to);
		/**
		 *  \brief Brief
		 *
		 *  \param [in] bit Parameter_Description
		 *  \return Return_Description
		 *
		 *  \details Details
		 */
		void push_back(bool bit);
		/**
		 *  \brief Brief
		 *
		 *  \param [in] other Parameter_Description
		 *  \return Return_Description
		 *
		 *  \details Details
		 */
		void append(const DynamicBitSet& other);
        /**
         *  \brief Brief
         *
         *  \return Return_Description
         *
         *  \details Details
         */
        bool any() const;
        /**
         *  \brief Brief
         *
         *  \return Return_Description
         *
         *  \details Details
         */
        bool all() const;
        /**
         *  \brief Brief
         *
         *  \return Return_Description
         *
         *  \details Details
         */
        bool none() const;
        /**
         *  \brief Brief
         *
         *  \param [in] idx Parameter_Description
         *  \return Return_Description
         *
         *  \details Details
         */
        bool test(uint_fast32_t idx) const;
        /**
         *  \brief Brief
         *
         *  \param [in] idx Parameter_Description
         *  \return Return_Description
         *
         *  \details Details
         */
        void reset(uint_fast32_t idx);
        /**
         *  \brief Brief
         *
         *  \param [in] idx Parameter_Description
         *  \return Return_Description
         *
         *  \details Details
         */
        void flip(uint_fast32_t idx);
        /**
         *  \brief Brief
         *
         *  \return Return_Description
         *
         *  \details Details
         */
        void flip();

		/**
		 *  \brief Brief
		 *
		 *  \param [in] newSize Parameter_Description
		 *  \return Return_Description
		 *
		 *  \details Details
		 */
		void resize(uint_fast32_t newSize);
        /**
         *  \brief Brief
         *
         *  \return Return_Description
         *
         *  \details Details
         */
        int_fast32_t count();
        /**
         *  \brief Brief
         *
         *  \return Return_Description
         *
         *  \details Details
         */
        int_fast32_t size() const;
		/**
		 *  \brief Brief
		 *
		 *  \return Return_Description
		 *
		 *  \details Details
		 */
		int_fast32_t capacity() const;
        /**
         *  \brief Brief
         *
         *  \return Return_Description
         *
         *  \details Details
         */
        std::string to_string() const;
		/**
		 *  \brief Brief
		 *
		 *  \return Return_Description
		 *
		 *  \details Details
		 */
		std::deque<uint32_t> getAllSetBits();

        uint64_t clearMask;
        uint64_t cBits=0;
        uint64_t *element=nullptr;
        int_fast32_t cElements=0;
		uint64_t currBitToSet=0;
        bool lazyCount=1;
        uint_fast32_t countSetBits;
		uint_fast32_t resizeStep=32768;
    private:
		const uint64_t OVERFLOW_MAX=0xFFFFFFFFFFFFFFFFLL;
};

std::ostream& operator<<(std::ostream &out,const DynamicBitSet& dSet);

namespace std
{
    template <>
    struct hash<DynamicBitSet>
    {
        size_t operator()(const DynamicBitSet& dynamicSet) const
        {
            return dynamicSet.hashcode();
        }
    };
}

#endif // DYNAMICBITSET_H
