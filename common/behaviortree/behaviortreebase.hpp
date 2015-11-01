#pragma once
#include "common/common.hpp"
#include "common/units/unit.hpp"
#include "common/gamestate.hpp"
#include "common/effects/effect.hpp"

namespace Bot { namespace Behaviors
{
	const shared_ptr<Effect> success = make_shared<Effects::NoEffect>();
	const shared_ptr<Effect> running = make_shared<Effects::NoEffect>();
	const shared_ptr<Effect> failure = make_shared<Effects::NoEffect>();

	/// Enumerates the options for when a parallel node is considered to have failed.
	/**
	- FAIL_ON_ONE indicates that the node will return failure as soon as one of its children fails.
	- FAIL_ON_ALL indicates that all of the node's children must fail before it returns failure.

	If FAIL_ON_ONE and SUCEED_ON_ONE are both active and are both trigerred in the same time step, failure will take precedence.
	*/
	enum FAILURE_POLICY {FAIL_ON_ONE,FAIL_ON_ALL};
	/// Enumerates the options for when a parallel node is considered to have succeeded.
	/**
	- SUCCEED_ON_ONE indicates that the node will return success as soon as one of its children succeeds.
	- SUCCEED_ON_ALL indicates that all of the node's children must succeed before it returns success.
	*/
	enum SUCCESS_POLICY {SUCCEED_ON_ONE,SUCCEED_ON_ALL};


	// Abstract base clase for Behavior Tree Nodes
	struct BehaviorTreeNode
	{
		virtual ~BehaviorTreeNode() {}

		// This method will be invoked before the node is executed for the first time.
		virtual void init(GameState& state, Unit& unit) {}

		// This method is invoked by the node's parent when the node should be run.
		virtual shared_ptr<Effect> execute(GameState& state, Unit& unit) = 0;

		virtual BehaviorTreeNode* clone() const = 0;
	};

	template <typename Derived>
	class BehaviorTreeNode_CRTP : public BehaviorTreeNode
	{
	public:
		virtual BehaviorTreeNode* clone() const final override
		{
			return new Derived(static_cast<const Derived&>(*this));
		}
	};



	// Abstract base class for Behavior Tree nodes with children
	template <typename Derived>
	struct BehaviorTreeInternalNode : BehaviorTreeNode_CRTP<Derived>
	{
	protected:
		vector<unique_ptr<BehaviorTreeNode>> children;
	public:
		BehaviorTreeInternalNode()
		{}
		BehaviorTreeInternalNode(const BehaviorTreeInternalNode& other)
			: children(::clone(other.children))
		{}

		virtual Derived& BehaviorTreeInternalNode::addChild(unique_ptr<BehaviorTreeNode> child)
		{
			children.push_back(std::move(child));
			return static_cast<Derived&>(*this);
		};
	};

	// Always returns the given status
	struct Always : BehaviorTreeNode_CRTP<Always>
	{
	protected:
		const shared_ptr<Effect> status;
	public:
		Always(const shared_ptr<Effect>& status)
			: status(status)
		{}
		virtual shared_ptr<Effect> execute(GameState& state, Unit& unit) override
		{
			return status;
		}
	};



	// Returns running for the specified number of iterations, then returns success after that
	template <int return_after, class StatusType>
	struct ReturnAfter: BehaviorTreeNode_CRTP<ReturnAfter<return_after, StatusType>>
	{
	protected:
		StatusType status;
		int n;
	public:
		ReturnAfter(StatusType status = StatusType())
			: status(status)
			, n(0)
		{}
	public:
		virtual void init(GameState& state, Unit& unit) override
		{
			n = 0;
		}
		virtual shared_ptr<Effect> execute(GameState& state, Unit& unit) override
		{
			if (n == return_after)
				return status;
			else
			{
				n++;
				return running;
			}
		}
	};
}}
