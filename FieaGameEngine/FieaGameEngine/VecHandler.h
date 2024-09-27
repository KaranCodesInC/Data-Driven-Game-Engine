#pragma once
#include "ParseCoordinator.h"
#include "IParseHandler.h"
#include <glm/glm.hpp>

namespace Fiea::GameEngine
{
	class VecHandler final : public IParseHandler
	{
	public:
		class Wrapper :public ParseCoordinator::Wrapper
		{
			RTTI_DECLARATIONS(VecHandler::Wrapper, ParseCoordinator::Wrapper);

		public:

			int maxDepth{ 0 };

			void IncrementDepth() override;
			void DecrementDepth() override;
			size_t Depth() const override;

			bool Equals(const RTTI* rhs) const;
			std::string ToString() const;

		private:
			
		};

		void Initialize() override;
		//void Cleanup() override;
		bool StartHandler(ParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArrayElement) override;
		bool EndHandler(Json::Value& value, ParseCoordinator::Wrapper& wrapper, const std::string& key) override;

		bool initializeCalled{ false };
		bool cleanupCalled{ false };
		size_t startCount{ 0 };
		size_t endCount{ 0 };

	private:
		bool isVector(Json::Value& jsonValue);
	};
}

