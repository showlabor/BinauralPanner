#include<algorithm>
//#include<iostream>
#include<memory>
#include<vector>
#include"Buffer.hpp"
using namespace std;

template<typename T>
class Frame<T>::impl{
	public:
		impl(const T* b,const  T* e,const T* pb,const T* pe):begin(b),end(e),phy_begin(pb),phy_end(pe),current(b){}
		~impl(){}
		void assign (const T* b,const  T* e,const T* pb,const T* pe){
			begin = b;
			end = e;
			phy_begin = pb;
			phy_end = pe;
			current = b;
		}

		const T* post_incre (){
			if(++current == end) return end-1;
			if(current==phy_end) current = phy_begin;
			return current;
		}

		T pre_incre(){
			if(current == end) return *(end-1);
			T temp = *current;
			if(++current == phy_end) current = phy_begin;
			return temp;
		}
		
		inline const T* GetNow() {return current;}
		inline const T* GetBegin() {return begin;}
		inline const T* GetEnd() {return end;}
		inline const T* GetPhyBegin() {return phy_begin;}
		inline const T* GetPhyEnd() {return phy_end;}
	private:
		const T* begin;
		const T* end;
		const T* phy_begin;
		const T* phy_end;
		const T* current;

};

template<typename T>
Frame<T>::Frame(const T* b,const  T* e,const T* pb,const T* pe):pimpl(unique_ptr<impl>(new impl(b,e,pb,pe))){}

template<typename T>
Frame<T>::Frame(const Frame<T>& other):pimpl(unique_ptr<impl>(new impl(other.begin(),other.end(),other.phy_begin(),other.phy_end()))){}

template<typename T>
Frame<T>::~Frame(){}

template<typename T>
Frame<T>& Frame<T>::operator= (const Frame<T>& other){
	if(this!=&other){	
		pimpl->assign(other.begin(),other.end(),other.phy_begin(),other.phy_end());
	}
	return *this;
}

template<typename T>
inline const T& Frame<T>::now() { return *pimpl->GetNow();}

template<typename T>
const T& Frame<T>::operator++(){
	return *(pimpl->post_incre());
}

template<typename T>
T Frame<T>::operator++(int){
	return pimpl->pre_incre();
}	

template<typename T>
inline const T* Frame<T>::begin() const { return pimpl->GetBegin();} 

template<typename T>
inline const T* Frame<T>::end() const { return pimpl->GetEnd();}

template<typename T>
inline const T* Frame<T>::phy_begin() const { return pimpl->GetPhyBegin();}

template<typename T>
inline const T* Frame<T>::phy_end() const { return pimpl->GetPhyEnd();}

template class Frame<double>;
template class Frame<float>;

/*
template<typename T>
class Frame{
	public:
		Frame(const T* b,const  T* e,const T* pb,const T* pe):_begin(b),_end(e),_phy_begin(pb),_phy_end(pe){
			_current=b;
		}

		Frame<T>& operator= (const Frame& other){
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
*/

template<typename T>
class Buffer<T>::impl{
	public:
		impl(size_t w, size_t h){
			SetSize(w,h);
			buffer = new T[size()];
			fill(buffer,end(),0);
			reset();
		}

		impl(const Buffer<T>& other){
			buffer = new T[other.size()];
			winSize = other.GetWinSize();
			hopSize =  other.GetHopSize();
			frames.reserve(winSize/hopSize);
			std::copy(other.begin(),other.end(),buffer);
			/*	T* ptr = _buffer;
				T* o_ptr = other.begin();
				while(o_ptr!=other.end()){
					*ptr++=*o_ptr++;
				}
			*/	
			reset();
		}

		~impl(){
			delete[] buffer;
		}

		void SetSize(const size_t w, const size_t h){
			winSize=w;
			hopSize=h;
			frames.reserve(winSize/hopSize);
		}
		
