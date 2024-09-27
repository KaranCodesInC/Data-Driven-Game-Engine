#include "pch.h"
#include "Action.h"
#include "GameObject.h"
#include <vector>

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(Action);

	Action::Action() : Attributed(TypeIdClass())
	{
		
	}

	Action::Action(size_t id) : Attributed(id)
	{

	}

	std::vector<signature> Action::Signatures()
	{
		return std::vector<signature>
		{
			{ "this"s, Datum::DatumType::Pointer, 1, 0 },
			{ "Actions"s, Datum::DatumType::String, 1, offsetof(Action, name) }
		};
	}

	void Action::setParentGameObject(GameObject* parent)
	{
		parentGameObject = parent;
	}

	/*Action* Action::Clone() const
	{
		return new Action(*this);
	}*/
}
