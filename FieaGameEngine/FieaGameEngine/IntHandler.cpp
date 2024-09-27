#include "pch.h"
#include "IntHandler.h"
#include "ScopeHandler.h"
#include "AttributedHandler.h"

namespace Fiea
{
	
	namespace GameEngine
	{
		RTTI_DEFINITIONS(IntHandler::Wrapper);

		void IntHandler::Initialize()
		{
			initializeCalled = true;
		}

		void IntHandler::Wrapper::IncrementDepth()
		{
			//++maxDepth;
			++depth;
		}

		void IntHandler::Wrapper::DecrementDepth()
		{
			--depth;
		}

		size_t IntHandler::Wrapper::Depth() const
		{
			return depth;
		}


		/**
		 * @brief Attempt to start parsing an integer value.
		 *
		 * This function checks if the current wrapper is of type ScopeHandler::Wrapper and then checks if
		 * the JSON value is an integer or an array of integers. If the conditions are met, it appends the
		 * integer(s) to the current scope and increments the start count.
		 *
		 * @param wrapper The wrapper associated with the parsing context.
		 * @param key The key associated with the JSON value.
		 * @param value The JSON value to parse.
		 * @param isArrayElement Flag indicating whether the value is an element of an array.
		 * @return true if parsing is successful, false otherwise.
		 */
		bool IntHandler::StartHandler(ParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArrayElement)
		{
			ScopeHandler::Wrapper* scopeWrapper = wrapper.As<ScopeHandler::Wrapper>();
			AttributedHandler::Wrapper* attwrapper = wrapper.As<AttributedHandler::Wrapper>();
			if (scopeWrapper != nullptr)
			{
				if (scopeWrapper->Depth() >= scopeWrapper->maxDepth)
				{
					scopeWrapper->maxDepth = scopeWrapper->Depth();
				}
				if (hasAttribute(key, "int"))
				{
					if (value.isInt())
					{
						Datum& D = scopeWrapper->GetCurrentScope()->Append(key);
						D.PushBack(value.asInt());
						++startCount;
						return true;
					}

					if (value.isArray())
					{
						if (value[0].isInt())
						{
							for (unsigned int i = 0; i < value.size(); ++i)
							{
								Datum& D = scopeWrapper->GetCurrentScope()->Append(key);
								D.PushBack(value[i].asInt());
								++startCount;
							}
						}
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
				if (hasAttribute(key, "int"))
				{
					if (value.isInt())
					{
						Datum& D = attwrapper->GetCurrentScope()->Append(key);
						D.PushBack(value.asInt());
						++startCount;
						return true;
					}

					if (value.isArray())
					{
						if (value[0].isInt())
						{
							for (unsigned int i = 0; i < value.size(); ++i)
							{
								Datum& D = attwrapper->GetCurrentScope()->Append(key);
								D.PushBack(value[i].asInt());
								++startCount;
							}
						}
						return true;
					}
				}
			}
			
			return false;
		}


		/**
		 * @brief Handles the end of parsing for an integer value.
		 *
		 * @param value The JSON value being parsed.
		 * @param wrapper The wrapper object for handling scope and depth.
		 * @param key The key associated with the parsed value.
		 * @return true if the end of parsing is successful, false otherwise.
		 */
		bool IntHandler::EndHandler(Json::Value& value, ParseCoordinator::Wrapper& wrapper, const std::string& key)
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
	}
}