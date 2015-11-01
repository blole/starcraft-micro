#pragma once
#include "common/common.hpp"
#include "common/squads/squad.hpp"
#include "common/units/unit.hpp"
#include "common/gamestate.hpp"
#include "common/effects/effect.hpp"
#include "common/players/player.hpp"


namespace Bot { namespace Squads
{
	template <class PlayerType>
	struct Playing : Squad
	{
		struct FrameActions
		{
			const unsigned int executionFrame;
			const vector<shared_ptr<Effect>> actions;
			FrameActions(unsigned int executionFrame, vector<shared_ptr<Effect>> actions)
				: executionFrame(executionFrame)
				, actions(actions)
			{}
		};
		deque<FrameActions> previousActions;
		static const int radius = 400;
		PlayerType play;
		
		Playing(const PlayerType& player = PlayerType())
			: play(player)
		{}
		
		virtual void onFrame() override
		{
			Squad::onFrame();
			
			vector<const Unit*> playerUnits;
			vector<const Unit*> enemyUnits;
			
			for (Unit* unit : units())
			{
				playerUnits.push_back(unit);
				for (auto& enemyBwapi : unit->bwapiUnit->getUnitsInRadius(radius, BWAPI::Filter::IsEnemy))
				{
					const Unit* enemy = &Unit::get(enemyBwapi);
					if (std::find(enemyUnits.begin(), enemyUnits.end(), enemy) == enemyUnits.end())
						enemyUnits.push_back(enemy);
				}
			}
			
			GameState state(playerUnits, enemyUnits, Broodwar->getFrameCount());
			const unsigned int nextExecutionFrame = Broodwar->getFrameCount() + Broodwar->getRemainingLatencyFrames();

			//apply current actions
			for (auto& unit : units())
			{
				auto order = unit->getActualOrders();
				if (order->isValid(state))
					order->applyTo(state);
			}

			//forget all non future actions
			while (!previousActions.empty() && previousActions.front().executionFrame <= Broodwar->getFrameCount())
				previousActions.pop_front();

			//forget too far future actions TODO: don't schedule them to begin with
			//this is relevant only on connections where Broodwar->getRemainingLatencyFrames() varies
			while (!previousActions.empty() && previousActions.back().executionFrame >= nextExecutionFrame)
				previousActions.pop_back();

			//apply future actions
			for (FrameActions& frameActions : previousActions)
			{
				while (frameActions.executionFrame > state.frame())
					state.advanceFrame();

				for (auto& action : frameActions.actions)
				{
					if (action->isValid(state))
						action->applyTo(state);
				}
			}

			while (state.frame() < nextExecutionFrame)
				state.advanceFrame();


			for (auto& unit : state.playerUnits)
			{
				Broodwar->drawCircleMap(unit->pos, 20, BWAPI::Colors::Blue);
				Broodwar->drawCircleMap(unit->bwapiUnit->getPosition(), 20, BWAPI::Colors::Green);
				Broodwar->drawTextMap(unit->pos - BWAPI::Position(0,  0), "   starting: %d", unit->bwapiUnit->isStartingAttack());
				Broodwar->drawTextMap(unit->pos - BWAPI::Position(0, 15), "attackFrame: %d", unit->bwapiUnit->isAttackFrame());
				Broodwar->drawTextMap(unit->pos - BWAPI::Position(0, 30), "  attacking: %d", unit->bwapiUnit->isAttacking());
				Broodwar->drawTextMap(unit->pos - BWAPI::Position(0, 45), "     target: %d", unit->bwapiUnit->getOrderTarget());
				Broodwar->drawTextMap(unit->pos - BWAPI::Position(0, 60), "   accelera: %d", unit->bwapiUnit->isAccelerating());
				Broodwar->drawTextMap(unit->pos - BWAPI::Position(0, 75), "   isMoving: %d", unit->bwapiUnit->isMoving());
				Broodwar->drawTextMap(unit->pos - BWAPI::Position(0, 90), "   cooldown: %d", unit->bwapiUnit->getGroundWeaponCooldown());
			}


			try
			{
				auto actions = play(state);
				for (shared_ptr<Effect>& action : actions)
				{
					if (action->isPlayerEffect(state))
						action->executeOrder(state);
				}
				previousActions.emplace_back(nextExecutionFrame, actions);
			}
			catch (const std::runtime_error&)	{ throw; }
			catch (const std::exception&)		{ throw; }
			catch (...)							{ throw; }
		}
	};
}}
