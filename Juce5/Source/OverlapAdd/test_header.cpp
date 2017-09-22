#include<iostream>
#include<assert.h>
#include<algorithm>
//#include<memory>
#include"Buffer.hpp"
using namespace std;
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
	assert(f++ == 7);
	assert(f++ == 7);
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
	auto pre_b = b.begin();
	Buffer<float> c (2,2);
	Buffer<float> e (4,4);
	assert(pre_b == b.begin());
	assert(b.end()-b.begin()==16+8);
	std::for_each(b.begin(),b.end(),[](float v){
		assert(v==0);
	});
	b = Buffer<float> (8,4);
	assert(b.end()-b.begin()==4+8);
	std::for_each(b.begin(),b.end(),[](float v){
		assert(v==0);
	});
	std::copy(a,a+12,b.begin());;
	float* ptr = a;
	std::for_each(b.begin(),b.end(),[&ptr](float v){
		assert(v==*ptr++);
	});
	assert(b.size() == 12);
	assert(b.GetWinSize() == 8);
	assert(b.GetHopSize() == 4);
	
	cout<<"Test Copy/Assign"<<endl;
	Buffer<float> bob (4,4);
//	cout<<b.begin()<<endl;
	cout<<"Assign"<<endl;
//	cout<<b.begin()<<endl;
//	cout<<bob.begin()<<endl;
	
	bob = b;
	cout<<"Copy Constructor"<<endl;
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

