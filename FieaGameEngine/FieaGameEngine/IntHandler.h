#pragma once
#include "IParseHandler.h"
#include "ParseCoordinator.h"

namespace Fiea::GameEngine
{
	class IntHandler final : public IParseHandler
	{
	public:
		class Wrapper :public ParseCoordinator::Wrapper
		{
			RTTI_DECLARATIONS(IntHandler::Wrapper, ParseCoordinator::Wrapper);

		public:

			int maxDepth{ 0 };

			void SetInt(int i);

			void IncrementDepth() override;
			void DecrementDepth() override;
			size_t Depth() const override;

		private:
			std::vector<int> intVec;
			int i;
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
