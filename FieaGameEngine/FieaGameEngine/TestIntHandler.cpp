#include "pch.h"
#include "TestIntHandler.h"
#include "RTTI.h"

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(TestIntHandler::Wrapper);

	void TestIntHandler::Wrapper::SetInt(int i)
	{
		TestIntHandler::Wrapper::i = i;
	}

	void TestIntHandler::Initialize()
	{
		initializeCalled = true;
	}

	void TestIntHandler::Wrapper::IncrementDepth()
	{
		//++maxDepth;
		++depth;
	}

	void TestIntHandler::Wrapper::DecrementDepth()
	{
		--depth;
	}

	size_t TestIntHandler::Wrapper::Depth() const
	{
		return depth;
	}

	bool TestIntHandler::StartHandler(ParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArrayElement)
	{
		// Increment the start count
		++startCount;

		// Check if the value is an integer
		if (value.isInt())
		{
			// Cast the wrapper to TestIntHandler::Wrapper
			TestIntHandler::Wrapper* intWrapper = wrapper.As<TestIntHandler::Wrapper>();
			if (intWrapper == nullptr)
			{
				return false;
				
			}
			else
			{
				// Add the integer to the intVec
				intWrapper->intVec.push_back(value.asInt());
				return true;
			}
		}
		return false;
	}
	
	bool TestIntHandler::EndHandler(Json::Value& value, ParseCoordinator::Wrapper& wrapper, const std::string& key)
	{
		++endCount;

		// Increment the end count
		TestIntHandler::Wrapper* intWrapper = wrapper.As <TestIntHandler::Wrapper>();
		if (intWrapper == nullptr)
		{
			return false;
		}

		return true;
	}
	
}
