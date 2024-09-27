#pragma once
#include "ParseCoordinator.h"
#include "IParseHandler.h"
#include "scope.h"
#include <json/json.h>
#include <glm/glm.hpp>

namespace Fiea::GameEngine
{
	class ScopeHandler : public IParseHandler
	{
	public:
		class Wrapper : public ParseCoordinator::Wrapper
		{
			RTTI_DECLARATIONS(ScopeHandler::Wrapper, ParseCoordinator::Wrapper);
			
		public:
			Wrapper() = delete;
			Wrapper(Scope* scope) : rootScope(scope), currentScope(scope) {};

			Scope* GetRoot() const { return rootScope; }
			Scope* GetCurrentScope() const { return currentScope; }
			Scope* SetCurrentScope(Scope* scope);
			Scope* resetParent(Scope* scope);

			std::size_t maxDepth{ 0 };

			bool Equals(const RTTI* rhs) const;
			std::string ToString() const;

			void IncrementDepth() override;
			void DecrementDepth() override;
			size_t Depth() const override;

		private:
			Scope* rootScope = nullptr;
			Scope* currentScope = nullptr;
			std::vector<Scope*> prevSubScopes;
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
