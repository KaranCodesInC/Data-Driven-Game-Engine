#pragma once
#include "pch.h"
#include "ScopeHandler.h"
#include "AttributedHandler.h"

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(ScopeHandler::Wrapper);

	Scope* ScopeHandler::Wrapper::SetCurrentScope(Scope* scope)
	{
		prevSubScopes.push_back(currentScope);
		currentScope = scope;
		return currentScope;
	}

	std::string ScopeHandler::Wrapper::ToString() const
	{
		return "Scope-Wrapper maxDepth=" + std::to_string(maxDepth);
	}

	void ScopeHandler::Wrapper::IncrementDepth()
	{
		++depth;
	}

	void ScopeHandler::Wrapper::DecrementDepth()
	{
		--depth;
	}

	size_t ScopeHandler::Wrapper::Depth() const
	{
		return depth;
	}

	Scope* ScopeHandler::Wrapper::resetParent(Scope* scope)
	{
		currentScope = scope->GetParent();
		return currentScope;
	}

	bool ScopeHandler::Wrapper::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr) return false;

		const ScopeHandler::Wrapper* other = rhs->As<ScopeHandler::Wrapper>();
		return rootScope == other->rootScope;
	}

	void ScopeHandler::Initialize()
	{
		 initializeCalled = true;
	}

	
	/**
	 * @brief Handles the start of parsing for a scope.
	 *
	 * @param wrapper The wrapper object for handling scope and depth.
	 * @param key The key associated with the parsed value.
	 * @param value The JSON value being parsed.
	 * @param isArrayElement A flag indicating if the value is an element of an array.
	 * @return true if the start of parsing is successful, false otherwise.
	 */
	bool ScopeHandler::StartHandler(ParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArrayElement)
	{
		ScopeHandler::Wrapper* scopeWrapper = wrapper.As<ScopeHandler::Wrapper>();

		if (scopeWrapper == nullptr) return false;

		if (scopeWrapper->Depth() >= scopeWrapper->maxDepth) {
			scopeWrapper->maxDepth = scopeWrapper->Depth();
		}
		if (value.isObject())
		{
			Scope& nestedScope = scopeWrapper->GetCurrentScope()->AppendScope(key);
			scopeWrapper->SetCurrentScope(&nestedScope);
			++startCount;
			return true;
		}
		return false;
	}


	/**
	 * @brief Handles the end of parsing for a scope.
	 *
	 * @param value The JSON value being parsed.
	 * @param wrapper The wrapper object for handling scope and depth.
	 * @param key The key associated with the parsed value.
	 * @return true if the end of parsing is successful, false otherwise.
	 */
	bool ScopeHandler::EndHandler(Json::Value& value, ParseCoordinator::Wrapper& wrapper, const std::string& key)
	{
		ScopeHandler::Wrapper* scopeWrapper = wrapper.As<ScopeHandler::Wrapper>();
		AttributedHandler::Wrapper* attwrapper = wrapper.As<AttributedHandler::Wrapper>();
		if (scopeWrapper == nullptr) return false;

		if (value.isObject())
		{	
			scopeWrapper->resetParent(scopeWrapper->GetCurrentScope());
			++endCount;
			return true;
		}
		return false;
	}

}

