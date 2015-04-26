#pragma once
#include "search/gamestate.hpp"
#include "search/units/unit.hpp"
#include <BWAPI.h>
#include <cassert>
#include <boost/mpl/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/concept/requires.hpp>

namespace Bot { namespace Search
{
	template<typename Derived, typename Base>
	using EnableIfDerived = typename std::enable_if<std::is_base_of<Base, Derived>::value>::type;

	class B { };
	class D : B { };

	class ASD {};

	template <int offset, class NextEffect, class Enable = void>
	class S : ASD
	{};

	template <int offset, template<int, class> class NextEffect, int A, class B>
	class S<offset, NextEffect<A, B>> : ASD
	{
		typedef typename std::enable_if<std::is_base_of<S<A, B>, NextEffect<A, B>>::value>::type check;
	};

	template <int offset, template<int, class> class NextEffect, int A, class B>
	class S<offset, NextEffect<A, B>, typename std::enable_if<std::is_base_of<S<0, B>, NextEffect<A, B>>::value>::type> : ASD
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

	class Effect
	{
	public:
		virtual ~Effect() {}
		virtual void applyTo(GameState* state) const = 0;
		virtual void executeOrder(GameState* state) const {}
		virtual bool isPlayerAction(const GameState* state) const { return true; }
	};

	class NoEffect final : public Effect
	{
	public:
		virtual void applyTo(GameState* state) const override
		{}
	};





	template <int offset = 0, class NextEffect = void, class Enable = void>
	class EffectQueue
		: public Effect
	{
	protected:
		virtual NextEffect* newNext() const;
		virtual void queueNext(GameState* state) const
		{
			state->queueEffect(offset, newNext());
		}
	};
	
	template <int offset, class NextEffect>
	class EffectQueue<offset, NextEffect,
		typename std::enable_if<std::is_default_constructible<NextEffect>::value>::type>
		: public Effect
	{
	protected:
		virtual NextEffect* newNext() const
		{
			return new NextEffect();
		}
		virtual void queueNext(GameState* state) const
		{
			state->queueEffect(offset, newNext());
		}
	};
	
	// last Effect in queue
	template <>
	class EffectQueue<0, void>
		: public Effect
	{
	protected:
		virtual void queueNext(GameState* state) const {}
	};





	template <int offset = 0, class NextEffect = void>
	class SingleUnitEffect
		: public EffectQueue<offset, NextEffect>
	{
	protected:
		const id_t unitID;
	public:
		SingleUnitEffect(const id_t unitID)
			: unitID(unitID)
		{}
	};

	template <int offset, template<int,class> class NextEffect, int A, class B>
	class SingleUnitEffect<offset, NextEffect<A, B>>
		: public EffectQueue<offset, NextEffect<A, B>>
	{
	protected:
		virtual NextEffect<A, B>* //typename std::enable_if<std::is_base_of<SingleUnitEffect<A, B>, NextEffect<A, B>>::value, NextEffect<A, B>*>::type
		newNext() const override
		{
			return new NextEffect<A, B>(unitID);
		}
	protected:
		const id_t unitID;
	public:
		SingleUnitEffect(const id_t unitID)
			: unitID(unitID)
		{}
	};



	template <int offset = 0, class NextEffect = void>
	class TwoUnitEffect
		: public SingleUnitEffect<offset, NextEffect>
	{
	protected:
		const id_t targetID;
	public:
		TwoUnitEffect(const id_t unitID, const id_t targetID)
			: SingleUnitEffect(unitID)
			, targetID(targetID)
		{}
	};

	template <int offset, template<int, class> class NextEffect, int A, class B>
	class TwoUnitEffect<offset, NextEffect<A,B>>
		: public SingleUnitEffect<offset, NextEffect<A,B>>
	{
		typedef typename std::enable_if<std::is_base_of<TwoUnitEffect<A, B>, NextEffect<A, B>>::value>::type check;
	protected:
		virtual NextEffect<A,B>* newNext() const override
		{
			return new NextEffect<A,B>(unitID, targetID);
		}
	protected:
		const id_t targetID;
	public:
		TwoUnitEffect(const id_t unitID, const id_t targetID)
			: SingleUnitEffect(unitID)
			, targetID(targetID)
		{}
	};

	/*template <int offset, template<int, class, class=void> class NextEffect, int A, class B>
	class TwoUnitEffect<offset, NextEffect<A,B>,
		typename std::enable_if<std::is_base_of<SingleUnitEffect<A,B>, NextEffect<A,B>>::value>::type>
		: public SingleUnitEffect<offset, NextEffect<A,B>>
	{
	protected:
		const id_t targetID;
	public:
		TwoUnitEffect(const id_t unitID, const id_t targetID)
			: SingleUnitEffect(unitID)
			, targetID(targetID)
		{}

	protected:
		virtual NextEffect<A,B>* newNext() const override
		{
			return new NextEffect<A,B>(unitID);
		}
	};*/
}}
