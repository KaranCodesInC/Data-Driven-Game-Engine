#include"pch.h"
#include "ActionIncrement.h"
#include "GameObject.h"
#include <cstddef>

using namespace std::string_literals;

namespace Fiea
{
	namespace GameEngine
	{
		RTTI_DEFINITIONS(ActionIncrement);

		
		ActionIncrement::ActionIncrement() : Action(ActionIncrement::TypeIdClass()) {}

		
		std::vector<signature> ActionIncrement::Signatures()
		{
			return std::vector<signature>
			{
				{ "this"s, Datum::DatumType::Pointer, 1, 0 },
				{ "ActionIncrement"s, Datum::DatumType::String, 1, offsetof(ActionIncrement, key) },
				{ "Integer"s, Datum::DatumType::Float, 1, offsetof(ActionIncrement, i) },
				{ "Float"s, Datum::DatumType::Float, 1, offsetof(ActionIncrement, value) }
			};
		}

		void ActionIncrement::Update(const GameTime&)
		{
			if (parentGameObject == nullptr)
				parentGameObject = parentGameObject->GetRoot(parentGameObject);
			if(parentGameObject != nullptr)
			{
				Datum* root = parentGameObject->Find(key);
				if (root != nullptr)
				{
					if (root->Type() == Datum::DatumType::Integer)
					{
						int& a = root->GetInt(0);
						a += i;

					}
					else if (root->Type() == Datum::DatumType::Float)
					{
						root->GetFloat(0) += value;
					}
				}
			}
		}

		ActionIncrement* ActionIncrement::Clone() const
		{
			return new ActionIncrement(*this);
		}
	}
}