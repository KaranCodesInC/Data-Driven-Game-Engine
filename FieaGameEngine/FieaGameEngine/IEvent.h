#pragma once
#include "RTTI.h"
#include "GameClock.h"
#include <vector>
#include "Subscriber.h"

namespace Fiea::GameEngine
{
    // Forward declaration
    class EventHandler;

    // Interface for events
    class IEvent : public RTTI
    {
        RTTI_DECLARATIONS(IEvent, RTTI);

        friend EventHandler;

    public:
        IEvent() = delete;

        IEvent(const IEvent& rhs) = default;
        IEvent& operator=(const IEvent& rhs) = default;
        IEvent(IEvent&& rhs) = default;
        IEvent& operator=(IEvent&& rhs) = default;

        virtual ~IEvent();

        GameTime::Millis TimeEnqueued();
        GameTime::Millis Delay();
        bool IsExpired(const GameTime& currentTime);
        virtual void Deliver(const GameTime& gameTime) const;
        bool DeleteAfterPublishing() {
            return deleteAfterPublishing_;
        };

        void AddSubscriber(Subscribers& sub);
        void RemoveSubscriber(Subscribers& sub);
        void emptySubs();

    private:
        GameTime::Millis enTime = 0;
        GameTime::Millis delay = 0;
        bool delivered;
        bool deleteAfterPublishing_ = true;

        void SetEnqueueTime(const GameTime& currentTime);

        void SetEnqueueAndDelay(const GameTime& currentTime, const GameTime::Millis delayTime);

    protected:
        std::vector<Subscribers*> subs;
    };
}

