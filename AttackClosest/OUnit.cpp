#include "OUnit.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

	// Constructor
OUnit::OUnit(BWAPI::Unit unit): dammageAttributed(0.0), numberOfAttackers(0) 
{
	GameUnit(unit);
}
	
	// Methods
bool OUnit::willDie()
{
	return false;
}