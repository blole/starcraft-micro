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
			
			GameState state(playerUnits, enemyUnits);

			for (auto& unit : state.units)
				unit->firstFrameInitToAddAlreadyActiveEffects(state);


			for (auto& unit : state.playerUnits)
			{
				//Broodwar->drawTextMap(bwapiUnit->pos - BWAPI::Position(0,15), "isAttackFrame: %d", bwapiUnit->isAttackFrame);
				//Broodwar->drawTextMap(bwapiUnit->pos - BWAPI::Position(0, 0), "isAttackFrame: %d", bwapiUnit->bwapiUnit->isAttackFrame());

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
				for (shared_ptr<Effect>& action : play(state))
				{
					if (action->isPlayerEffect(state))
						action->executeOrder(state);
				}
			}
			catch (const std::runtime_error&)	{ throw; }
			catch (const std::exception&)		{ throw; }
			catch (...)							{ throw; }
		}
	};
}}
