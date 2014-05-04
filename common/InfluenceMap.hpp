#pragma once
#include <BWAPI.h>
#include "PUnit.hpp"
#include "OUnit.hpp"

class InfluenceMap
{
public:
	// Attributes
	std::map<BWAPI::UnitType,double**> matrixInfluence;
	int mapWidth;
	int mapHeight;

	// Informations map
	std::map<BWAPI::UnitType,double> mapSpeed;
	std::map<BWAPI::UnitType,double> mapAcceleration;
	std::map<BWAPI::UnitType,double> mapTurn;
	std::map<BWAPI::UnitType,double> mapAttackTime;
	std::map<BWAPI::UnitType,double> mapAttackRange;


	// Constructor
	// CAN BE INSTANTIATE ONCE EVERYUNIT EXISTS
	InfluenceMap();
	// Methods
	void init();
	void update();
	void debugDisplayInfluence();
	
};
