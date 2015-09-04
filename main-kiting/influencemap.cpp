#include "main-kiting/influencemap.hpp"
#include "common/unit.hpp"

using namespace BWAPI;
using namespace Bot;

std::map<BWAPI::UnitType,double**> InfluenceMap::matrixInfluence;
std::map<BWAPI::UnitType,double> InfluenceMap::mapSpeed;
std::map<BWAPI::UnitType,double> InfluenceMap::mapAcceleration;
std::map<BWAPI::UnitType,double> InfluenceMap::mapTurn;
std::map<BWAPI::UnitType,double> InfluenceMap::mapAttackTime;
std::map<BWAPI::UnitType,double> InfluenceMap::mapAttackRange;
int InfluenceMap::mapWidth;
int InfluenceMap::mapHeight;

void InfluenceMap::init()
{
	mapWidth = Broodwar->mapWidth();
	mapHeight = Broodwar->mapHeight();

	// Create a influence map for each type of my units
	for (auto& unit : Broodwar->self()->getUnits())
	{
		auto type = unit->getType();
		auto iter = matrixInfluence.find(type);
		if(! (iter != matrixInfluence.end()))
		{
			// Update informations map
			mapSpeed[type] = 6.4; //type.topSpeed();
			mapAcceleration[type] = 9; //type.acceleration();
			mapTurn[type] = 3; //type.turnRadius();
			mapAttackTime[type] = 1; // TODO FIND A WAY TO COMPUTE THIS VALUE
			mapAttackRange[type] = 160; //type.groundWeapon().maxRange();

			// Matrix not already existing
			matrixInfluence[type] = (double**) malloc(mapWidth*sizeof(double*));
			for(int i=0;i<mapWidth;i++)
			{
				matrixInfluence[type][i] = (double*) malloc(mapHeight*sizeof(double));
				for(int j=0;j<mapHeight;j++)
				{
					matrixInfluence[type][i][j] = 0;
				}
			}
		}
	}

	// Create informations map for each type of ennemy units
	for (auto& enemy : Broodwar->enemies())
	{
		for (auto& enemyUnit : enemy->getUnits())
		{
			auto type = enemyUnit->getType();
			auto iter = mapSpeed.find(type);
			if (!(iter != mapSpeed.end()))
			{
				// Update informations map
				mapSpeed[type] = 4; //type.topSpeed();
				mapAcceleration[type] = 2; //type.acceleration();
				mapTurn[type] = 1; //type.turnRadius();
				mapAttackTime[type] = 2; // TODO FIND A WAY TO COMPUTE THIS VALUE
				mapAttackRange[type] = 15; //type.groundWeapon().maxRange();
			}
		}
	}
}


	// Methods
void InfluenceMap::update()
{
	// Update the influence map of each unit type
	for(auto currentMap = matrixInfluence.begin(); currentMap != matrixInfluence.end();++currentMap)
	{
		auto type = currentMap->first;
		auto matrix = currentMap->second;
		// Compute kitting type for the unit type
		double k = 1;
		double kitingTime = mapAttackTime[type]
							+ mapAcceleration[type] 
							+ 2 * mapTurn[type];

		// Update influence map of this type
		for(int i=0;i<mapWidth;i++)
		{
			for(int j=0;j<mapHeight;j++)
			{
				// Reset influence
				matrix[i][j] = 0;

				// Recompute influence of all enemies
				for (auto& enemy : Broodwar->enemies())
				{
					for (auto& enemyUnit : enemy->getUnits())
					{
						auto enemyType = enemyUnit->getType();
						double dmax = mapAttackRange[type];
						//+ k
						//+ mapSpeed[enemyType] * kitingTime;
						Position cellPosition(i * 32, j * 32);
						double d = cellPosition.getApproxDistance(enemyUnit->getPosition());
						if (d < dmax)
						{
							matrix[i][j] += Broodwar->getDamageFrom(type, enemyType) / mapAttackTime[enemyType];
						}
					}
				}
			}
		}
	}

	debugDisplayInfluence();
}

void InfluenceMap::debugDisplayInfluence()
{
	// Draw range
	//Broodwar->drawTextScreen(50, 40,  "Zealot speed: %f", mapSpeed[UnitTypes::Protoss_Zealot]);
	//Broodwar->drawTextScreen(50, 50,  "Zealot acceleration: %f", mapAcceleration[UnitTypes::Protoss_Zealot]);
	//Broodwar->drawTextScreen(50, 60,  "Zealot turn: %f", mapTurn[UnitTypes::Protoss_Zealot]);
	//Broodwar->drawTextScreen(50, 70,  "Zealot attack time: %f", mapAttackTime[UnitTypes::Protoss_Zealot]);
	//Broodwar->drawTextScreen(50, 80,  "Zealot attack range: %f", mapAttackRange[UnitTypes::Protoss_Zealot]);
	//Broodwar->drawTextScreen(50, 90,  "Vulture speed: %f", mapSpeed[UnitTypes::Terran_Vulture]);
	//Broodwar->drawTextScreen(50, 100,  "Vulture acceleration: %f", mapAcceleration[UnitTypes::Terran_Vulture]);
	//Broodwar->drawTextScreen(50, 110,  "Vulture turn: %f", mapTurn[UnitTypes::Terran_Vulture]);
	//Broodwar->drawTextScreen(50, 120,  "Vulture attack time: %f", mapAttackTime[UnitTypes::Terran_Vulture]);
	//Broodwar->drawTextScreen(50, 130,  "Vulture attack range: %f", mapAttackRange[UnitTypes::Terran_Vulture]);

	int unitKind = matrixInfluence.size();
	if(unitKind == 0)
		return;
	
	int currentColorIndex = -1;
	for(auto currentMap = matrixInfluence.begin(); currentMap != matrixInfluence.end();++currentMap)
	{
		currentColorIndex++;
		auto type = currentMap->first;
		auto matrix = currentMap->second;
	
		// Find max
		double max = 0;
		for(int i=0;i<mapWidth;i++)
		{
			for(int j=0;j<mapHeight;j++)
			{
				if(matrix[i][j] > max)
					max = matrix[i][j];
			}
		}
	
		for(int i=0;i<mapWidth;i++)
		{
			for(int j=0;j<mapHeight;j++)
			{
				double radius = 16*matrix[i][j] / max;
				if(radius > 0)
				{
					Position cellPosition(i*32,j*32);
					//Broodwar->drawTextMap(cellPosition,"%f",matrix[i][j]);
					Broodwar->drawEllipseMap(cellPosition, (int)radius, (int)radius, Color((int)(radius*(255.0 / unitKind)), 0, 0), false);
					//Broodwar->drawBoxMap(cellPosition-Position(16,16),cellPosition+Position(16,16),Color(radius*(255/max),0,0),true);
				}
			}
		}
	}
}