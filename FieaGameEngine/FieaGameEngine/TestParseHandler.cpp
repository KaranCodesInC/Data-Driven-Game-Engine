#include "pch.h"
#include "TestParseHandler.h"

namespace Fiea::GameEngine
{
#pragma region TestParseHandler::Wrapper
	RTTI_DEFINITIONS(TestParseHandler::Wrapper);

	/**
	 * @brief RTTI override for Equals
	 * @param rhs
	 * @return true if equal, false otherwise
	*/
	bool TestParseHandler::Wrapper::Equals(const RTTI* rhs) const {
		if (rhs == nullptr) return false;

		const TestParseHandler::Wrapper* other = rhs->As<TestParseHandler::Wrapper>();
		return maxDepth == other->maxDepth;
	}

	/**
	 * @brief RTTI override for ToString
	 * @return string representation of this Wrapper
	*/
	std::string TestParseHandler::Wrapper::ToString() const {
		return "TestParseHandler: maxDepth=" + std::to_string(maxDepth);
	}

	void TestParseHandler::Wrapper::IncrementDepth()
	{
		++depth;
		//++maxDepth;
	}

	void TestParseHandler::Wrapper::DecrementDepth()
	{
		--depth;
	}

	size_t TestParseHandler::Wrapper::Depth() const
	{
		return depth;
	}
#pragma endregion TestParseHandler::Wrapper

#pragma region TestParseHandler
	/**
	 * @brief Override for IParseHandler::Initialize
	*/
	void TestParseHandler::Initialize() {
		initializeCalled = true;
	}

	/**
	 * @brief Override for IParseHandler::Cleanup
	*/
	/*void TestParseHandler::Cleanup() {
		cleanupCalled = true;
	}*/

	/**
	 * @brief Override for IParseHandler::Start
	 * @param unused
	 * @param unused
	 * @param wrapper The wrapper to populate
	 * @return True, if handled, false otherwise
	*/
	bool TestParseHandler::StartHandler(ParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArrayElement) {
		TestParseHandler::Wrapper* testWrapper = wrapper.As<TestParseHandler::Wrapper>();
		if (testWrapper == nullptr) return false;

		if (testWrapper->Depth() >= testWrapper->maxDepth) {
			testWrapper->maxDepth = testWrapper->Depth();
		}
		++startCount;
		return true;
	}

	/**
	 * @brief Override for IParseHandler::End
	 * @param unused
	 * @param wrapper The wrapper to populate
	 * @return True, if handled, false otherwise
	*/
	bool TestParseHandler::EndHandler(Json::Value& value, ParseCoordinator::Wrapper& wrapper, const std::string& key) {
		Wrapper* testWrapper = wrapper.As<TestParseHandler::Wrapper>();
		if (testWrapper == nullptr) return false;

		++endCount;
		return true;
	}

	/*void TestParseHandler::Cleanup()
	{}*/

#pragma endregion TestParseHandler
}
