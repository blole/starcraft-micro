#include "BehaviorTree.h"
#include <boost/test/unit_test.hpp>
#include "TestClass.h"

using namespace BehaviorTree;

BOOST_AUTO_TEST_CASE( CheckEquals)
{
	TestClass* t = new TestClass();
	IntCondition<TestClass>* c1 = new IntCondition<TestClass>(&TestClass::return5,EQUAL,5);
	c1->init(t);
	BOOST_CHECK_EQUAL(c1->execute(t),BT_SUCCESS);
	delete c1;

	IntCondition<TestClass>* c2 = new IntCondition<TestClass>(&TestClass::return5,EQUAL,10);
	c2->init(t);
	BOOST_CHECK_EQUAL(c2->execute(t),BT_FAILURE);
	delete c2;
	delete t;
}

BOOST_AUTO_TEST_CASE( CheckEquals_Const)
{
	TestClass* t = new TestClass();
	IntCondition<TestClass>* c1 = new IntCondition<TestClass>(&TestClass::return5Const,EQUAL,5);
	c1->init(t);
	BOOST_CHECK_EQUAL(c1->execute(t),BT_SUCCESS);
	delete c1;

	IntCondition<TestClass>* c2 = new IntCondition<TestClass>(&TestClass::return5Const,EQUAL,10);
	c2->init(t);
	BOOST_CHECK_EQUAL(c2->execute(t),BT_FAILURE);
	delete c2;
	delete t;
}

BOOST_AUTO_TEST_CASE( CheckNotEquals)
{
	TestClass* t = new TestClass();
	IntCondition<TestClass>* c1 = new IntCondition<TestClass>(&TestClass::return5,NOT_EQUAL,5);
	c1->init(t);
	BOOST_CHECK_EQUAL(c1->execute(t),BT_FAILURE);
	delete c1;

	IntCondition<TestClass>* c2 = new IntCondition<TestClass>(&TestClass::return5,NOT_EQUAL,10);
	c2->init(t);
	BOOST_CHECK_EQUAL(c2->execute(t),BT_SUCCESS);
	delete c2;
	delete t;
}

BOOST_AUTO_TEST_CASE( CheckGreater)
{
	TestClass* t = new TestClass();
	IntCondition<TestClass>* c1 = new IntCondition<TestClass>(&TestClass::return5,GREATER_THAN,2);
	c1->init(t);
	BOOST_CHECK_EQUAL(c1->execute(t),BT_SUCCESS);
	delete c1;

	IntCondition<TestClass>* c2 = new IntCondition<TestClass>(&TestClass::return5,GREATER_THAN,10);
	c2->init(t);
	BOOST_CHECK_EQUAL(c2->execute(t),BT_FAILURE);
	delete c2;
	delete t;
}

BOOST_AUTO_TEST_CASE( CheckLess)
{
	TestClass* t = new TestClass();
	IntCondition<TestClass>* c1 = new IntCondition<TestClass>(&TestClass::return5,LESS_THAN,10);
	c1->init(t);
	BOOST_CHECK_EQUAL(c1->execute(t),BT_SUCCESS);
	delete c1;

	IntCondition<TestClass>* c2 = new IntCondition<TestClass>(&TestClass::return5,LESS_THAN,-1);
	c2->init(t);
	BOOST_CHECK_EQUAL(c2->execute(t),BT_FAILURE);
	delete c2;
	delete t;
}

BOOST_AUTO_TEST_CASE( CheckLessOrEq)
{
	TestClass* t = new TestClass();
	IntCondition<TestClass>* c1 = new IntCondition<TestClass>(&TestClass::return5,LESS_OR_EQ,5);
	c1->init(t);
	BOOST_CHECK_EQUAL(c1->execute(t),BT_SUCCESS);
	delete c1;

	IntCondition<TestClass>* c2 = new IntCondition<TestClass>(&TestClass::return5,LESS_OR_EQ,10);
	c2->init(t);
	BOOST_CHECK_EQUAL(c2->execute(t),BT_SUCCESS);
	delete c2;


	IntCondition<TestClass>* c3 = new IntCondition<TestClass>(&TestClass::return5,LESS_OR_EQ,4);
	c3->init(t);
	BOOST_CHECK_EQUAL(c3->execute(t),BT_FAILURE);
	delete c3;
	delete t;
}

BOOST_AUTO_TEST_CASE( CheckGreaterOrEq)
{
	TestClass* t = new TestClass();
	IntCondition<TestClass>* c1 = new IntCondition<TestClass>(&TestClass::return5,GREATER_OR_EQ,5);
	c1->init(t);
	BOOST_CHECK_EQUAL(c1->execute(t),BT_SUCCESS);
	delete c1;

	IntCondition<TestClass>* c2 = new IntCondition<TestClass>(&TestClass::return5,GREATER_OR_EQ,10);
	c2->init(t);
	BOOST_CHECK_EQUAL(c2->execute(t),BT_FAILURE);
	delete c2;


	IntCondition<TestClass>* c3 = new IntCondition<TestClass>(&TestClass::return5,GREATER_OR_EQ,4);
	c3->init(t);
	BOOST_CHECK_EQUAL(c3->execute(t),BT_SUCCESS);
	delete c3;
	delete t;
}