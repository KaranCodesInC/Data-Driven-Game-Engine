#include "pch.h"
#include "ScopeHandler.h"
#include "FloatHandler.h"
#include "AttributedHandler.h"

namespace Fiea
{

	namespace GameEngine
	{
		RTTI_DEFINITIONS(FloatHandler::Wrapper);

		void FloatHandler::Initialize()
		{
			initializeCalled = true;
		}

		void FloatHandler::Wrapper::IncrementDepth()
		{
			++depth;
		}

		void FloatHandler::Wrapper::DecrementDepth()
		{
			--depth;
		}

		size_t FloatHandler::Wrapper::Depth() const
		{
			return depth;
		}

		/**
		 * @brief Handles the start of parsing for floating point values.
		 *
		 * @param wrapper The wrapper providing access to the parsing context.
		 * @param key The key associated with the value being parsed.
		 * @param value The JSON value being parsed.
		 * @param isArrayElement Flag indicating if the value is an element of an array.
		 * @return true if the start of parsing was successful, false otherwise.
		 */
		bool FloatHandler::StartHandler(ParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArrayElement)
		{
			ScopeHandler::Wrapper* scopeWrapper = wrapper.As<ScopeHandler::Wrapper>();
			AttributedHandler::Wrapper* attwrapper = wrapper.As<AttributedHandler::Wrapper>();
			if (scopeWrapper != nullptr)
			{
				if (scopeWrapper->Depth() >= scopeWrapper->maxDepth)
				{
					scopeWrapper->maxDepth = scopeWrapper->Depth();
				}
				if (hasAttribute(key, "float"))
				{
					if (value.isDouble())
					{
						Datum& D = scopeWrapper->GetCurrentScope()->Append(key);
						D.PushBack(value.asFloat());
						++startCount;
						return true;
					}

					if (value.isArray())
					{
						if (value[0].isDouble())
						{
							for (unsigned int i = 0; i < value.size(); ++i)
							{
								Datum& D = scopeWrapper->GetCurrentScope()->Append(key);
								D.PushBack(value[i].asFloat());
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
				if (hasAttribute(key, "float"))
				{
					if (value.isDouble())
					{
						Datum& D = attwrapper->GetCurrentScope()->Append(key);
						D.PushBack(value.asFloat());
						++startCount;
						return true;
					}

					if (value.isArray())
					{
						if (value[0].isDouble())
						{
							for (unsigned int i = 0; i < value.size(); ++i)
							{
								Datum& D = attwrapper->GetCurrentScope()->Append(key);
								D.PushBack(value[i].asFloat());
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
		 * @brief Handles the end of parsing for floating point values.
		 *
		 * @param value The JSON value being parsed.
		 * @param wrapper The wrapper providing access to the parsing context.
		 * @param key The key associated with the value being parsed.
		 * @return true if the end of parsing was successful, false otherwise.
		 */
		bool FloatHandler::EndHandler(Json::Value& value, ParseCoordinator::Wrapper& wrapper, const std::string& key)
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
