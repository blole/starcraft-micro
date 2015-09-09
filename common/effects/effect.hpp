#pragma once
#include "common/gamestate.hpp"
#include "common/units/unit.hpp"
#include "common/common.hpp"

namespace Bot
{
	class Effect
	{
	public:
		virtual ~Effect() {}
		virtual void applyTo(GameState& state) const = 0;
		virtual void executeOrder(GameState& state) const {}
		virtual bool isPlayerEffect(const GameState& state) const { return false; }
		virtual bool isValid(const GameState& state) const { return true; }
	};

	class NoEffect : public Effect
	{
	public:
		virtual void applyTo(GameState& state) const final override
		{}
	};

	template <bool isPlayer>
	class NoEffectPlayer final : public NoEffect
	{
	public:
		virtual bool isPlayerEffect(const GameState& state) const final override
		{
			return isPlayer;
		}
	};

	class AdvanceFrameEffect : public Effect
	{
		const int frames;
	public:
		AdvanceFrameEffect(int frames)
			: frames(frames)
		{}
		virtual void applyTo(GameState& state) const override
		{
			state.advanceFrames(frames);
		}
	};

	template <class Data>
	class EffectWithData : public Effect
	{
	protected:
		const Data data;
	public:
		EffectWithData(const Data& data)
			: data(data)
		{}
	};



	template <class Data, int offset = 0, class NextEffect = void>
	class EffectChain
	{
	public:
		void queueNext(GameState& state, const Data& data) const
		{
			state.queueEffect(offset, std::make_shared<NextEffect>(data));
		}
	};
	template <class Data>
	class EffectChain<Data, 0, void>
	{
	public:
		void queueNext(GameState& state, const Data& data) const
		{}
	};
	


	template <class Data>
	class OneUnitEffect : public EffectWithData<Data>
	{
	protected:
		const BWAPI::Unit& bwapiUnit() const { return data.bwapiUnit; }
	public:
		OneUnitEffect(const Data& data)
			: EffectWithData(data)
		{}
		virtual bool isPlayerEffect(const GameState& state) const override
		{
			return bwapiUnit()->getPlayer() == Broodwar->self();
		}
		virtual bool isValid(const GameState& state) const override
		{
			return state.has(bwapiUnit());
		}
	};
	template <class Data>
	class TwoUnitEffect : public OneUnitEffect<Data>
	{
	protected:
		const BWAPI::Unit& bwapiTarget() const { return data.bwapiTarget; }
	public:
		TwoUnitEffect(Data data)
			: OneUnitEffect<Data>(data)
		{}
		virtual bool isValid(const GameState& state) const override
		{
			return OneUnitEffect<Data>::isValid(state) && state.has(bwapiTarget());
		}
	};



	struct OneUnitEffectData
	{
		BWAPI::Unit const bwapiUnit;
		OneUnitEffectData(const Unit& unit)
			: bwapiUnit(unit.bwapiUnit)
		{}
	};

	struct TwoUnitEffectData : OneUnitEffectData
	{
		BWAPI::Unit const bwapiTarget;
		TwoUnitEffectData(const Unit& unit, const Unit& target)
			: OneUnitEffectData(unit)
			, bwapiTarget(target.bwapiUnit)
		{}
	};
}
