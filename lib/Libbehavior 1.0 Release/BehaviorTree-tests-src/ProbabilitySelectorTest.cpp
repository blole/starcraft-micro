#include "BehaviorTree.h"
#include <boost/test/unit_test.hpp>
#include <algorithm>


using namespace BehaviorTree;
#define max(a,b) (((a) > (b)) ? (a) : (b))

BOOST_AUTO_TEST_CASE(trivial)
{
	ProbabilityNode* node = new ProbabilityNode();
	node->addChild(new SuccessAfter(0));
	int dummy_agent = 0;
	node->init(&dummy_agent);
	
	BOOST_CHECK_EQUAL(node->execute(&dummy_agent),BT_SUCCESS);

	delete node;
}

BOOST_AUTO_TEST_CASE(FiftyFifty)
{
	ProbabilityNode* node = new ProbabilityNode();
	node->addChild(new SuccessAfter(0));
	node->addChild(new FailureAfter(0));
	int dummy_agent = 0;
	node->init(&dummy_agent);

	int successes = 0,failures = 0;
	for (int i = 0 ; i<10000 ; i++)
	{
		switch (node->execute(&dummy_agent))
		{
		case BT_FAILURE: failures++;  break;
		case BT_SUCCESS: successes++; break;
		}
	}

	if (max(failures,successes) > 5100)
		BOOST_FAIL("questionable statistical distribution");

	delete node;
}

BOOST_AUTO_TEST_CASE(WithWeights1)
{
	ProbabilityNode* node = new ProbabilityNode();
	node->addChild(new SuccessAfter(0),9);
	node->addChild(new FailureAfter(0),1);
	int dummy_agent = 0;
	node->init(&dummy_agent);

	int successes = 0,failures = 0;
	for (int i = 0 ; i<10000 ; i++)
	{
		switch (node->execute(&dummy_agent))
		{
		case BT_FAILURE: failures++;  break;
		case BT_SUCCESS: successes++; break;
		}
	}

	if (failures < 950 || failures > 1050 || successes < 8950|| successes > 9050)
		BOOST_FAIL("questionable statistical distribution");

	delete node;
}