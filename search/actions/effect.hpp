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
}}
