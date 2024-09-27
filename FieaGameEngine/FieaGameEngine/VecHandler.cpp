#include "pch.h"
#include <glm/glm.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include "VecHandler.h"
#include "ScopeHandler.h"
#include <json/json.h>
#include "AttributedHandler.h"

namespace Fiea
{

	namespace GameEngine
	{
		RTTI_DEFINITIONS(VecHandler::Wrapper);

		void VecHandler::Initialize()
		{
			initializeCalled = true;
		}

		void VecHandler::Wrapper::IncrementDepth()
		{
			++depth;
		}

		void VecHandler::Wrapper::DecrementDepth()
		{
			--depth;
		}

		size_t VecHandler::Wrapper::Depth() const
		{
			return depth;
		}

	/**
	 * @brief Checks if the JSON value represents a valid 4-dimensional vector.
	 *
	 * @param value The JSON value to check.
	 * @return True if the value represents a valid vector, false otherwise.
	 */
		bool VecHandler::StartHandler(ParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArrayElement)
		{
			ScopeHandler::Wrapper* scopeWrapper = wrapper.As<ScopeHandler::Wrapper>();
			AttributedHandler::Wrapper* attwrapper = wrapper.As<AttributedHandler::Wrapper>();
			if (scopeWrapper != nullptr)
			{
				if (scopeWrapper->Depth() >= scopeWrapper->maxDepth)
				{
					scopeWrapper->maxDepth = scopeWrapper->Depth();
				}
				if (hasAttribute(key, "vec"))
				{
					if (isVector(value))
					{
						glm::vec4 vec = glm::vec4(value[0].asFloat(), value[1].asFloat(), value[2].asFloat(), value[3].asFloat());
						Datum& D = scopeWrapper->GetCurrentScope()->Append(key);
						D.PushBack(vec);
						++startCount;
						return true;
					}
				}
				
			}
			if (attwrapper != nullptr)
			{
				if (attwrapper->Depth() >= attwrapper->maxDepth)
				{
					attwrapper->maxDepth = attwrapper->Depth();
				}
				if (hasAttribute(key, "vec"))
				{
					if (isVector(value))
					{
						glm::vec4 vec = glm::vec4(value[0].asFloat(), value[1].asFloat(), value[2].asFloat(), value[3].asFloat());
						Datum& D = attwrapper->GetCurrentScope()->Append(key);
						D.PushBack(vec);
						++startCount;
						return true;
					}
				}

			}
			return false;
		}

		/**
		 * @brief Ends handling of a JSON object representing a vector.
		 *
		 * @param value The JSON value that was being handled.
		 * @param wrapper The wrapper containing contextual information.
		 * @param key The key of the JSON object.
		 * @return True if handling ended successfully, false otherwise.
		 */
		bool VecHandler::EndHandler(Json::Value& value, ParseCoordinator::Wrapper& wrapper, const std::string& key)
		{
			ScopeHandler::Wrapper* scopeWrapper = wrapper.As<ScopeHandler::Wrapper>();
			AttributedHandler::Wrapper* attwrapper = wrapper.As<AttributedHandler::Wrapper>();
			if (scopeWrapper && attwrapper == nullptr)
			{
				return false;
			}

			++endCount;
			return true;
		}

//---------------------------------------------------------------------------------------------------------------------------------

	/**
	 * @brief Checks if the given JSON value represents a vector.
	 *
	 * @param value The JSON value to check.
	 * @return true if the value represents a vector, false otherwise.
	 */
		bool VecHandler::isVector(Json::Value& value)
		{
			// Check if the value is an array
			if (!value.isArray())
			{
				return false;
			}

			// Check if the array has 4 elements
			if (value.size() != 4)
			{
				return false;
			}

			// Check if all elements are numerical values
			for (unsigned int i = 0; i < 4; ++i)
			{
				if (!value[i].isNumeric())
				{
					return false;
				}
			}

			// If all conditions are met, it is likely a vector
			return true;
		}

	}
}

