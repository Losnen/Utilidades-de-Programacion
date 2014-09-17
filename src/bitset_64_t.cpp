#include "bitset_64_t.hpp"

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>


 /* reverse:  reverse string s in place */
 void reverse(char s[])
 {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }


/* itoa:  convert n to characters in s */
 void itoa(unsigned long n, char s[],int base)
 {
     long i;

     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % base + '0';   /* get next digit */
     } while ((n /= base) > 0);     /* delete it */
     s[i] = '\0';
     reverse(s);
 }

namespace GOMA {

	void bitset_64_t::build(void)
	{
		block_ = new block_t [sz_];
		memset(block_, 0x0, sz_ * sizeof(block_t));
	}

	void bitset_64_t::clean(void)
	{
		if (block_){
			delete [] block_;
			block_ = NULL;
		}
	}

	bitset_64_t::bitset_64_t(void) :
	block_(NULL),
	sz_(1) 
	{	
		build();
	}


	bitset_64_t::bitset_64_t(int sz) :
	block_(NULL), 
	sz_((int) (ceil((double) (sz) / BITSET_BLOCK_SIZE)))
	{
		build();
	}


	bitset_64_t::bitset_64_t(const bitset_64_t& bs) :
	block_(NULL),
	sz_(bs.sz_) 
	{
		block_ = new block_t [sz_];

		memcpy(block_, bs.block_, sz_ * sizeof(block_t));	
	}


	int bitset_64_t::cardinality(void)
	{
		int card = 0;
			
		for(int i = 0; i < sz_; i++)
			card += __builtin_popcount(block_[i]);
			
		return card;
	}

	inline void bitset_64_t::clear(void) {	
		memset(block_, 0x0, sz_ * sizeof(block_t));
	}

	bitset_64_t::~bitset_64_t(void) {
		clean();
	}

	inline void bitset_64_t::insert(int i) {
		set_bit_64(block_, i);		
	}

	void bitset_64_t::insert(const bitset_64_t& bs) {
		
		block_t* b   = block_;
		block_t* bsb = bs.block_;
		
		for (int i = 0; i < sz_; i++){
			(*b) |= (*bsb);
			b ++;
			bsb ++;
		}
	}


	void bitset_64_t::remove(const bitset_64_t& bs) {
		
		block_t* b   = block_;
		block_t* bsb = bs.block_;
		
		for (int i = 0; i < sz_; i++){

			(*b) &= ~(*bsb);
			b ++;
			bsb ++;		
		}
	}

	inline void bitset_64_t::remove(int i) {

		reset_bit_64(block_, i);
	}

	inline bool bitset_64_t::contains(int i) {
		
		return test_bit_64(block_, i);
	}

	bool bitset_64_t::contains(const bitset_64_t& bs) {

		block_t* b   = block_;
		block_t* bsb = bs.block_;
		
		bool contenido = true;	
		
		for (int i = 0; (i < sz_) && (contenido); i++){

			contenido = (((*b) & (*bsb)) == (*bsb));
			b ++;
			bsb ++;		
		}	

		return contenido;
	}


	void bitset_64_t::op_intersec(const bitset_64_t& bs, bitset_64_t& result) {

		result.clear();
		
		block_t* b   = block_;
		block_t* bsb = bs.block_;
		block_t* res = result.block_;
		
		for (int i = 0; i < sz_; i++){

			(*res) = (*b) & (*bsb);
			b ++;
			bsb ++;
			res ++;
		}	
	}

	void bitset_64_t::op_union(const bitset_64_t& bs, bitset_64_t& result) {

		result.clear();
		
		block_t* b   = block_;
		block_t* bsb = bs.block_;
		block_t* res = result.block_;
		
		for (int i = 0; i < sz_; i++){

			(*res) = (*b) | (*bsb);
			b ++;
			bsb ++;
			res ++;
		}
	}

	void bitset_64_t::op_minus(const bitset_64_t& bsj, bitset_64_t& result) {

		result.clear();
		block_t aux;
		
		for (int i = 0; i < sz_; i++){
			
			aux = ~bsj.block_[i];       
			result.block_[i] = (block_[i] & aux);
		}
	}

	#ifdef _DEBUG
	void bitset_64_t::write(ostream& os) {

		const int bsz = sizeof (block_t)*8;

		for (int i = 0; i < sz_; i++) {

			block_t aux = block_[i];

			for (int j = 0; (j < bsz) && (aux != 0); j++) {

				if ((aux & ONE_MASK) == 0x01) {

					os << i * bsz + j;
					os << " ";
				}

				aux = aux >> 1;
			}
		}
	}
	#endif

	bool bitset_64_t::operator==(bitset_64_t& bs) {
	   
		for (int i = 0; i < sz_; i++)
			if (block_[i] != bs.block_[i])
				return false;

		return true;
	}

	
	  const bitset_64_t& bitset_64_t::operator=(const bitset_64_t& bs) {

			sz_=bs.sz_;
		
			block_ = new block_t [sz_];
			memcpy(block_, bs.block_, sz_ * sizeof(block_t));	
						
			return bs;
		}
	

	bitset_64_iterator_t::bitset_64_iterator_t(const bitset_64_t& bs):
	block_(bs.block_),
	cb_(*bs.block_),
	cb_inx_(bs.sz_),
	offset_(0)
	{}

	bitset_64_iterator_t::~bitset_64_iterator_t(void)
	{}

	int bitset_64_iterator_t::get(void)
	{
		int pos;
		int b_end = find_bit_64(&cb_, &pos);
		
		while (b_end){
					
			cb_inx_ --;
			
			if (!cb_inx_) 
				return -1;			
			else{			
				block_ ++;
				offset_ += 64;
				cb_ = *block_;
				b_end = find_bit_64(&cb_, &pos);
			}
		}
		return offset_ + pos;
	}


}

 