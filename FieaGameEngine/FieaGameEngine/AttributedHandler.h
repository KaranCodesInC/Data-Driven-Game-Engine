#pragma once
#include "ParseCoordinator.h"
#include "IParseHandler.h"
#include "scope.h"
#include <json/json.h>
#include <glm/glm.hpp>

namespace Fiea::GameEngine
{
	class AttributedHandler : public IParseHandler
	{
	public:
		class Wrapper : public ParseCoordinator::Wrapper
		{
			RTTI_DECLARATIONS(AttributedHandler::Wrapper, ParseCoordinator::Wrapper);

		public:
			Wrapper();
			~Wrapper();

			Scope* GetRoot() const { return rootScope; }
			Scope* GetCurrentScope() const { return currentScope; }
			Scope* SetCurrentScope(Scope* scope);
			Scope* resetParent(Scope* scope);
			void setRoot(Scope* scope);

			std::size_t maxDepth{ 0 };

			bool Equals(const RTTI* rhs) const;
			std::string ToString() const;

			void IncrementDepth() override;
			void DecrementDepth() override;
			size_t Depth() const override;

		private:
			Scope* rootScope = nullptr;
			Scope* currentScope = nullptr;

		};
		void Initialize() override;
		//void Cleanup() override;

		bool StartHandler(ParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArrayElement) override;
		bool EndHandler(Json::Value& value, ParseCoordinator::Wrapper& wrapper, const std::string& key) override;



		bool initializeCalled{ false };
		size_t startCount{ 0 };
		size_t endCount{ 0 };
	};
}
