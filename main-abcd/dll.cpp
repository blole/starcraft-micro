#include "common/dll.hpp"
#include "common/main.hpp"
#include "common/generalallunitssinglesquad.hpp"
#include "squad_abcd.hpp"

extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule()
{
	using namespace Bot;

	std::function<Squad*()> newSquad = []{
		return new SquadABCD();
	};
	General* general = new GeneralAllUnitsSingleSquad(newSquad);

	return new Main(general);
}
