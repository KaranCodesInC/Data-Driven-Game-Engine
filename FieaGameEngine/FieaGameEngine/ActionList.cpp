#include "pch.h"
#include "ActionList.h"
#include "FactoryManager.h"

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(ActionList);

	ActionList::ActionList() : Action(ActionList::TypeIdClass())
	{
		
		actions = new Scope();
		Adopt(std::move(actions), "Actions");
	}

	ActionList::ActionList(size_t id) : Action(id)
	{
		actions = new Scope();
		Adopt(std::move(actions), "Actions");
	}

	Action* ActionList::CreateAction(const std::string& className)
	{
 		Action* action = FactoryManager<Scope>::create(className)->As<Action>();
		actions->Adopt(std::move(action), className);
		return action;
	}

	void ActionList::addAction(Action* action, const std::string& key)
	{
		if (action == nullptr)
		{
			throw std::invalid_argument("INVALID");
		}

		size_t index = 0;
		if (actions->FindContainedScope(action, index) != nullptr || actions->Find(action->name) != nullptr)
		{
			throw std::invalid_argument("Action already exists");
		}

		// Transfer ownership of the Action to this ActionList
		actions->Adopt(std::move(action), action->name);
	}

	void ActionList::removeAction(const std::string& key)
	{
		Datum* datum = actions->Find(key);
		if (datum == nullptr || datum->Size() == 0 || (*datum)[0].As<Action>() == nullptr)
		{
			throw std::invalid_argument("Action Does Not Exist");
		}

		datum->RemoveAt(0);
	}

	Action* ActionList::getAction(const string& classname)
	{
		Datum* D = actions->Find(classname);
		if (D == nullptr || D->Size() == 0)
		{
			throw std::invalid_argument("Action not Present");
		}

		return D->GetScope(0)->As<Action>();
	}

	std::vector<signature> ActionList::Signatures()
	{
		return std::vector<signature>
		{
			{ "this"s, Datum::DatumType::Pointer, 1, 0 },
			{ "Actions"s, Datum::DatumType::Table, 0, 0 }
		};
	}

	void ActionList::Update(const GameTime& time)
	{
		for (size_t i = 0; i < actions->ScopeSize(); i++)
		{
			for (size_t j = 0; j < (*actions)[i].Size(); j++)
			{
				Action* currentAction = (*actions)[i].GetScope(j)->As<Action>();
				if (currentAction != nullptr)
				{
					currentAction->Update(time);
				}
			}
		}
	}
	
	ActionList* ActionList::Clone() const
	{
		return new ActionList(*this);
	}
	
}
