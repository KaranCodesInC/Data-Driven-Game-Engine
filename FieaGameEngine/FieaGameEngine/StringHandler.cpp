#include "pch.h"
#include "ScopeHandler.h"
#include "StringHandler.h"
#include "AttributedHandler.h"

namespace Fiea
{

	namespace GameEngine
	{
		RTTI_DEFINITIONS(StringHandler::Wrapper);

		void StringHandler::Initialize()
		{
			initializeCalled = true;
		}

		void StringHandler::Wrapper::IncrementDepth()
		{
			++depth;
		}

		void StringHandler::Wrapper::DecrementDepth()
		{
			--depth;
		}

		size_t StringHandler::Wrapper::Depth() const
		{
			return depth;
		}

		/**
		 * @brief Handles the start of parsing for a string value.
		 *
		 * @param wrapper The wrapper object for handling scope and depth.
		 * @param key The key associated with the parsed value.
		 * @param value The JSON value being parsed.
		 * @param isArrayElement Indicates whether the value is part of an array.
		 * @return true if the start of parsing is successful, false otherwise.
		 */
		bool StringHandler::StartHandler(ParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArrayElement)
		{
			ScopeHandler::Wrapper* scopeWrapper = wrapper.As<ScopeHandler::Wrapper>();
			AttributedHandler::Wrapper* attwrapper = wrapper.As<AttributedHandler::Wrapper>();
			if (scopeWrapper != nullptr)
			{
				if (scopeWrapper->Depth() >= scopeWrapper->maxDepth)
				{
					scopeWrapper->maxDepth = scopeWrapper->Depth();
				}
				if (hasAttribute(key, "str"))
				{
					if (value.isString())
					{
						Datum& D = scopeWrapper->GetCurrentScope()->Append(key);
						D.PushBack(value.asString());
						++startCount;
						return true;
					}

					if (value.isArray())
					{
						if (value[0].isString())
						{
							for (unsigned int i = 0; i < value.size(); ++i)
							{
								Datum& D = scopeWrapper->GetCurrentScope()->Append(key);
								D.PushBack(value[i].asString());
								++startCount;
							}
							return true;
						}
					}
				}
			}
			if (attwrapper != nullptr)
			{
				if (attwrapper->Depth() >= attwrapper->maxDepth)
				{
					attwrapper->maxDepth = attwrapper->Depth();
				}
				
				if (hasAttribute(key, "str"))
				{
					if (value.isString())
					{
						Datum& D = attwrapper->GetCurrentScope()->Append(key);
						D.PushBack(value.asString());
						++startCount;
						return true;
					}

					if (value.isArray())
					{
						if (value[0].isString())
						{
							for (unsigned int i = 0; i < value.size(); ++i)
							{
								Datum& D = attwrapper->GetCurrentScope()->Append(key);
								D.PushBack(value[i].asString());
								++startCount;
							}
							return true;
						}
					}
				}
			}
			return false;
		}

		/**
		* @brief Handles the end of parsing for a string value.
		*
		* @param value The JSON value being parsed.
		* @param wrapper The wrapper object for handling scope and depth.
		* @param key The key associated with the parsed value.
		* @return true if the end of parsing is successful, false otherwise.
		*/
		bool StringHandler::EndHandler(Json::Value& value, ParseCoordinator::Wrapper& wrapper, const std::string& key)
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
