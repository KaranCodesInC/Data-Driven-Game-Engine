#pragma once
#include "ParseCoordinator.h"
#include "IParseHandler.h"

namespace Fiea::GameEngine
{
	class FloatHandler final : public IParseHandler
	{
	public:
		class Wrapper :public ParseCoordinator::Wrapper
		{
			RTTI_DECLARATIONS(FloatHandler::Wrapper, ParseCoordinator::Wrapper);

		public:

			int maxDepth{ 0 };

			void SetFloat(int i);

			void IncrementDepth() override;
			void DecrementDepth() override;
			size_t Depth() const override;

			bool Equals(const RTTI* rhs) const;
			std::string ToString() const;

		private:
			std::vector<float> floatVec;
			float i;
		};

		void Initialize() override;
		//void Cleanup() override;
		bool StartHandler(ParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArrayElement) override;
		bool EndHandler(Json::Value& value, ParseCoordinator::Wrapper& wrapper, const std::string& key) override;

		bool initializeCalled{ false };
		bool cleanupCalled{ false };
		size_t startCount{ 0 };
		size_t endCount{ 0 };
	};
}
