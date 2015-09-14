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

			while (!previousActions.empty() && previousActions.front().executionFrame < state.frame())
				previousActions.pop_front();

			//apply previous actions
			auto actionIter = previousActions.begin();
			while (actionIter != previousActions.end() && state.frame() < nextExecutionFrame)
			{
				while (actionIter->executionFrame > state.frame())
					state.advanceFrames(1);

				do
				{
					for (const shared_ptr<Effect>& action : actionIter->actions)
					{
						if (action->isValid(state))
							action->applyTo(state);
					}
					++actionIter;
				} while (actionIter != previousActions.end() && actionIter->executionFrame == state.frame());
			}


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
