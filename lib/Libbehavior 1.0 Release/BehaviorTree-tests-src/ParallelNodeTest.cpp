#include "BehaviorTree.h"
#include <boost/test/unit_test.hpp>


using namespace BehaviorTree;

BOOST_AUTO_TEST_CASE( simple1 )
{
	ParallelNode* node = new ParallelNode();
	node->addChild(new FailureAfter(1));
	node->addChild(new FailureAfter(3));
	int dummy_agent = 0;
	for (int i = 0 ; i <2 ; i++)
	{
	node->init(&dummy_agent);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_RUNNING);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_RUNNING);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_RUNNING);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_FAILURE);
	}
	delete node;
}

BOOST_AUTO_TEST_CASE( simple2 )
{
	ParallelNode* node = new ParallelNode();
	node->addChild(new FailureAfter(0));
	int dummy_agent = 0;
	node->init(&dummy_agent);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_FAILURE);

	delete node;
}

// as currently specified, the parallelnode will neither succeed nor fail in this situation
// is this the best behavior?
// seems like it should fail instead of being in limbo
BOOST_AUTO_TEST_CASE( simple3 )
{
	ParallelNode* node = new ParallelNode();
	node->addChild(new SuccessAfter(1));
	node->addChild(new FailureAfter(1));
	node->addChild(new FailureAfter(1));
	int dummy_agent = 0;
	node->init(&dummy_agent);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_RUNNING);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_RUNNING);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_RUNNING);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_RUNNING);

	delete node;
}

BOOST_AUTO_TEST_CASE( simple4 )
{
	ParallelNode* node = new ParallelNode();
	node->addChild(new SuccessAfter(1));
	node->addChild(new SuccessAfter(2));
	int dummy_agent = 0;
	for (int i = 0 ; i <2 ; i++)
	{
	node->init(&dummy_agent);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_RUNNING);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_RUNNING);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_SUCCESS);
	}

	delete node;
}

BOOST_AUTO_TEST_CASE( simple5 )
{
	ParallelNode* node = new ParallelNode(FAIL_ON_ONE);
	node->addChild(new SuccessAfter(1));
	node->addChild(new SuccessAfter(2));
	node->addChild(new FailureAfter(3));
	node->addChild(new SuccessAfter(4));
	int dummy_agent = 0;
	for (int i = 0 ; i <2 ; i++)
	{
	node->init(&dummy_agent);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_RUNNING);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_RUNNING);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_RUNNING);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_FAILURE);
	}

	delete node;
}

BOOST_AUTO_TEST_CASE( simple6 )
{
	ParallelNode* node = new ParallelNode(FAIL_ON_ALL,SUCCEED_ON_ONE);
	node->addChild(new SuccessAfter(2));
	node->addChild(new FailureAfter(0));
	node->addChild(new FailureAfter(1));
	node->addChild(new FailureAfter(4));
	int dummy_agent = 0;
	for (int i = 0 ; i <2 ; i++)
	{
	node->init(&dummy_agent);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_RUNNING);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_RUNNING);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_SUCCESS);
	}

	delete node;
}

BOOST_AUTO_TEST_CASE( simple7 )
{
	ParallelNode* node = new ParallelNode();
	node->addChild(new SuccessAfter(2));
	node->addChild(new SuccessAfter(0));
	node->addChild(new SuccessAfter(4));
	node->addChild(new SuccessAfter(7));
	node->addChild(new SuccessAfter(6));
	node->addChild(new SuccessAfter(2));
	int dummy_agent = 0;
	for (int i = 0 ; i <2 ; i++)
	{
	node->init(&dummy_agent);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_RUNNING);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_RUNNING);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_RUNNING);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_RUNNING);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_RUNNING);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_RUNNING);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_RUNNING);
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_SUCCESS);
	}

	delete node;
}