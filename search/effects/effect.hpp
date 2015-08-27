#pragma once
#include "search/gamestate.hpp"
#include "search/units/unit.hpp"
#include "common/common.hpp"

namespace Bot { namespace Search
{
	class Effect
	{
	public:
		virtual ~Effect() {}
		virtual void applyTo(GameState& state) const = 0;
		virtual void executeOrder(GameState& state) const {}
		virtual bool isPlayerEffect() const { return false; }
	};
	
	class NoEffect final : public Effect
	{
	public:
		virtual void applyTo(GameState& state) const final override
		{}
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

	template <int offset = 0, class NextEffect = void>
	class EffectChain
	{
	public:
		template <class Data>
		void queueNext(GameState& state, const Data& data) const
		{
			state.queueEffect(offset, std::make_shared<NextEffect>(data));
		}
	};
	template <>
	class EffectChain<0, void>
	{
	public:
		template <class Data>
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

		virtual bool isPlayerEffect() const override
		{
			return GameState::isPlayerUnit(unitID());
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
