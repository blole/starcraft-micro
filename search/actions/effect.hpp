#pragma once
#include "search/gamestate.hpp"
#include "search/units/unit.hpp"
#include <BWAPI.h>
#include <cassert>
#include <boost/mpl/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>

namespace Bot { namespace Search
{
	class B { };
	class D : B { };

	template <int offset, class NextEffect, class Enable = void>
	class S
	{};

	template <int offset, template<int, class> class NextEffect, int A, class B>
	class S<offset, NextEffect<A, B>>
	{
		typedef typename std::enable_if<std::is_base_of<S<A, B>, NextEffect<A, B>>::value>::type check;
	};

	template <int offset, template<int, class> class NextEffect, int A, class B>
	class S<offset, NextEffect<A, B>, typename std::enable_if<std::is_base_of<S<0, B>, NextEffect<A, B>>::value>::type>
	{
	};

	/*template <int offset, template<int, class> class NextEffect, int A, class B>
	class S<offset, NextEffect<A, B>, typename std::enable_if<std::is_base_of<S<0, B>, NextEffect<A, B>>::value>::type>
	{
	};*/

	/*
	template <int offset, int nextOffset, class NextNextEffect>
	class S<offset, S<nextOffset, NextNextEffect>>
		: public Effect<offset, S<nextOffset, NextNextEffect>>
	*/

	template <int offset = 0, class NextEffect = void>
	class Effect;

	template <int offset, template<int, class> class NextEffect, int A, class B>
	class Effect<offset, NextEffect<A,B>>
	{
		typedef typename std::enable_if<std::is_base_of<Effect<A, B>, NextEffect<A, B>>::value>::type check;
	protected:
		virtual NextEffect<A, B>* newNext() const = 0;
	public:
		virtual void applyTo(GameState* state) = 0;
		virtual void applyToAndQueueNextEffect(GameState* state)
		{
			applyTo(state);
			state->queueEffect(offset, newNext());
		}
	};

	// last Effect in queue
	template <>
	class Effect<0, void>
	{
	public:
		virtual void applyTo(GameState* state) = 0;
		virtual void applyToAndQueueNextEffect(GameState* state)
		{
			applyTo(state);
		}
	};



	template <int offset = 0, class NextEffect = void>
	class SingleUnitEffect
		   : public Effect<offset, NextEffect>
	{
		typedef typename std::enable_if<std::is_same<void, NextEffect>::value>::type check;
	protected:
		const id_t unitID;
	public:
		SingleUnitEffect(const id_t unitID)
			: unitID(unitID)
		{}
	};

	template <int offset, template<int,class> class NextEffect, int A, class B>
	class SingleUnitEffect<offset, NextEffect<A, B>>
		: public Effect<offset, NextEffect<A, B>>
	{
		typedef typename std::enable_if<std::is_base_of<SingleUnitEffect<A,B>, NextEffect<A,B>>::value>::type check;
	protected:
		const id_t unitID;
	public:
		SingleUnitEffect(const id_t unitID)
			: unitID(unitID)
		{}

	protected:
		virtual NextEffect<A,B>* newNext() const override
		{
			return new NextEffect<A,B>(unitID);
		}
	};



	template <int offset = 0, class NextEffect = void>
	class ClearAttackFrame : public SingleUnitEffect<offset, NextEffect>
	{
	public:
		ClearAttackFrame(const id_t unitID)
			: SingleUnitEffect(unitID)
		{}

		void applyTo(GameState* state) override
		{
			//state->getUnitModifiable(unitID)->isAttackFrame = false;
		}
	};
}}