		inline void reset(){
			current = buffer;
			frame_begin = end()-hopSize;
			frame_end = buffer+winSize-hopSize;
		}

		void resize(size_t w, size_t h){
			if(size() != w+h){
				SetSize(w, h);
				delete[] buffer;
				buffer = new T[size()];
				std::fill(buffer,end(),0);
			}	
			reset();
		}

		void push_back(const T* data){
		//	T* frame_begin = _current-_hopSize;
		//	if(frame_begin<begin()) frame_begin += size();
		//	T* frame_end = frame_begin + _winSize;
		//	if(frame_end>=end()) frame_begin -= size();
			
			if(frames.empty()){
				while(frames.size()!=frames.capacity()){
					frames.push_back(Frame<T>(frame_begin,frame_end,buffer,end()));
					check_frame_bound();
				}

			}else{
				for(auto &f:frames){
					f=Frame<T>(frame_begin,frame_end,buffer,end());
					check_frame_bound();
				}
			}
		
			//copy data to buffer	
			const T* data_end = data+winSize;
			while(data!=data_end){
				*current = *data++;
				if(++current==end())
					reset();
			}
		}

		void assign (const Buffer<T>& other){
		//	if(this!=&other){
				if(winSize!=0)
					 delete[] buffer;
				buffer = new T[other.size()];
				winSize = other.GetWinSize();
				hopSize =  other.GetHopSize();
				frames.reserve(winSize/hopSize);
				std::copy(other.begin(),other.end(),buffer);
				reset();
		//	}
		//	return *this;
		}

		void check_frame_bound (){
			frame_begin += hopSize;
			if(frame_begin >= end()) frame_begin -= size();
			frame_end += hopSize;
			if(frame_end >= end()) frame_end -= size();
		}

		inline size_t size() {return winSize+hopSize;}
		inline T* end(){return buffer+size();}
		
		T* buffer, *current, *frame_begin, *frame_end;
		size_t winSize = 0;
		size_t hopSize = 0;
		vector<Frame<T>> frames;
};

template<typename T>
Buffer<T>::Buffer():pimpl(unique_ptr<impl>(new impl(512,256))){}

template<typename T>
Buffer<T>::Buffer(size_t winSize, size_t hopSize):pimpl(unique_ptr<impl>(new impl(winSize, hopSize))){}

template<typename T>
Buffer<T>::Buffer(const Buffer<T>& other):pimpl(unique_ptr<impl>(new impl(other))){}

template<typename T>
Buffer<T>::~Buffer(){}

template<typename T>
void Buffer<T>::resize(size_t w, size_t h){pimpl->resize(w,h);}

template<typename T>
void Buffer<T>::push_back(const T* data){pimpl->push_back(data);}

template<typename T>
Buffer<T>& Buffer<T>::operator= (const Buffer<T>& other){
	if(this!=&other) pimpl->assign(other);
	return *this;
}

template<typename T>
void Buffer<T>::reset(){pimpl->reset();}

template<typename T>
inline T* Buffer<T>::begin() const {return pimpl->buffer;}

template<typename T>
inline T* Buffer<T>::end() const {return pimpl->end();}

template<typename T>
inline size_t Buffer<T>::GetWinSize() const {return pimpl->winSize;}

template<typename T>
inline size_t Buffer<T>::GetHopSize() const {return pimpl->hopSize;}

template<typename T>
inline const vector<Frame<T>>& Buffer<T>::get_frames () const {return pimpl->frames;}

template<typename T>
inline size_t Buffer<T>::size() const{return pimpl->size();}

template class Buffer<float>;
template class Buffer<double>;

