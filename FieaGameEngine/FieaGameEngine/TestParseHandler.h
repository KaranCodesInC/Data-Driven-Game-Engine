#pragma once
#include "ParseCoordinator.h"
#include "IParseHandler.h"

namespace Fiea::GameEngine
{
	class TestParseHandler final : public IParseHandler {
	public:
		class Wrapper : public ParseCoordinator::Wrapper {
			RTTI_DECLARATIONS(TestParseHandler::Wrapper, ParseCoordinator::Wrapper);

		public:
			// this is the "output" for this wrapper
			std::size_t maxDepth{ 0 };

			//bool Equals(const RTTI* rhs) const;
			std::string ToString() const override;

			bool Equals(const RTTI* rhs) const override;

			void IncrementDepth() override;
			void DecrementDepth() override;
			size_t Depth() const override;
		};
	

	void Initialize() override;
	//void Cleanup() override;
	bool StartHandler(ParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArrayElement) override;
	bool EndHandler(Json::Value& value, ParseCoordinator::Wrapper& wrapper, const std::string& key) override;

	//bool Equals(const RTTI* rhs) const;
	//std::string ToString() const;

	bool initializeCalled{ false };
	bool cleanupCalled{ false };
	size_t startCount{ 0 };
	size_t endCount{ 0 };
	};
}
