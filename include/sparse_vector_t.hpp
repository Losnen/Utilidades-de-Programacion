#pragma once

#ifndef DEFAULT_BUFF_SPARSE_VECTOR_SZ
	#define DEFAULT_BUFF_SPARSE_VECTOR_SZ 10
#endif

#ifdef _DEBUG
	#include <cassert>
	#include <iostream>
	#include <iomanip>
	#include <cstring>
	
	using namespace std;
	
    #define IT_PER_LINE 8	
	#define WIDE_OUTPUT 9
#endif 

#include <cstdio>

namespace GOMA {

	class sparse_vector_t {
		
	public:
		int     nz_;      /** Number of non-null items   */
		int     max_sz_;   /** Maximum potential size     */	

		double* val_;     /** Array of non-null items    */
		int* 	inx_;     /** Indices of non-null items  */


	public:
		sparse_vector_t(int sz, double* v, double eps);
		sparse_vector_t(int nz, int* inx, double* val);
		sparse_vector_t(void);	
		sparse_vector_t(const sparse_vector_t&);

		virtual ~sparse_vector_t(void);

	public:
		void set(int nz, int* inx, double* val);

		double scal_prod(double* v);
		double scal_prod(sparse_vector_t& si);
		double scal_prod(sparse_vector_t* si);

		void sort(void);
		
		void set(int pos, double val);
		
	private:
		void clean(void);
		void build(int nz);
			

	#ifdef _DEBUG
	public:
		void write(ostream& os);
	#endif	
	};
	
}
