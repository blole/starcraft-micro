#pragma once
#include <BWAPI.h>
#include "common/PUnit.hpp"
#include "common/OUnit.hpp"

class InfluenceMap
{
public:
	// Attributes
	static std::map<BWAPI::UnitType,double**> matrixInfluence;
	static int mapWidth;
	static int mapHeight;

	// Informations map
	static std::map<BWAPI::UnitType,double> mapSpeed;
	static std::map<BWAPI::UnitType,double> mapAcceleration;
	static std::map<BWAPI::UnitType,double> mapTurn;
	static std::map<BWAPI::UnitType,double> mapAttackTime;
	static std::map<BWAPI::UnitType,double> mapAttackRange;

	// Methods
	static void init();
	static void update();
	static void debugDisplayInfluence();
	
};