/*
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
		//	T* frame_begin = _current-_hopSize;
		//	if(frame_begin<begin()) frame_begin += size();
		//	T* frame_end = frame_begin + _winSize;
		//	if(frame_end>=end()) frame_begin -= size();
			
			if(_frames.empty()){
				while(_frames.size()!=_frames.capacity()){
					_frames.push_back(Frame<T>(_frame_begin,_frame_end,begin(),end()));
					check_frame_bound();
				}

			}else{
				for(auto &f:_frames){
					f=Frame<T>(_frame_begin,_frame_end,begin(),end());
					check_frame_bound();
				}
			}
		
			//copy data to buffer	
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
			
				std::copy(other.begin(),other.end(),_buffer);
				reset();
			}
			return *this;
		}

		inline void reset(){
			_current = _buffer;
			_frame_begin = end()-_hopSize;
			_frame_end = begin()+_winSize-_hopSize;
		}
		inline T* begin() const {return _buffer;}
		inline T* end() const {return _buffer+_winSize+_hopSize;}
		inline size_t GetWinSize() const {return _winSize;}
		inline size_t GetHopSize() const {return _hopSize;}
		inline const vector<Frame<T>>& get_frames () const {return _frames;}
		inline size_t size() const{return _winSize+_hopSize;}

	
	private:
		void SetSize(const size_t winSize, const size_t hopSize){
			_winSize=winSize;
			_hopSize=hopSize;
			_frames.reserve(winSize/hopSize);
		}
	
		void check_frame_bound (){
			_frame_begin += _hopSize;
			if(_frame_begin >= end()) _frame_begin -= size();
			_frame_end += _hopSize;
			if(_frame_end >= end()) _frame_end -= size();
		}		
	

		size_t _winSize=0;
		size_t _hopSize=0;
		 T* _buffer,*_current, *_frame_begin, *_frame_end;
		vector<Frame<T>> _frames;
		


};
*/
/*
template class Buffer<float>;
template class Buffer<double>;
template class Frame<float>;
template class Frame<double>;
*/
/*
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

	cout<<"Test Copy/Assign"<<endl;
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

	cout<<"Test Push Back"<<endl;
	float rand[] = {0.3,-0.4,5,-0.5,0.345,3,0.001,-9.3};
	bob.push_back(rand);

	ptr=rand;
	std::for_each(bob.begin(),bob.begin()+8,[&ptr](float v){
		assert(v==*ptr++);
	});
	ptr=a+8;
	std::for_each(bob.begin()+8,bob.end(),[&ptr](float v){
		assert(v==*ptr++);
	});

	cout<<"Test Get Frames"<<endl;
	auto f_v = bob.get_frames();
	assert(f_v.size() == 2);
	auto fr = f_v[0];
	for(int i=0;i<4;++i){
		assert(fr++==a[i+8]);
	}
	for(int i=0;i<4;++i){
		assert(fr++==rand[i]);
	}
	fr = f_v[1];
	std::for_each(rand,rand+8,[&fr](float v){
		assert(v==fr++);
	});

	cout<<"Test Second Push"<<endl;
	bob.push_back(rand);
	ptr=rand+4;
	std::for_each(bob.begin(),bob.begin()+4,[&ptr](float v){
		assert(v==*ptr++);
	});
	ptr=rand+4;
	std::for_each(bob.begin()+4,bob.begin()+8,[&ptr](float v){
		assert(v==*ptr++);
	});
	ptr=rand;
	std::for_each(bob.begin()+8,bob.end(),[&ptr](float v){
		assert(v==*ptr++);
	});


	cout<<"Test Get Frames"<<endl;
	f_v = bob.get_frames();
	assert(f_v.size() == 2);
	for(auto f:f_v){
		for(int i=0;i<8;++i){
			cout<<f++<<" ";
		}
		cout<<endl;
	}

	cout<<"Test Third Push"<<endl;
	bob.push_back(rand);
	cout<<"Test Get Frames"<<endl;
	f_v = bob.get_frames();
	assert(f_v.size() == 2);
	for(auto f:f_v){
		for(int i=0;i<8;++i){
			cout<<f++<<" ";
		}
		cout<<endl;
	}

	cout<<"All pass"<<endl;

	

	return 0;
}

#endif
*/
