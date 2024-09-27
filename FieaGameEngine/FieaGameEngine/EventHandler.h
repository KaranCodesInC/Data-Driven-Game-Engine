#pragma once
#include "IEvent.h"

namespace Fiea::GameEngine
{
	class EventHandler
	{
	public:

		static void Enqueue(IEvent* event, const GameTime& currentTime, const GameTime::Millis delayTime = 0);
		static void Update(const GameTime& gameTime);
		static void Clear();
		static bool IsEmpty();
		static size_t Size();
		void Cleanup();

		static void AddSubscribers(size_t id, Subscribers& s);
		static void RemoveSubscribers(size_t id, Subscribers& s);
		static void RemoveSubscriberFromAllEvents(Subscribers* s);
		static void RemoveAllSubs();
		static size_t GetAllSubs();

	private:

		inline static std::vector<IEvent*>* mEvent;
		inline static std::unordered_map<size_t, std::vector<Subscribers*>>* mSubs;

	};
}
