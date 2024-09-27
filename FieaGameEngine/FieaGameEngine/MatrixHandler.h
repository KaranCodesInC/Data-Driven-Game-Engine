#pragma once
#include "ParseCoordinator.h"
#include "IParseHandler.h"
#include <glm/glm.hpp>

namespace Fiea::GameEngine
{
	class MatrixHandler final : public IParseHandler
	{
	public:
		class Wrapper :public ParseCoordinator::Wrapper
		{
			RTTI_DECLARATIONS(MatrixHandler::Wrapper, ParseCoordinator::Wrapper);

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
		bool isMat(Json::Value& jsonValue);
		glm::mat4 JsonToMat4(const Json::Value& value);
	};
}

