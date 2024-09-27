#include "pch.h"
#include "EventHandler.h"

namespace Fiea::GameEngine
{	

	/**
	 * @brief Enqueues an event with a specified delay.
	 * @param event Pointer to the event to be enqueued.
	 * @param currentTime Reference to the current game time.
	 * @param delayTime Delay time till the event should be processed.
	 */
	void EventHandler::Enqueue(IEvent* event, const GameTime& currentTime, const GameTime::Millis delayTime)
	{
		event->SetEnqueueAndDelay(currentTime, delayTime);
		mEvent->push_back(event);
		if (mEvent == nullptr)
		{
			mEvent = new std::vector<IEvent*>();
		}
	}

	/**
	 * @brief Updates the event handler by processing queued events that have expired.
	 * @param gameTime Reference to the current game time.
	 */
    void EventHandler::Update(const GameTime& gameTime)
    {
        // Iterate through the events
        for (auto it = mEvent->begin(); it != mEvent->end();)
        {
            // Check if the event is expired
            if ((*it)->IsExpired(gameTime))
            {
                // Publish the event
                (*it)->Deliver(gameTime);

                // Remove the event from the vector if it's marked for deletion after invocation
                if ((*it)->DeleteAfterPublishing())
                {
                    delete* it;
                    it = mEvent->erase(it);
                    continue; // Skip the increment because erase already moves the iterator
                }
            }

            // Move to the next event
            ++it;
        }
    }

	/**
	 * @brief Adds subscribers for a specific event ID.
	 * @param id The ID of the event.
	 * @param s Reference to the subscriber to be added.
	 */
    void EventHandler::AddSubscribers(size_t id, Subscribers& s)
    {
        if (mSubs == nullptr)
        {
            mSubs = new std::unordered_map<size_t, std::vector<Subscribers*>>();
        }

        auto result = mSubs->emplace(id, std::vector<Subscribers*>());

        result.first->second.push_back(&s);
    }


	/**
	 * @brief Removes a subscriber for a specific event ID.
	 * @param id The ID of the event.
	 * @param s Reference to the subscriber to be removed.
	 */
	void EventHandler::RemoveSubscribers(size_t id, Subscribers& s)
	{
		if (mSubs != nullptr)
		{
			auto result = mSubs->find(id);
			if (result->first)
			{
				std::vector<Subscribers*>& _subs = result->second;
				size_t index = 0;
				for (auto iter = _subs.begin(); iter != _subs.end(); ++iter, ++index)
				{
					if (*iter == &s)
					{
						_subs.erase(_subs.begin() + index);
						break;
					}
				}
			}
		}
	}

	/**
	 * @brief Removes a subscriber from all events.
	 * @param s Pointer to the subscriber to be removed.
	 */
	void EventHandler::RemoveSubscriberFromAllEvents(Subscribers* s)
	{
		if (s == nullptr)
			return;

		for (auto& entry : *mSubs)
		{
			if (entry.second.size() == 0)
			{
				continue;
			}
			else
			{
				auto pos = std::find(entry.second.begin(), entry.second.end(), s);
				if (pos != entry.second.end())
				{
					entry.second.erase(pos);
				}
			}
		}

	}


	/**
	 * @brief Gets the total number of subscribers across all events.
	 * @return The total number of subscribers.
	 */
	size_t EventHandler::GetAllSubs()
	{
		if (mSubs != nullptr)
		{
			size_t count = 0;
			for (auto& vec : *mSubs)
			{
				count += vec.second.size();
			}
			return count;
		}
		else
		{
			return 0;
		}

	}

	/**
	 * @brief Cleans up memory by deleting all events and subscribers.
	 */
	void EventHandler::Cleanup()
	{
		if (mEvent != nullptr)
		{
			for (auto ev : *mEvent)
			{
				delete ev;
			}
			mEvent->clear();
			delete mEvent;
			mEvent = nullptr;
		}

		if (mEvent != nullptr)
		{
			RemoveAllSubs();
			delete mSubs;
			mSubs = nullptr;
		}
	}

	/**
	 * @brief Clears all events.
	 */
	void EventHandler::Clear()
	{
		for (auto event : *mEvent)
		{
			delete event;
		}
		mEvent->clear();
	}
    
	/**
	 * @brief Removes all subscribers from all events.
	 */
	void EventHandler::RemoveAllSubs()
	{
		if (mSubs != nullptr)
		{
			mSubs->clear();
		}

	}

	/**
	 * @brief Gets the number of events currently in the event handler.
	 * @return The number of events.
	 */
	size_t EventHandler::Size()
	{
		if (mEvent)
		{
			return mEvent->size();
		}
		return size_t(0);
	}

	/**
	 * @brief Checks if the event handler is empty.
	 * @return True if the event handler is empty, false otherwise.
	 */
	bool EventHandler::IsEmpty()
	{
		if (mEvent)
		{
			return mEvent->empty();
		}
		return true;

	}
}
