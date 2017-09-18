#include"OverlapAdd.hpp"


template<typename T>
OverlapAdd<T>::OverlapAdd(){
}

template<typename T>
OverlapAdd<T>::OverlapAdd(size_t _hopSize, size_t _winSize){

}

template<typename T>
OverlapAdd<T>::~OverlapAdd(){

}

template<typename T>
void OverlapAdd<T>::SetHopSize(size_t _hopSize){

}


template<typename T>
void OverlapAdd<T>::SetWinSize(size_t _winSize){

}
		
template<typename T>	
void OverlapAdd<T>::AddFrame (const vector<T>& data){

}

template<typename T>
void OverlapAdd<T>::GetFrame (vector<T>& data){

}

template<typename T>
void OverlapAdd<T>::GetFrame (T* begin, T* end){

}

template<typename T>	
OverlapAdd& OverlapAdd<T>::operator=(const OverlapAdd other){

}	
/*
	private:
		list<vector<T>> _frames;
		size_t _hopSize, _winSize;
		vector<T> winfun;
		vector<T> overlap_compensate_fun;
*/
