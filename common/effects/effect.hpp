#pragma once
#include "common/gamestate.hpp"
#include "common/units/unit.hpp"
#include "common/common.hpp"

namespace Bot { namespace Search
{
	class Effect
	{
	public:
		virtual ~Effect() {}
		virtual void applyTo(GameState& state) const = 0;
		virtual void executeOrder(GameState& state) const {}
		virtual bool isPlayerEffect(const GameState& state) const { return false; }
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
		virtual bool isPlayerEffect(const GameState& state) const final override { return isPlayer; }
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
		Data data;
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
		id_t unitID() const { return data.unitID; }
	public:
		OneUnitEffect(const Data& data)
			: EffectWithData(data)
		{}

		virtual bool isPlayerEffect(const GameState& state) const override
		{
			return state.units[unitID()]->isPlayer;
		}
	};
	template <class Data>
	class TwoUnitEffect : public OneUnitEffect<Data>
	{
	protected:
		id_t targetID() const { return data.targetID; }
	public:
		TwoUnitEffect(Data data)
			: OneUnitEffect<Data>(data)
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
