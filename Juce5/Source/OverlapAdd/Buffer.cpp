#include<algorithm>
#include<iostream>
using namespace std;


template<typename T>
class Frame{
	public:
		Frame(const T* b,const  T* e,const T* pb,const T* pe):_begin(b),_end(e),_phy_begin(pb),_phy_end(pe){
			_current=b;
		}

		Frame<T>& operator= (const Frame other){
			if(this!=&other){
				_begin = other.begin();
				_end = other.end();
				_phy_begin = other.phy_begin();
				_phy_end = other.phy_end();
				_current = _begin;
			}
			return *this;
		}
		
		const T& now () {return *_current;}
		
		const T& operator++(){
			if(++_current == _end) return *_end;
			if(_current==_phy_end) _current = _phy_begin;
			return *_current;
		}

		T operator++(int){
			if(_current == _end) return *_end;
			T temp = *_current;
			if(++_current == _phy_end) _current = _phy_begin;
			return temp;
		}

		inline const T* begin() const { return _begin;} 
		inline const T* end() const { return _end;}
		inline const T* phy_begin() const { return _phy_begin;}
		inline const T* phy_end() const { return _phy_end;}

	private:
		const T* _current;
		const T* _begin;
		const T* _end;
		const T* _phy_begin;
		const T* _phy_end;
};

template<typename T>
class Buffer{
	public:
		Buffer(){
			SetSize(512,256);
			_buffer = new T [size()];
			std::fill(begin(),end(),0);
			reset();
		}

		Buffer(size_t winSize, size_t hopSize){
			SetSize(winSize,hopSize);
			_buffer = new T [size()];
			std::fill(begin(),end(),0);
			reset();
						
		}	

		Buffer(const Buffer<T>& other){
			_buffer = new T[other.size()];
			_winSize = other.GetWinSize();
			_hopSize =  other.GetHopSize();
			_frames.reserve(_winSize/_hopSize);
			std::copy(other.begin(),other.end(),begin());
			/*	T* ptr = _buffer;
				T* o_ptr = other.begin();
				while(o_ptr!=other.end()){
					*ptr++=*o_ptr++;
				}
			*/	
			reset();
		}	

		~Buffer(){delete[] _buffer;}
			
		void resize(size_t winSize, size_t hopSize){
			if(size() != winSize+hopSize){
				SetSize(winSize, hopSize);
				delete[] _buffer;
				_buffer = new T[size()];
				std::fill(begin(),end(),0);
			}	
			reset();
		}
		void push_back(const T* data){
			T* frame_begin = _current;
			T* frame_end = frame_begin + _winSize;
			
			if(_frames.empty()){
				while(_frames.size()!=_frames.capacity()){
					_frames.push_back(Frame<T>(frame_begin,frame_end,begin(),end()));
					check_frame_bound(frame_begin,frame_end);
				}

			}else{
				for(auto &f:_frames){
					f=Frame<T>(frame_begin,frame_end,begin(),end());
					check_frame_bound(frame_begin,frame_end);
				}
			}
	
			const T* data_end = data+_winSize;
			while(data!=data_end){
				*_current = *data++;
				if(++_current==end())
					reset();
			}
		}
			

		Buffer<T>& operator= (const Buffer<T>& other){
			if(this!=&other){
				if(_winSize!=0)
					 delete[] _buffer;
				_buffer = new T[other.size()];
				_winSize = other.GetWinSize();
				_hopSize =  other.GetHopSize();
				_frames.reserve(_winSize/_hopSize);
				std::copy(other.begin(),other.end(),begin());
			/*	T* ptr = _buffer;
				T* o_ptr = other.begin();
				while(o_ptr!=other.end()){
					*ptr++=*o_ptr++;
				}
			*/	reset();
			}
			return *this;
		}

		inline void reset(){_current = _buffer;}
		inline T* begin() const {return _buffer;}
		inline T* end() const {return _buffer+_winSize+_hopSize;}
		inline size_t GetWinSize() const {return _winSize;}
		inline size_t GetHopSize() const {return _hopSize;}
		inline const vector<Frame<T>>& get_frames (){return _frames;}
		inline int size() const{return _winSize+_hopSize;}

	private:
	
		void SetSize(const size_t winSize, const size_t hopSize){
			_winSize=winSize;
			_hopSize=hopSize;
			_frames.reserve(winSize/hopSize);
		}
	
		void check_frame_bound (T* frame_begin, T* frame_end){
			frame_begin += _hopSize;
			if(frame_begin >= end()) frame_begin -= _winSize;
			frame_end += _hopSize;
			if(frame_begin >= end()) frame_begin -= _winSize;
		}		


		size_t _winSize=0;
		size_t _hopSize=0;
		 T* _buffer,*_current;
		vector<Frame<T>> _frames;
		


};

#ifdef TEST
#include<iostream>
#include<assert.h>
int main(){
	cout<<"Test Frame"<<endl;
	float a [] = {1,2,3,4,5,6,7,8,9,10,11,12};
	cout<<"Test Constructor"<<endl;
	Frame<float> f (a+1,a+4,a,a+10);
	std::for_each(a+1,a+4,[&f](float v){
		assert(v==f++);
	});
	f = Frame<float> (a,a+7,a,a+10);
	std::for_each(a,a+7,[&f](float v){
		assert(v==f.now());
		++f;
	});
	assert(f++ == 8);
	assert(f++ == 8);
	f = Frame<float> (a,a+6,a,a+10);
	std::for_each(a,a+6,[&f](float v){
		assert(v==f.now());
		++f;
	});
	f = Frame<float> (a+4,a+7,a,a+10);
	std::for_each(a+4,a+7,[&f](float v){
		assert(v==f++);
	});
	f = Frame<float> (a+8,a+2,a,a+10);
	std::for_each(a+8,a+10,[&f](float v){
		assert(v==f++);
	});
	std::for_each(a,a+2,[&f](float v){
		assert(v==f++);
	});

	cout<<"Test Buffer"<<endl;
	Buffer<float> b (16,8);
	assert(b.end()-b.begin()==16+8);
	std::for_each(b.begin(),b.end(),[](float v){
		assert(v==0);
	});
	b = Buffer<float> (8,4);
	assert(b.end()-b.begin()==4+8);
	std::for_each(b.begin(),b.end(),[](float v){
		assert(v==0);
	});

	std::copy(a,a+12,b.begin());
	float* ptr = a;
	std::for_each(b.begin(),b.end(),[&ptr](float v){
		assert(v==*ptr++);
	});

	assert(b.size() == 12);
	assert(b.GetWinSize() == 8);
	assert(b.GetHopSize() == 4);

	cout<<"Test Copy"<<endl;
	Buffer<float> bob ;
	bob = b;
	Buffer<float> bab = bob;
	ptr = a;
	std::for_each(bob.begin(),bob.end(),[&ptr](float v){
		assert(v==*ptr++);
	});
	ptr = a;
	std::for_each(bab.begin(),bab.end(),[&ptr](float v){
		assert(v==*ptr++);
	});
	cout<<"All pass"<<endl;

	

	return 0;
}

#endif
