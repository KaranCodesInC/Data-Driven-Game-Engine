#include "pch.h"
#include "ActionList.h"
#include "ActionListWhile.h"
#include "FactoryManager.h"

using namespace std::string_literals;

namespace Fiea
{
	namespace GameEngine
	{
		RTTI_DEFINITIONS(ActionListWhile);

		ActionListWhile::ActionListWhile() : ActionList(ActionListWhile::TypeIdClass())
		{
		}

		/*ActionListWhile::ActionListWhile(size_t id) : ActionList(id)
		{
			this->Append("Actions").SetType(Datum::DatumType::Table);
		}*/

		void ActionListWhile::SetPreamble(Action* action)
		{
			if (action == nullptr)
				throw std::invalid_argument("Cannot set a nullptr as the preamble action");

			preamble = action;
			Adopt(std::move(preamble), "PreambleObj"s);
		}

		std::vector<signature> ActionListWhile::Signatures()
		{
			return std::vector<signature>
			{
				{ "this"s, Datum::DatumType::Pointer, 1, 0 },
				{ "IntCondition"s, Datum::DatumType::Integer, 1, offsetof(ActionListWhile, condition) },
				{ "PreamblePrescribed"s, Datum::DatumType::Table, 0, 0 },
				{ "IncrementPrescribed"s, Datum::DatumType::Table, 0, 0 }
			};
		}
		void ActionListWhile::Update(const GameTime& time)
		{
			if (!isConditionSet)
				return;

			if (preamble != nullptr)
				preamble->Update(time);

			while (condition > 0)
			{
				for (size_t i = 0; i < actions->ScopeSize(); i++)
				{
					(*actions)[i].GetScope(0)->As<Action>()->Update(time);
				}

				if (increment != nullptr)
					increment->Update(time);

				condition--;
			}
		}

		void ActionListWhile::SetCondition(int num)
		{
			condition = num;
			isConditionSet = true;
		}

		void ActionListWhile::SetIncrement(Action* action)
		{
			if (action == nullptr)
				throw std::invalid_argument("Cannot set a nullptr as the increment action");

			increment = action;
			Adopt(std::move(increment), "Increment"s);
		}

		ActionListWhile* ActionListWhile::Clone() const
		{
			return new ActionListWhile(*this);
		}
	}
}
