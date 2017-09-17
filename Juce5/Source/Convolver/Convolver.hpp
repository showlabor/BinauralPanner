#ifndef CONVOLVER_INCLUDED
#define CONVOLVER_INCLUDED

#include<gls/gsl_fft_real.h>
#include<list>

using namespace std;

class Convolver{
	public:
		typedef short Domain;
		const Domain TIME = 0;
		const Domain FREQ = 1;
		Convolver();
		Convolver(size_t max_size);
		~Convolver();
		void Reserve (size_t max_size);
		void Transform (double* data, const size_t channels,const size_t length);
		void ComplextMultiply (double* a, const double* b, size_t channels, size_t length);
		void ConvolveFreq(const double* freq, const size_t channels, const size_t length);
		double& operator++ ();
		double operator++ (int);
		double* begin() const;
		const double* end() const;
	private:
		void Allocate(size_t size);
		double* buffer, end, current;
		size_t buffer_size;

};

#endif
