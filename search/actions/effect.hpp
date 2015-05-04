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

	template <class Data>
	class EffectWithData : public Effect
	{
	protected:
		Data data;
	public:
		EffectWithData(const Data& data)
			: data(data)
		{}
	};

	template <int offset = 0, class NextEffect = void>
	class EffectChain
	{
	public:
		template <class Data>
		void queueNext(GameState* state, const Data& data) const
		{
			state->queueEffect(offset, new NextEffect(data));
		}
	};
	template <>
	class EffectChain<0, void>
	{
	public:
		template <class Data>
		void queueNext(GameState* state, const Data& data) const
		{}
	};
	
	template <class Data = nullptr_t>
	class OneUnitEffect : public EffectWithData<Data>
	{
	protected:
		const id_t& unitID;
	public:
		OneUnitEffect(const Data& data)
			: EffectWithData(data)
			, unitID(data.unitID)
		{}
	};

	template <class Data = nullptr_t>
	class TwoUnitEffect : public EffectWithData<Data>
	{
	protected:
		const id_t& unitID;
		const id_t& targetID;
	public:
		TwoUnitEffect(const Data& data)
			: EffectWithData(data)
			, unitID(data.unitID)
			, targetID(data.targetID)
		{}
	};

	struct OneUnitEffectData
	{
		id_t unitID;
		OneUnitEffectData(const id_t& unitID)
			: unitID(unitID)
		{}
	};

	struct TwoUnitEffectData : OneUnitEffectData
	{
		id_t targetID;
		TwoUnitEffectData(const id_t& unitID, const id_t& targetID)
			: OneUnitEffectData(unitID)
			, targetID(targetID)
		{}
	};

	/*template <class Derived, class Base,
		typename std::enable_if<std::is_default_constructible<Derived>::value>::type>
	Derived* getNew(const Base* base)
	{
		return new Derived();
	}

	template <class Derived, int A, class B>
	typename std::enable_if<std::is_base_of<SingleUnitEffect<A, B>, Derived>::value, Derived*>::type
		getNew(const SingleUnitEffect<A, B>* base)
	{
		return new Derived(base->unitID);
	}

	template <class Derived, int A, class B>
	typename std::enable_if<std::is_base_of<TwoUnitEffect<A, B>, Derived>::value, Derived*>::type
		getNew(const TwoUnitEffect<A, B>* base)
	{
		return new Derived(base->unitID);
	}*/



	/*template <class NextEffect>
	struct queue_next
	{
		template <class PrevEffect>
		static void lol(const PrevEffect* const prev, GameState* state, int offset)
		{
			state->queueEffect(offset, new NextEffect());
		}
		template<int A, class B>
		static void lol<SingleUnitEffect<A, B>>(const SingleUnitEffect<A, B>* const prev, GameState* state, int offset)
		{
			state->queueEffect(offset, new NextEffect(prev->unitID));
		}
	};

	template<int A, class B>
	struct queue_next<SingleUnitEffect<A,B>>
	{
		static void lol<SingleUnitEffect<A, B>>(const SingleUnitEffect<A, B>* const prev, GameState* state, int offset)
		{
			state->queueEffect(offset, new NextEffect(prev->unitID));
		}
	};
	
	template <>
	struct queue_next<void>
	{
		template <class PrevEffect>
		static void lol(const PrevEffect* const prev, GameState* state, int offset)
		{}
	};*/
}}
