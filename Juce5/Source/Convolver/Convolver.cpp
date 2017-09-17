#include "Convolver.hpp"


Convolver::Convolver(){
	Allocate(1024);
}

Convolver::Convolver(size_t max_size){
	Allocate(max_size);
}
		
Convolver::~Convolver(){
	delete[] _buffer;
}

void Convolver::Reserve (size_t max_size){
	if(max_size  !=_buffer_size){
		delete[] _buffer;
		Allocate(max_size);
	}else{
		_current = _buffer;
	}
}

void Convolver::Transform (double* data, const size_t channels, const size_t length){
//	length /= channels;
	for(int ch=0;ch<channels;++ch){
		gsl_fft_real_radix2_transform(data+ch,channels,length);
	}
}

void Convolver::InverseTransform (double* data, const size_t channels, const size_t length){
//	length /= channels;
	for(int ch=0;ch<channels;++ch){
		gsl_fft_halfcomplex_radix2_inverse(data+ch,channels,length);
	}
}

void Convolver::ComplexMultiply (double* a, const double* b, const size_t channels, const size_t length){
//	lenght /= channels;
	size_t N = length*channels;
	size_t N_over_2 = N*0.5;
	gsl_complex temp_complex;
	unsigned real_idx,img_idx;

	for(int ch=0;ch<channels;++ch){

		a[ch]*=b[ch];		
		real_idx = channels+ch;
		img_idx = N-channels+ch;
//N_over_2 will incre at the end of the while loop
		while(real_idx<N_over_2){
			temp_complex = gsl_complex_mul(gsl_complex_rect(a[real_idx],a[img_idx]),gsl_complex_rect(b[real_idx],b[img_idx]));
			a[real_idx] = temp_complex.dat[0];
			a[img_idx] = temp_complex.dat[1];
			real_idx+=channels;
			img_idx-=channels;
		}
		
		a[N_over_2]*=b[N_over_2];
		++N_over_2;
	}
}
		
void Convolver::ConvolveFreq(const double* freq, const Convolver::Domain output_domain, const size_t channels, const size_t length){
	Transform(_buffer,channels,length);
	ComplexMultiply(_buffer,freq,channels,length);
	if(output_domain==TIME)
		Transform(_buffer,channels,length);
}

double* Convolver::operator++ (){
	if(++_current == _end) return _end;
	return ++_current;
}

double Convolver::operator++ (int){
	if(_current == _end) return *_end;
	return *_current++;
}

void Convolver::Allocate(size_t size){
	_buffer = new double [size];
	_buffer_size = size;
	_current = _buffer;
	_end = _buffer+size;
}

double* Convolver::begin() const{
	return _buffer;
}

double* Convolver::end() const{
	return _end;
}

void Convolver::push_back(double value){
	*_current++ = value;
	if(_current==_end) _current = _buffer;
}

const size_t Convolver::GetBufferSize() const{
	return _buffer_size;
}

Convolver& Convolver::operator=(const Convolver& other){
	Reserve(other.GetBufferSize());
	return *this;
}
/*
void SetInput(const double* data, size_t channels, size_t length){
	std::copy(data,data+(channels*length),buffer);
	_length = length;
	_channels = channels;
}


void SetInput(const double* data, size_t channels, size_t length, const double gain){
	Reserve(length);
	std::for_each(data,data+(channels*length),[buffer, gain](double data){
		
	});
	_length = length;
	_channels = channels;
}
*/
