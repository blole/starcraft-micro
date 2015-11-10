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
		deque<vector<shared_ptr<Effect>>> pendingEffects;
		static const int radius = 400;
		PlayerType play;
		
		Playing(const PlayerType& player = PlayerType())
			: play(player)
		{}
		
		virtual void onFrame() override
		{
			Squad::onFrame();
			
			//TODO: return if the next execution frame is further away than necessary

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
			const unsigned int nextExecutionFrameOffset = Broodwar->getRemainingLatencyFrames()+1;
			const unsigned int nextExecutionFrame = Broodwar->getFrameCount() + nextExecutionFrameOffset;

			//remove all effects that have become invalid
			for (auto& frameEffects : pendingEffects)
			{
				frameEffects.erase(std::remove_if(frameEffects.begin(), frameEffects.end(),
					[&state](shared_ptr<Effect>& e) {return !e->isValid(state);}), frameEffects.end());
			}

			//apply predicted effects via applyPredicted()
			if (!pendingEffects.empty())
			{
				for (auto& effect : pendingEffects.front())
					effect->applyPredicted(state);
				pendingEffects.pop_front();
			}
			state.pendingEffects = pendingEffects;

			//and apply actual effects via applyObserved()
			for (auto& unit : units())
			{
				for (auto effect : unit->getNewEffects())
				{
					if (effect->isValid(state))
						effect->applyObserved(state);
				}
			}

			pendingEffects = state.pendingEffects;

			GameState projectedState(state);

			for (unsigned int i = 0; i < nextExecutionFrameOffset; i++)
				projectedState.advanceFrame();

			if (pendingEffects.size() < nextExecutionFrameOffset)
				pendingEffects.resize(nextExecutionFrameOffset);

			try
			{
				auto actions = play(projectedState);

				for (shared_ptr<Effect>& action : actions)
				{
					if (action->isPlayerEffect(projectedState))
						action->executeOrder(projectedState);
					pendingEffects[nextExecutionFrameOffset-1].push_back(action);
				}
			}
			catch (const std::runtime_error&)	{ throw; }
			catch (const std::exception&)		{ throw; }
			catch (...)							{ throw; }



			for (auto& unit : state.units)
			{
				Broodwar->drawCircleMap(unit->pos, 20, BWAPI::Colors::Blue);
				Broodwar->drawCircleMap(unit->bwapiUnit->getPosition(), 20, BWAPI::Colors::Green);
				Broodwar->drawTextMap(unit->pos - BWAPI::Position(0,  0), "   starting: %d", unit->bwapiUnit->isStartingAttack());
				Broodwar->drawTextMap(unit->pos - BWAPI::Position(0, 15), "attackFrame: %d", unit->bwapiUnit->isAttackFrame());
				Broodwar->drawTextMap(unit->pos - BWAPI::Position(0, 30), "  attacking: %d", unit->bwapiUnit->isAttacking());
				Broodwar->drawTextMap(unit->pos - BWAPI::Position(0, 45), "     target: %d", unit->bwapiUnit->getOrderTarget());
				Broodwar->drawTextMap(unit->pos - BWAPI::Position(0, 60), "   accelera: %d", unit->bwapiUnit->isAccelerating());
				Broodwar->drawTextMap(unit->pos - BWAPI::Position(0, 75), "   isMoving: %d", unit->bwapiUnit->isMoving());
				Broodwar->drawTextMap(unit->pos - BWAPI::Position(0, 90), " w cooldown: %d", unit->bwapiUnit->getGroundWeaponCooldown());
				Broodwar->drawTextMap(unit->pos - BWAPI::Position(0,105), " m cooldown: %d", unit->moveCooldown);
				Broodwar->drawTextMap(unit->pos - BWAPI::Position(0,120), "         hp: %d", unit->bwapiUnit->getHitPoints());
				if (unit->bwapiUnit->getGroundWeaponCooldown() != unit->groundWeaponCooldown)
					Broodwar << Broodwar->getFrameCount() << " error: " << unit->bwapiUnit->getGroundWeaponCooldown() << " != " << unit->groundWeaponCooldown << " ";
			}

			for (auto& unit : state.playerUnits)
			{
				if (unit->bwapiUnit->isStartingAttack())
					Broodwar << Broodwar->getFrameCount() << ": started attacking" << std::endl;
			}
		}
	};
}}
