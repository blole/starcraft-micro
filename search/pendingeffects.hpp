#pragma once
#include <vector>
#include <list>

namespace Bot { namespace Search
{
	class Action;

	class PendingEffects
	{
	private:
		std::vector<std::list<Action*>> effects;

	public:
		std::list<Action*> getEffects(int frameOffset)	{return effects[frameOffset];}
		
		void addEffect(unsigned int frameOffset, Action* action)
		{
			if (effects.size() <= frameOffset)
				effects.resize(frameOffset+1);

			effects[frameOffset].push_back(action);
		}
		void advanceFrames(unsigned int framesToAdvance)
		{
			effects.erase(effects.begin(), effects.begin() + framesToAdvance);
		}
	};
}}
