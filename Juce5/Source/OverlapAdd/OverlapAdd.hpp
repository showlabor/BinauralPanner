#ifndef INCLUDED_OVERLAPADD
#define INCLUDED_OVERLAPADD

#include<list>
#include<vector>
#include<iterator>

using namespace std;

template<typename T>
class OverlapAdd{
	public:
		
		OverlapAdd();
		OverlapAdd(size_t _hopSize, size_t _winSize);
		~OverlapAdd();

		void SetHopSize(size_t _hopSize);
		void SetWinSize(size_t _winSize);
		
		void AddFrame (const vector<T>& data);
		void GetFrame (vector<T>& data);
		void GetFrame (T* begin, T* end);
		
		OverlapAdd& operator=(const OverlapAdd other);	

	private:
		list<vector<T>> _frames;
		size_t _hopSize, _winSize;
		vector<T> winfun;
		vector<T> overlap_compensate_fun;
};
#endif
