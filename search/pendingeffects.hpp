#pragma once
#include <vector>
#include <list>
#include <cassert>

namespace Bot { namespace Search
{
	class Action;

	class PendingEffects
	{
	private:
		std::vector<std::list<Action*>> effects;

	public:
		const std::list<Action*>& getEffects(int frameOffset)
		{
			assert(frameOffset > 0);

			return effects[frameOffset-1];
		}
		
		void addEffect(unsigned int frameOffset, Action* action)
		{
			assert(frameOffset > 0);

			if (effects.size() < frameOffset)
				effects.resize(frameOffset);

			effects[frameOffset-1].push_back(action);
		}
		void advanceFrames(unsigned int framesToAdvance)
		{
			assert(framesToAdvance > 0);

			effects.erase(effects.begin(), effects.begin() + framesToAdvance);
		}
	};
}}
