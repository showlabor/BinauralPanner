#include<Convolver.hpp>


Convolver::Convolver(){
	Allocate(1024);
}

Convolver::Convolver(size_t max_size){
	Allocate(max_size);
}
		
Convolver::~Convolver(){
	delete[] buffer;
}

void Convolver::Reserve (size_t max_size){
	if(max_size > buffer_size){
		delete[] buffer;
		Allocate(max_size);
	}else{
		current = buffer;
	}
}

void Convolver::Transform (double* data, const size_t channels, const size_t length){
//	length /= channels;
	for(int ch=0;ch<channels;++ch){
		gsl_fft_real_radix2_transform(data+ch,channels,length);
	}
}

void Convolver::ComplextMultiply (double* a, const double* b, const size_t channels, const size_t length){
//	lenght /= channels;
	size_t N = length*channels;
	size_t N_over_2 = _N*0.5;
	gsl_complex temp_complex;
	unsigned real_idx,img_idx;

	for(int ch=0;ch<channels;++ch){

		a[ch]*=b[ch];		
		real_idx = channels+ch;
		img_idx = N-channels+ch;
//N_over_2 will incre at the end of the while loop
		while(real_idx<N_over_2){
			temp_complex = gls_complex_mul(gsl_complex_rect(a[real_idx],a[img_idx]),gsl_complex_rect(b[real_idx],b[img_idx]));
			a[real_idx] = temp_complex[0];
			a[img_idx] = temp_complex[1];
			real_idx+=channels;
			img_idx-=channels;
		}
		
		a[N_over_2]*=b[N_over_2];
		++N_over_2;
	}
}
		
void Convolver::ConvolveFreq(const double* freq, const Convolver::Domain output_domain, const size_t channels, const size_t length){
	Transform(buffer,channels,length);
	ComplexMultiply(buffer,freq,channels,length);
	if(output_domain==TIME)
		Transform(buffer,channels,length);
}

double& Convolver::operator++ (){
	if(current == end) return end;
	return ++current;
}

double operator++ (int){
	if(current == end) return end;
	return *current++;
}

void Convolver::Allocate(size_t size){
	buffer = new double [size];
	buffer_size = size;
	current = buffer;
	end = buffer+size;
}

double* Convolver::begin() const{
	return buffer;
}

const double* Convovler::end() const{
	return end;
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
