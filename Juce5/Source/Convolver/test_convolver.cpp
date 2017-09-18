#include<iostream>
#include<iterator>
#include<algorithm>
#include"Convolver.hpp"
#include<assert.h>

int main(){
	cout<<"Test Constructor"<<endl;
	Convolver c ;
	c = Convolver (2048);
	assert(c.GetBufferSize()==2048);
	c = Convolver(8);
	assert(c.GetBufferSize()==8);

	cout<<"Test complex mul"<<endl;
	double a[] = {1,2,3,1,2,3,4,1};
	double b[] = {6,7,8,6,7,8,7,6};


	c.ComplexMultiply(a,b,1,8);
	
	double ans[] = {6,8,-4,-18,14,26,53,19};
	double* ptr = ans;
	std::for_each(a,a+8,[&ptr](double v){
		assert(*ptr++==v);
	});
	
	double dc[] = {1,2,3,4,5,6,7,8};
	double e[] = {3,2,1,2,3,2,3,1};
	c.ComplexMultiply(dc,e,2,4);
	double ans_b[] = {3,4,-18,0,15,12,16,20};
	ptr=ans_b;


	std::for_each(dc,dc+8,[&ptr](double v){
		assert(*ptr++==v);
	});
	
	
	cout<<"Test Assign"<<endl;

	c = Convolver(8);
	std::for_each(ans,ans+8,[&c](double v){
		c.push_back(v);
	});

	ptr = ans;
	std::for_each(c.begin(),c.end(),[&ptr](double v){
		assert(*ptr++==v);
	});

	std::for_each(ans_b,ans_b+8,[&c](double v){
		c.push_back(v);
	});

	ptr = ans_b;
	std::for_each(c.begin(),c.end(),[&ptr](double v){
		assert(*ptr++==v);
	});

	cout<<"Test Transform"<<endl;
	c.Transform(ans_b,2,4);
	c.InverseTransform(ans_b,2,4);
	ptr = ans_b;
	std::for_each(c.begin(),c.end(),[&ptr](double v){
		assert(*ptr++==v);
	});

	cout<<"Test Convolver"<<endl;
	c = Convolver(16);
	double data[] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	double ir[] = {16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
	c.Transform(ir,1,16);
	std::for_each(data,data+16,[&c](double v){
		c.push_back(v);
	});
	
	c.ConvolveFreq(ir,Convolver::TIME,1,16);
	double expect = 16;
	std::for_each(c.begin(),c.end(),[&expect](double v){
		double compare = v-expect--;
		if(compare <0) compare*=-1;
		assert(compare < 0.00001 );
	});

	std::for_each(data,data+16,[&c](double v){
		c.push_back(v);
	});

	double il_ir[] = {8,1,7,2,6,3,5,3,4,5,3,6,2,7,1,8};
	c.Transform(il_ir,2,8);
	c.ConvolveFreq(il_ir,Convolver::TIME,2,8);
	double il_exp[] = {8,0,7,0,6,0,5,0,4,0,3,0,2,0,1,0};
	ptr = il_exp;
	std::for_each(c.begin(),c.end(),[&ptr](double v){
		double compare = v-*ptr++;
		if(compare <0) compare*=-1;
		assert(compare < 0.00001 );
	});
	
	cout<<"All pass"<<endl;

	return 0;
}
