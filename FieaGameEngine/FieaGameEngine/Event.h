#pragma once
#include "IEvent.h"

namespace Fiea::GameEngine
{
	template<class Payload>
	class Event final : public IEvent
	{
		RTTI_DECLARATIONS(Event<Payload>, IEvent);

	public:

		Event() = delete;

		Event(const Payload& payload, bool deleteAfterInvoke = true) : IEvent(deleteAfterInvoke), message(payload) {};

		Event(const Event& rhs) = default;
		Event& operator=(const Event& rhs) = default;
		Event(Event&& rhs) = default;
		Event& operator=(Event&& rhs) = default;

		~Event() override {}

		Payload Message() const
		{
			return message;
		};

	private:
		Payload message;

	};
}

