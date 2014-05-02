#include "common/PUnit.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

std::map<int, PUnit*> PUnit::units;

PUnit::PUnit(Unit unit)
	: GameUnit(unit)
	, target(nullptr)
	, brain(nullptr)
	, squad(nullptr)
	, hasTarget(false)
{
}

	// Methods
bool PUnit::isAttacking()
{
	return (this->unit->isAttackFrame() || (this->unit->getGroundWeaponCooldown() != 0) || this->unit->isAttacking());
}

void PUnit::setTarget(PositionOrUnit newTarget)
{
	target = newTarget;
	hasTarget = true;
}

void PUnit::attackTarget(PositionOrUnit newTarget, bool addToQueue = false)
{
	this->setTarget(newTarget);
	this->unit->attack(target,addToQueue);
}

Unit PUnit::getClosestEnemy()
{
	return this->unit->getClosestUnit(Filter::IsEnemy);
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
			double distI = potentialEnemy->unit->getPosition().getApproxDistance(this->getPosition());
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
		this->target = bestTarget->unit;
		bestTarget->dammageAttributed += Broodwar->getDamageFrom(
						this->unit->getType(),this->target.getUnit()->getType());
		this->unit->attack(this->target);
		hasTarget = true;
	}
	else if(this->target.getUnit() != bestTarget->unit)
	{
		// Update previous target
		OUnit* previousTarget = OUnit::get(this->target.getUnit());
		previousTarget->dammageAttributed -= Broodwar->getDamageFrom(
						this->unit->getType(),this->target.getUnit()->getType());
	
		// Update new target
		this->target = bestTarget->unit;
		bestTarget->dammageAttributed += Broodwar->getDamageFrom(
						this->unit->getType(),this->target.getUnit()->getType());
		this->unit->attack(this->target);
		hasTarget = true;
	}
}



PUnit* PUnit::get(BWAPI::Unit unit)
{
	int id = unit->getID();

	auto iter = units.find(id);
	if (iter != units.end())
		return iter->second;
	else
	{
		PUnit* u = new PUnit(unit);
		PUnit::units[id] = u;
		return u;
	}
}
