#include "common/punit.hpp"
#include <iostream>
#include "common/squad.hpp"

using namespace BWAPI;
using namespace Filter;
using namespace Bot;

std::unordered_map<int, PUnit*> PUnit::units;

PUnit::PUnit(Unit unit)
	: GameUnit(unit)
	, target(nullptr)
	, brain(nullptr)
	, squad(nullptr)
	, hasTarget(false)
{
	PUnit::units[unit->getID()] = this;
}

bool PUnit::isAttacking()
{
	//	return (unit->isAttacking());
	return (unit->isAttackFrame() || (unit->getGroundWeaponCooldown() != 0) || unit->isAttacking());
}

void PUnit::setTarget(PositionOrUnit newTarget)
{
	target = newTarget;
	hasTarget = true;
}

void PUnit::attackTarget(PositionOrUnit newTarget, bool addToQueue = false)
{
	setTarget(newTarget);
	unit->attack(target,addToQueue);
}

Unit PUnit::getClosestEnemy()
{
	return unit->getClosestUnit(Filter::IsEnemy);
}


void PUnit::attackClosestEnemyNonWorried()
{
	// Find the closest target that will not die
	auto oUnits = OUnit::units;	
	
	double bestDist = 10000000;
	OUnit* bestTarget = nullptr; 
	for(auto i = oUnits.begin(); i!=oUnits.end(); i++)
	{
		OUnit* potentialEnemy = i->second;
		if(!(potentialEnemy->willDie()))
		{
			double distI = potentialEnemy->unit->getPosition().getApproxDistance(getPosition());
			if(distI < bestDist)
			{
				bestDist = distI;
				bestTarget = potentialEnemy;
			}
		}
	}

	if(bestTarget == nullptr)
	{
		hasTarget = false;
		return;
	}

	// Attack the new target
	if(!hasTarget)
	{
		target = bestTarget->unit;
		bestTarget->dammageAttributed += Broodwar->getDamageFrom(
						unit->getType(),target.getUnit()->getType());
		unit->attack(target);
		hasTarget = true;
	}
	else if(target.getUnit() != bestTarget->unit)
	{
		// Update previous target
		OUnit* previousTarget = OUnit::get(target.getUnit());
		previousTarget->dammageAttributed -= Broodwar->getDamageFrom(
						unit->getType(),target.getUnit()->getType());
	
		// Update new target
		target = bestTarget->unit;
		bestTarget->dammageAttributed += Broodwar->getDamageFrom(
						unit->getType(),target.getUnit()->getType());
		unit->attack(target);
		hasTarget = true;
	}
}

bool PUnit::isUnderAttack()
{
	return unit->isUnderAttack();
}


PUnit* PUnit::get(BWAPI::Unit unit)
{
	int id = unit->getID();

	auto iter = units.find(id);
	if (iter != units.end())
		return iter->second;
	else
		return new PUnit(unit);
}
