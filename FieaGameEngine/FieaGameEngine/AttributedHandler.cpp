#include "pch.h"
#include "AttributedHandler.h"
#include "FactoryManager.h"

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(AttributedHandler::Wrapper);

	bool AttributedHandler::Wrapper::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr) return false;

		const AttributedHandler::Wrapper* other = rhs->As<AttributedHandler::Wrapper>();
		return rootScope == other->rootScope;
	}


	std::string AttributedHandler::Wrapper::ToString() const
	{
		return "AttributedHandler's Wrapper maxDepth=" + std::to_string(maxDepth);
	}

	void AttributedHandler::Initialize()
	{
		initializeCalled = true;
	}

	void AttributedHandler::Wrapper::IncrementDepth()
	{
		//++maxDepth;
		++depth;
	}

	void AttributedHandler::Wrapper::DecrementDepth()
	{
		--depth;
	}

	size_t AttributedHandler::Wrapper::Depth() const
	{
		return depth;
	}

	AttributedHandler::Wrapper::Wrapper()
	{
		rootScope = new Scope();
		SetCurrentScope(rootScope);
	}

	AttributedHandler::Wrapper::~Wrapper()
	{
		delete rootScope;
	}

	void AttributedHandler::Wrapper::setRoot(Scope* scope)
	{
		rootScope = scope;
		SetCurrentScope(rootScope);
	}

	Scope* AttributedHandler::Wrapper::SetCurrentScope(Scope* scope)
	{
		currentScope = scope;
		return currentScope;
	}

	Scope* AttributedHandler::Wrapper::resetParent(Scope* scope)
	{
		currentScope = scope->GetParent();
		return currentScope;
	}
	
	//---------------------------------------------------------------------------------------------------------------------------------------
	/**
	* @brief Starts handling of parsing.
	* @param wrapper The wrapper containing parsing information.
	* @param key The key being parsed.
	* @param value The value being parsed.
	* @param isArrayElement Flag indicating if the value is an array element.
	* @return True if parsing starts successfully, false otherwise.
	*/
	bool AttributedHandler::StartHandler(ParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArrayElement)
	{
		AttributedHandler::Wrapper* AttributedWrapper = wrapper.As<AttributedHandler::Wrapper>();
		if (AttributedWrapper == nullptr) return false;

		if (AttributedWrapper->Depth() >= AttributedWrapper->maxDepth) {
			AttributedWrapper->maxDepth = AttributedWrapper->Depth();
		}
		if (hasAttribute(key, "obj"))
		{
			Scope& nestedScope = AttributedWrapper->GetCurrentScope()->AppendScope(key);
			AttributedWrapper->SetCurrentScope(&nestedScope);
			++startCount;
			return true;
		}

		else if(hasAttribute(key, "Assassin"))
		{
			Scope* currentScope = AttributedWrapper->GetCurrentScope();
			Scope* nestedScope = FactoryManager<Scope>::create("Assassin");
			AttributedWrapper->SetCurrentScope(nestedScope);
			currentScope->Adopt(std::move(nestedScope), key);
			++startCount;
			return true;
		}

		else if (hasAttribute(key, "GameObj"))
		{
			Scope* currentScope = AttributedWrapper->GetCurrentScope();
			Scope* nestedScope = FactoryManager<Scope>::create("GameObj");
			AttributedWrapper->SetCurrentScope(nestedScope);
			currentScope->Adopt(std::move(nestedScope), key);
			++startCount;
			return true;
		}
		else if (hasAttribute(key, "Sorcerer"))
		{
			Scope* currentScope = AttributedWrapper->GetCurrentScope();
			Scope* nestedScope = FactoryManager<Scope>::create("Sorcerer");
			AttributedWrapper->SetCurrentScope(nestedScope);
			currentScope->Adopt(std::move(nestedScope), key);
			++startCount;
			return true;
		}
		return false;

	}

	/**
	* @brief Ends handling of parsing.
	* @param value The value being parsed.
	* @param wrapper The wrapper containing parsing information.
	* @param key The key being parsed.
	* @return True if parsing ends successfully, false otherwise.
	*/
	bool AttributedHandler::EndHandler(Json::Value& value, ParseCoordinator::Wrapper& wrapper, const std::string& key)
	{
		AttributedHandler::Wrapper* AttributedWrapper = wrapper.As<AttributedHandler::Wrapper>();
		if (AttributedWrapper == nullptr) return false;

		if (hasAttribute(key, "Assassin") || hasAttribute(key, "GameObj") || hasAttribute(key, "obj") || hasAttribute(key, "Act") || hasAttribute(key, "Sorcerer"))
		{
			AttributedWrapper->resetParent(AttributedWrapper->GetCurrentScope());
			++endCount;
			return true;
		}
		return false;
	}
}
