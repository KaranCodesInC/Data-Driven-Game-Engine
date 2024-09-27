#pragma once
#include <string>
#include <unordered_map>
#include "Scope.h"
#include "AttributedFoo.h"
#include "AttributedBar.h"
#include "AttributedAssassin.h"
#include "GameObject.h"
#include "Action.h"
#include "ActionListWhile.h"
#include "ActionList.h"
#include "ActionIncrement.h"
#include "Sorcerer.h"

namespace Fiea
{
	namespace GameEngine
	{
		template<class BaseType>
		class Factory
		{
		public:
			virtual BaseType* create() = 0;
			virtual std::string className() = 0;
			virtual ~Factory() = default;
		};
	}
}

#define MAKE_FACTORY(Base, Concrete) \
		class Concrete##Factory : public Factory<Base> { \
		public: \
			Fiea::GameEngine::Concrete* create() override { \
				return new Concrete(); \
			} \
			\
			std::string className() override { \
				return #Concrete; \
			} \
			\
		};
