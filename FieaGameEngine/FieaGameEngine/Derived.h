#pragma once
#include "RTTI.h"
#include "Foo.h"
#include "Empty.h"

namespace Fiea::GameEngine::test
{
	class derived : public Empty
	{
		RTTI_DECLARATIONS(derived, Empty);
		//static const RTTI::IdType _typeId;
	public:
		
	};
}