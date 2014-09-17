#pragma once

#ifdef _DEBUG
	#include <cassert>
	#include <iostream>
	#include <iomanip>
	#include <cstring>	
	
	using namespace std;
	
    #define IT_PER_LINE 8	
	#define WIDE_OUTPUT 9
#endif 

#include <cstdint>


extern "C" void set_bit_64  (int64_t*, int i);
extern "C" void reset_bit_64(int64_t*, int i);
extern "C" bool test_bit_64 (int64_t*, int i);
extern "C" bool find_bit_64 (int64_t*, int* pos);


#define BITSET_BLOCK_SIZE 64
#define ONE_MASK 0x01

typedef int64_t block_t;

namespace GOMA {

	class bitset_64_t {
	public:
		block_t* block_;
		int sz_;

	public:
		bitset_64_t(void);
		bitset_64_t(int max);
		bitset_64_t(const bitset_64_t& bs);

		~bitset_64_t(void);

		void insert(int i);
		void insert(const bitset_64_t& bs);
		
		void remove(int i);
		void remove(const bitset_64_t& bs);
		
		bool contains(int i);
		bool contains(const bitset_64_t& a);
		
		void clear(void);
		
		void op_intersec(const bitset_64_t& bsj, bitset_64_t& result);
		void op_union   (const bitset_64_t& bsj, bitset_64_t& result);
		void op_minus   (const bitset_64_t& bsj, bitset_64_t& result);
		
		int cardinality(void);
		
	#ifdef _DEBUG	
		void write(std::ostream&);
	#endif	
		
		bool operator==(bitset_64_t& bs);	
		const bitset_64_t& operator=(const bitset_64_t& bs);	

	private:
		void build(void);
		void clean(void);
	};
	 
	#define END_BITSET_ITERATOR -1
	  
	class bitset_64_iterator_t {
	public:
		
		block_t*    block_;      // Bitset
		
		block_t 	cb_;         // Current block
		int         cb_inx_;     // Current block inx
		int         offset_;
		
	public:
		bitset_64_iterator_t(const bitset_64_t& bs);
		virtual ~bitset_64_iterator_t(void);

		int get(void);
	}; 

}