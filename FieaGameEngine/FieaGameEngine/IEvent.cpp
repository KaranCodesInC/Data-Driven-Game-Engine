#include "pch.h"
#include "IEvent.h"

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(IEvent);

    /**
     * @brief Add a subscriber to the event's list
     * @param s Reference to the subscriber to be added
     */
    void IEvent::AddSubscriber(Subscribers& s)
    {
        subs.push_back(&s);
    }

    /**
     * @brief Clear the list of subscribers
     */
    void IEvent::emptySubs()
    {
        subs.clear();
    }

    /**
     * @brief Set the enqueue time of the event
     * @param currentTime Reference to the current game time
     */
    void IEvent::SetEnqueueTime(const GameTime& currentTime)
    {
        enTime = currentTime.Game();
    }

    /**
     * @brief Set the enqueue time and delay of the event
     * @param currentTime Reference to the current game time
     * @param delayTime Delay time till the event should invoke
     */
    void IEvent::SetEnqueueAndDelay(const GameTime& currentTime, const GameTime::Millis delayTime)
    {
        enTime = currentTime.Game();
        delay = delayTime;
    }

    /**
     * @brief Get the time when the event was enqueued
     * @return The time when the event was enqueued
     */
    GameTime::Millis IEvent::TimeEnqueued()
    {
        return enTime;
    }

    /**
     * @brief Get the delay of the event
     * @return The delay of the event
     */
    GameTime::Millis IEvent::Delay()
    {
        return delay;
    }

    /**
     * @brief Remove a subscriber from the event's list
     * @param s Reference to the subscriber to be removed
     */
    void IEvent::RemoveSubscriber(Subscribers& s)
    {
        size_t index = 0;
        for (auto iter = subs.begin(); iter != subs.end(); ++iter, ++index)
        {
            if (*iter == &s)
            {
                subs.erase(subs.begin() + index);
                break;
            }
        }
    }

    /**
     * @brief Check if the event has expired
     * @param currentTime Reference to the current game time
     * @return True if the event has expired, false otherwise
     */
    bool IEvent::IsExpired(const GameTime& currentTime)
    {
        long long i = currentTime.Game();
        if (currentTime.Game() >= enTime + delay)
        {
            return true;
        }

        return false;
    }

    /**
     * @brief Deliver the event to all subscribers
     * @param gameTime Reference to the current game time
     */
    void IEvent::Deliver(const GameTime& gameTime) const
    {
        for (auto listener : subs)
        {
            listener->Notify(*this, gameTime);
        }
    }

}
