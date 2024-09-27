#pragma once
#include "Factory.h"
#include <memory>

namespace Fiea
{
	namespace GameEngine
	{
		template <class BaseType>
		class FactoryManager
		{
		public:
			static Factory<BaseType>& Find(const std::string& classname);
			static void addFactory(std::unique_ptr<Factory<BaseType>>&& factory);
			static void removeFactory(Factory<BaseType>& factory);
			static BaseType* create(const std::string& classname);
			static void clear();
		private:
			inline static std::unordered_map<std::string, std::unique_ptr<Factory<BaseType>>> factories;
		};


		class AttributedFooFactory : public Factory<Scope> {
		public: Fiea::GameEngine::AttributedFoo* create() override {
			return new AttributedFoo();
		} std::string className() override {
			return "AttributedFoo";
		}
		};;

		class AttributedAssassinFactory : public Factory<Scope> {
		public: Fiea::GameEngine::AttributedAssassin* create() override {
			return new AttributedAssassin();
		} std::string className() override {
			return "Assassin";
		}
		};;

		class GameObjectFactory : public Factory<Scope> {
		public: Fiea::GameEngine::GameObject* create() override {
			return new GameObject();
		} std::string className() override {
			return "GameObj";
		}
		};;

		class ActionListWhileFactory : public Factory<Scope> {
		public: Fiea::GameEngine::ActionListWhile* create() override {
			return new ActionListWhile();
		} std::string className() override {
			return "ActionListWhile";
		}
		};;

		MAKE_FACTORY(Scope, ActionList);

		MAKE_FACTORY(Scope, ActionIncrement);

		MAKE_FACTORY(Scope, Sorcerer);
	}
}

#include "FactoryManager.inl"