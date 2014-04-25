#ifndef TESTCLASS_H
#define TESTCLASS_H

class TestClass
{
public:
	TestClass(){};
	int return5() {return 5;};
	int return5Const() const {return 5;};
	bool returnTrue() {return true;};
	bool returnFalse() {return false;};
	float returnFP5(){return 5.0;};
};

#endif