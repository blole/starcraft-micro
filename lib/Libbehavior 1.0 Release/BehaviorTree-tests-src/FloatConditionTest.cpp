#include "BehaviorTree.h"
#include <boost/test/unit_test.hpp>
#include "TestClass.h"

using namespace BehaviorTree;

BOOST_AUTO_TEST_CASE( FP_CheckClose)
{
	TestClass* t = new TestClass();
	FloatCondition<TestClass>* c1 = new FloatCondition<TestClass>(&TestClass::returnFP5,CLOSE,5.0f);
	c1->init(t);
	BOOST_CHECK_EQUAL(c1->execute(t),BT_SUCCESS);
	delete c1;

	FloatCondition<TestClass>* c2 = new FloatCondition<TestClass>(&TestClass::returnFP5,CLOSE,10.0f);
	c2->init(t);
	BOOST_CHECK_EQUAL(c2->execute(t),BT_FAILURE);
	delete c2;
	delete t;
}

BOOST_AUTO_TEST_CASE( FP_CheckNotClose)
{
	TestClass* t = new TestClass();
	FloatCondition<TestClass>* c1 = new FloatCondition<TestClass>(&TestClass::returnFP5,NOT_CLOSE,5.0f);
	c1->init(t);
	BOOST_CHECK_EQUAL(c1->execute(t),BT_FAILURE);
	delete c1;

	FloatCondition<TestClass>* c2 = new FloatCondition<TestClass>(&TestClass::returnFP5,NOT_CLOSE,10.0f);
	c2->init(t);
	BOOST_CHECK_EQUAL(c2->execute(t),BT_SUCCESS);
	delete c2;

	FloatCondition<TestClass>* c3 = new FloatCondition<TestClass>(&TestClass::returnFP5,NOT_CLOSE,5.000001f);
	c3->init(t);
	BOOST_CHECK_EQUAL(c3->execute(t),BT_FAILURE);
	delete c3;
	delete t;
}

BOOST_AUTO_TEST_CASE( FP_CheckGreater)
{
	TestClass* t = new TestClass();
	FloatCondition<TestClass>* c1 = new FloatCondition<TestClass>(&TestClass::returnFP5,GREATER_THAN_FP,2.0f);
	c1->init(t);
	BOOST_CHECK_EQUAL(c1->execute(t),BT_SUCCESS);
	delete c1;

	FloatCondition<TestClass>* c2 = new FloatCondition<TestClass>(&TestClass::returnFP5,GREATER_THAN_FP,10.0f);
	c2->init(t);
	BOOST_CHECK_EQUAL(c2->execute(t),BT_FAILURE);
	delete c2;
	delete t;
}

BOOST_AUTO_TEST_CASE( FP_CheckLess)
{
	TestClass* t = new TestClass();
	FloatCondition<TestClass>* c1 = new FloatCondition<TestClass>(&TestClass::returnFP5,LESS_THAN_FP,10.0f);
	c1->init(t);
	BOOST_CHECK_EQUAL(c1->execute(t),BT_SUCCESS);
	delete c1;

	FloatCondition<TestClass>* c2 = new FloatCondition<TestClass>(&TestClass::returnFP5,LESS_THAN_FP,-1.0f);
	c2->init(t);
	BOOST_CHECK_EQUAL(c2->execute(t),BT_FAILURE);
	delete c2;
	delete t;
}

BOOST_AUTO_TEST_CASE( FP_CheckLessOrClose)
{
	TestClass* t = new TestClass();
	FloatCondition<TestClass>* c1 = new FloatCondition<TestClass>(&TestClass::returnFP5,LESS_OR_CLOSE,5.0f);
	c1->init(t);
	BOOST_CHECK_EQUAL(c1->execute(t),BT_SUCCESS);
	delete c1;

	FloatCondition<TestClass>* c2 = new FloatCondition<TestClass>(&TestClass::returnFP5,LESS_OR_CLOSE,10.0f);
	c2->init(t);
	BOOST_CHECK_EQUAL(c2->execute(t),BT_SUCCESS);
	delete c2;


	FloatCondition<TestClass>* c3 = new FloatCondition<TestClass>(&TestClass::returnFP5,LESS_OR_CLOSE,4.0f);
	c3->init(t);
	BOOST_CHECK_EQUAL(c3->execute(t),BT_FAILURE);
	delete c3;
	delete t;
}

BOOST_AUTO_TEST_CASE( FP_CheckGreaterOrClose)
{
	TestClass* t = new TestClass();
	FloatCondition<TestClass>* c1 = new FloatCondition<TestClass>(&TestClass::returnFP5,GREATER_OR_CLOSE,5.0f);
	c1->init(t);
	BOOST_CHECK_EQUAL(c1->execute(t),BT_SUCCESS);
	delete c1;

	FloatCondition<TestClass>* c2 = new FloatCondition<TestClass>(&TestClass::returnFP5,GREATER_OR_CLOSE,10.0f);
	c2->init(t);
	BOOST_CHECK_EQUAL(c2->execute(t),BT_FAILURE);
	delete c2;


	FloatCondition<TestClass>* c3 = new FloatCondition<TestClass>(&TestClass::returnFP5,GREATER_OR_CLOSE,4.0f);
	c3->init(t);
	BOOST_CHECK_EQUAL(c3->execute(t),BT_SUCCESS);
	delete c3;

	FloatCondition<TestClass>* c4 = new FloatCondition<TestClass>(&TestClass::returnFP5,GREATER_OR_CLOSE,5.000001f);
	c4->init(t);
	BOOST_CHECK_EQUAL(c4->execute(t),BT_SUCCESS);
	delete c4;
	delete t;
}