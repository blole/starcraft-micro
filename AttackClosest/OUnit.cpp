#include "OUnit.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

	// Constructor
OUnit::OUnit(BWAPI::Unit unit): GameUnit(unit), dammageAttributed(0.0), numberOfAttackers(0) 
{
}
	
	// Methods
bool OUnit::willDie()
{
	return false;
}