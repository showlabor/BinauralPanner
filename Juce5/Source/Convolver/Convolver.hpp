#ifndef CONVOLVER_INCLUDED
#define CONVOLVER_INCLUDED

#include<gsl/gsl_fft_real.h>
#include<gsl/gsl_fft_halfcomplex.h>
#include<gsl/gsl_complex.h>
#include<gsl/gsl_complex_math.h>
#include<list>

using namespace std;

class Convolver{
	public:
		typedef short Domain;
		const static Domain TIME = 0;
		const static Domain FREQ = 1;
		Convolver();
		Convolver(size_t max_size);
		~Convolver();
		void Reserve (size_t max_size);
		void Transform (double* data, const size_t channels,const size_t length);
		void InverseTransform (double* data, const size_t channels,const size_t length);
		void ComplexMultiply (double* a, const double* b, size_t channels, size_t length);
		void ConvolveFreq(const double* freq, Domain output_domain,const size_t channels, const size_t length);
		double* operator++ ();
		double operator++ (int);
		const size_t GetBufferSize() const;
		Convolver& operator= (const Convolver& other);
		double* begin() const;
		double* end() const;
		void push_back (double value);
	private:
		void Allocate(size_t size);
		double* _buffer, * _end, *_current;
		size_t _buffer_size;

};

#endif
