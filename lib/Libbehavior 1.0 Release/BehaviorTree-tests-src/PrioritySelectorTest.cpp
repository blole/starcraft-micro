#include "BehaviorTree.h"
#include <boost/test/unit_test.hpp>


using namespace BehaviorTree;


BOOST_AUTO_TEST_CASE( currentNodeInitialize )
{
	PriorityNode* node = new PriorityNode();
	BOOST_CHECK(node->currentPosition == -1);
}

BOOST_AUTO_TEST_CASE( pst_simple1 )
{
	PriorityNode* node = new PriorityNode();
	AlwaysRunning* ar = new AlwaysRunning();
	node->addChild(ar);
	int dummy_agent = 0;
	node->init(&dummy_agent);
	BOOST_CHECK_EQUAL(node->currentPosition, -1);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_RUNNING);
	BOOST_CHECK_EQUAL(node->currentPosition, 0);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_RUNNING);
	BOOST_CHECK_EQUAL(node->currentPosition, 0);
	delete node;
}

BOOST_AUTO_TEST_CASE( pst_simple2 )
{
	PriorityNode* node = new PriorityNode();
	node->addChild(new FailureAfter(1));
	node->addChild(new FailureAfter(1));
	node->addChild(new SuccessAfter(1));
	int dummy_agent = 0;
	for (int i = 0 ; i <2 ; i++)
	{
	node->init(&dummy_agent);
	BOOST_CHECK_EQUAL(node->currentPosition, -1);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_RUNNING);
	BOOST_CHECK_EQUAL(node->currentPosition, 0);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_RUNNING);
	BOOST_CHECK_EQUAL(node->currentPosition, 1);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_RUNNING);
	BOOST_CHECK_EQUAL(node->currentPosition, 2);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_SUCCESS);
	BOOST_CHECK_EQUAL(node->currentPosition, -1);
	}

	delete node;
}

BOOST_AUTO_TEST_CASE( pst_simple3 )
{
	PriorityNode* node = new PriorityNode();
	node->addChild(new SuccessAfter(1));
	node->addChild(new FailureAfter(1));
	node->addChild(new FailureAfter(1));
	int dummy_agent = 0;
	for (int i = 0 ; i <2 ; i++)
	{
	node->init(&dummy_agent);
	BOOST_CHECK_EQUAL(node->currentPosition, -1);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_RUNNING);
	BOOST_CHECK_EQUAL(node->currentPosition, 0);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_SUCCESS);
	}

	delete node;
}

BOOST_AUTO_TEST_CASE( pst_simple4 )
{
	PriorityNode* node = new PriorityNode();
	node->addChild(new FailureAfter(1));
	node->addChild(new FailureAfter(1));
	int dummy_agent = 0;
	for (int i = 0 ; i <2 ; i++)
	{
	node->init(&dummy_agent);
	BOOST_CHECK_EQUAL(node->currentPosition, -1);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_RUNNING);
	BOOST_CHECK_EQUAL(node->currentPosition, 0);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_RUNNING);
	BOOST_CHECK_EQUAL(node->currentPosition, 1);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_FAILURE);
	BOOST_CHECK_EQUAL(node->currentPosition, -1);
	}

	delete node;
}