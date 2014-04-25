#include "BehaviorTree.h"
#include <boost/test/unit_test.hpp>


using namespace BehaviorTree;


BOOST_AUTO_TEST_CASE( ZeroLimit )
{
	CountLimitNode* node = new CountLimitNode(0);
	node->addChild(new SuccessAfter(0));
	node->init(NULL);

	BOOST_CHECK_EQUAL(node->execute(NULL),BT_FAILURE);

	delete node;
}

BOOST_AUTO_TEST_CASE( ThreeLimit )
{
	CountLimitNode* node = new CountLimitNode(3);
	node->addChild(new SuccessAfter(0));
	
	for (int i =0 ; i<2 ; i++)
	{
		node->init(NULL);
		BOOST_CHECK_EQUAL(node->execute(NULL),BT_SUCCESS);
		BOOST_CHECK_EQUAL(node->execute(NULL),BT_SUCCESS);
		BOOST_CHECK_EQUAL(node->execute(NULL),BT_SUCCESS);
		BOOST_CHECK_EQUAL(node->execute(NULL),BT_FAILURE);
	}
	delete node;
}

BOOST_AUTO_TEST_CASE( ThreeLimitWithRuntime )
{
	CountLimitNode* node = new CountLimitNode(3);
	node->addChild(new SuccessAfter(1));
	
	for (int i =0 ; i<2 ; i++)
	{
		node->init(NULL);
		BOOST_CHECK_EQUAL(node->execute(NULL),BT_RUNNING);
		BOOST_CHECK_EQUAL(node->execute(NULL),BT_SUCCESS);
		BOOST_CHECK_EQUAL(node->execute(NULL),BT_RUNNING);
		BOOST_CHECK_EQUAL(node->execute(NULL),BT_SUCCESS);
		BOOST_CHECK_EQUAL(node->execute(NULL),BT_RUNNING);
		BOOST_CHECK_EQUAL(node->execute(NULL),BT_SUCCESS);
		BOOST_CHECK_EQUAL(node->execute(NULL),BT_FAILURE);
		BOOST_CHECK_EQUAL(node->execute(NULL),BT_FAILURE);
	}

	delete node;
}

BOOST_AUTO_TEST_CASE( ThreeLimitWithRuntime_Failure )
{
	CountLimitNode* node = new CountLimitNode(3);
	node->addChild(new FailureAfter(1));

	for (int i =0 ; i<2 ; i++)
	{
		node->init(NULL);
		BOOST_CHECK_EQUAL(node->execute(NULL),BT_RUNNING);
		BOOST_CHECK_EQUAL(node->execute(NULL),BT_FAILURE);
		BOOST_CHECK_EQUAL(node->execute(NULL),BT_RUNNING);
		BOOST_CHECK_EQUAL(node->execute(NULL),BT_FAILURE);
		BOOST_CHECK_EQUAL(node->execute(NULL),BT_RUNNING);
		BOOST_CHECK_EQUAL(node->execute(NULL),BT_FAILURE);
		BOOST_CHECK_EQUAL(node->execute(NULL),BT_FAILURE);
		BOOST_CHECK_EQUAL(node->execute(NULL),BT_FAILURE);
	}

	delete node;
}

BOOST_AUTO_TEST_CASE( ThreeLimitDisableReinit )
{
	CountLimitNode* node = new CountLimitNode(3,false);
	node->addChild(new SuccessAfter(0));
	
	node->init(NULL);
	BOOST_CHECK_EQUAL(node->execute(NULL),BT_SUCCESS);
	BOOST_CHECK_EQUAL(node->execute(NULL),BT_SUCCESS);
	BOOST_CHECK_EQUAL(node->execute(NULL),BT_SUCCESS);
	BOOST_CHECK_EQUAL(node->execute(NULL),BT_FAILURE);

	for (int i =0 ; i<2 ; i++)
	{
		node->init(NULL);
		BOOST_CHECK_EQUAL(node->execute(NULL),BT_FAILURE);
		BOOST_CHECK_EQUAL(node->execute(NULL),BT_FAILURE);
	}
	delete node;
}



