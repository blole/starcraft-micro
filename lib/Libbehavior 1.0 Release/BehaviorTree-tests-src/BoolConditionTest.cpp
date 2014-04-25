#include "BehaviorTree.h"
#include <boost/test/unit_test.hpp>
#include "TestClass.h"

using namespace BehaviorTree;

BOOST_AUTO_TEST_CASE(success)
{
	TestClass* t = new TestClass();
	BoolCondition<TestClass>* c1 = new BoolCondition<TestClass>(&TestClass::returnTrue,true);
	c1->init(t);
	BOOST_CHECK_EQUAL(c1->execute(t),BT_SUCCESS);
	delete c1;

	BoolCondition<TestClass>* c2 = new BoolCondition<TestClass>(&TestClass::returnFalse,false);
	c2->init(t);
	BOOST_CHECK_EQUAL(c2->execute(t),BT_SUCCESS);
	delete c2;
	delete t;
}

BOOST_AUTO_TEST_CASE(failure)
{
	TestClass* t = new TestClass();
	BoolCondition<TestClass>* c1 = new BoolCondition<TestClass>(&TestClass::returnTrue,false);
	c1->init(t);
	BOOST_CHECK_EQUAL(c1->execute(t),BT_FAILURE);
	delete c1;

	BoolCondition<TestClass>* c2 = new BoolCondition<TestClass>(&TestClass::returnFalse,true);
	c2->init(t);
	BOOST_CHECK_EQUAL(c2->execute(t),BT_FAILURE);
	delete c2;
	delete t;
}