#ifndef INCLUDED_BUFFER
#define INCLUDED_BUFFER
#include<memory>
template<typename T>
class Frame{
	private:
		class impl;
		std::unique_ptr<impl> pimpl;
	public:
		Frame(const T* b,const  T* e,const T* pb,const T* pe);
		Frame(const Frame<T>& other);
		Frame<T>& operator= (const Frame<T>& other);
		~Frame();
		const T& now (); 
		const T& operator++();
		T operator++(int);
		const T* begin() const; 
		const T* end() const ;
		const T* phy_begin() const ;
		const T* phy_end() const ;

};

template<typename T>
class Buffer{
	private:
		class impl;
		std::unique_ptr<impl> pimpl;
	public:
		Buffer();
		Buffer(size_t winSize, size_t hopSize);
		Buffer(const Buffer<T>& other);
		~Buffer();
		void resize(size_t winSize, size_t hopSize);
		void push_back(const T* data);
		Buffer<T>& operator= (const Buffer<T>& other);
		void reset();
		T* begin() const; 
		T* end() const ;
		size_t GetWinSize() const ;
		size_t GetHopSize() const ;
		const std::vector<Frame<T>>& get_frames () const; 
		size_t size() const;

};

#endif

