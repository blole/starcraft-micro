#include "BehaviorTree.h"
#include <boost/test/unit_test.hpp>


using namespace BehaviorTree;


BOOST_AUTO_TEST_CASE( repeat_simple1 )
{
	RepeatNode* node = new RepeatNode(5);
	node->addChild(new SuccessAfter(0));
	int dummy_agent = 0;
	node->init(&dummy_agent);
	
	for (int i = 0 ; i<2 ; i++)
	{
		BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_RUNNING);
		BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_RUNNING);
		BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_RUNNING);
		BOOST_CHECK_EQUAL(node->execute(&dummy_agent), BT_RUNNING);
		BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_SUCCESS);
	}
	delete node;
}

BOOST_AUTO_TEST_CASE( repeat_simple2 )
{
	RepeatNode* node = new RepeatNode(5);
	node->addChild(new FailureAfter(0));
	int dummy_agent = 0;
	node->init(&dummy_agent);

	for (int i = 0 ; i<2; i++)
		BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_FAILURE);
	delete node;
}

