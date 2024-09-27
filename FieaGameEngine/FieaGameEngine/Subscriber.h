#pragma once

#include "GameClock.h"

namespace Fiea::GameEngine
{
	class IEvent;
	
	class Subscribers
	{

	public:
		virtual void Notify(const IEvent& event, const GameTime& gameTime) = 0;
	};
}
