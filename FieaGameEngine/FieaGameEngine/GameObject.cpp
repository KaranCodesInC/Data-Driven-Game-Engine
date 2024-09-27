#include "pch.h"
#include "GameObject.h"
#include "FactoryManager.h"


using namespace std::string_literals;

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(GameObject);

	/**
	 * @brief Default constructor for GameObject.
	 *        Initializes the GameObject with a "GameObj" attribute of type Table.
	 */
	GameObject::GameObject() : Attributed(TypeIdClass())
	{
		this->Append("GameObj").SetType(Datum::DatumType::Table);
		actions = new Scope();
		Adopt(std::move(actions), "Actions");
	}

	/**
	 * @brief Parameterized constructor for GameObject.
	 * @param id The ID to assign to the GameObject.
	 *        Initializes the GameObject with a "GameObj" attribute of type Table.
	 */
	GameObject::GameObject(size_t id) : Attributed(id)
	{
		this->Append("GameObj").SetType(Datum::DatumType::Table);
		actions = new Scope();
		Adopt(std::move(actions), "Actions");
	}

	/**
	 * @brief Returns the signatures for GameObject attributes.
	 * @return A vector of signatures defining GameObject's attributes.
	 */
	std::vector<signature> GameObject::Signatures()
	{
		return std::vector<signature>
		{
			{ "self"s, Datum::DatumType::Pointer, 1, 0},
			{ "Name"s, Datum::DatumType::String, 1, offsetof(GameObject, Name) },
			{ "Position"s,Datum::DatumType::Vector,1,offsetof(GameObject,Transform.position) },
			{ "Rotation"s,Datum::DatumType::Vector,1,offsetof(GameObject,Transform.rotation) },
			{ "Scale"s,Datum::DatumType::Vector,1,offsetof(GameObject,Transform.scale) },
			{ "children"s,Datum::DatumType::Table,0,0 }
		};
	}

	/**
	 * @brief Adds a child GameObject to this GameObject.
	 * @param child Pointer to the child GameObject to add.
	 */
	void GameObject::addChild(GameObject* child)
	{
		if (child == nullptr)
		{
			return;
		}

		for (size_t i = 0; i < this->Find("GameObj")->Size(); ++i)
		{
			if (this->Find("GameObj")->GetScope(i) == child)
			{
				return;
			}
		}

		this->Adopt(std::move(child), "GameObj");

	}

	/**
	 * @brief Removes a child GameObject at the specified index.
	 * @param index The index of the child GameObject to remove.
	 */
	void GameObject::removeChild(size_t index)
	{
		if (index >= this->Find("GameObj")->Size())
		{
			return;
		}

		this->Find("GameObj")->RemoveAt(index);
		if (this->Find("GameObj")->Size() == 0)
		{
			this->Find("GameObj")->~Datum();
		}

	}
	/**
	 * @brief Creates a deep copy of the GameObject.
	 * @return A pointer to the newly created copy of the GameObject.
	 */
	GameObject* GameObject::Clone() const
	{
		return new GameObject(*this);
	}

	/**
	 * @brief Creates a deep copy of the GameObject.
	 * @return A pointer to the newly created copy of the GameObject.
	 */
	void GameObject::Update(const GameTime& time)
	{
		for(size_t i = 0; i < this->Find("GameObj")->Size(); ++i)
		{
			this->Find("GameObj")->GetScope(i)->As<GameObject>()->Update(time);
		}
		for (size_t i = 0; i < actions->ScopeSize(); i++)
		{
			(*actions)[i].GetScope(0)->As<Action>()->Update(time);
		}
		updateCall = true;
	}

	/**
	 * @brief Gets the child GameObject at the specified index.
	 * @param index The index of the child GameObject to retrieve.
	 * @return A pointer to the child GameObject at the specified index.
	 * @throw std::runtime_error if the child GameObject is not found at the specified index.
	 */
	GameObject* GameObject::getChild(size_t index)
	{
		if (index >= this->Find("GameObj")->Size())
		{
			throw std::runtime_error("GameObj Child not found");
		}
		return this->Find("GameObj")->GetScope(index)->As<GameObject>();
	}

		/**
	 * @brief Returns the Datum containing actions for this GameObject.
	 * @return A pointer to the Datum containing actions.
	 */
	Datum* GameObject::Actions()
	{
		return this->Find("Actions");
	}

	/**
 * @brief Creates a new instance of an Action derived class and adds it to this GameObject.
 * @param className The name of the Action derived class to create.
 * @return A pointer to the Datum containing the created Action.
 */
	Action* GameObject::CreateAction(const std::string& className, const std::string& instanceName)
	{
		Action* action = FactoryManager<Scope>::create(className)->As<Action>();
		addAction(action, instanceName);
		return action;
	}

	Action* GameObject::getAction(const string& classname)
	{
		Datum* D = this->Find(classname);
		if (D == nullptr || D->Size() == 0)
		{
			throw std::invalid_argument("Action Not Found");
		}

		return D->GetScope(0)->As<Action>();
	}

	/**
 * @brief Recursively retrieves the root GameObject of a given GameObject.
 * @param GameObj The GameObject for which to find the root.
 * @return A pointer to the root GameObject, or nullptr if not found.
 */
	GameObject* GameObject::GetRoot(GameObject* GameObj)
	{
		GameObject* G = this->GetParent()->As<GameObject>();
		if (G != nullptr)
		{
			GetRoot(G);
		}
		else
		{
			return G;
		}

		return nullptr;
	}


	void GameObject::addAction(Action* action, const std::string& key)
	{
		if (action == nullptr)
		{
			throw std::invalid_argument("Cannot add a nullptr as an Action");
		}

		size_t index = 0;
		if (actions->FindContainedScope(action, index) != nullptr || actions->Find(action->name) != nullptr)
		{
			throw std::invalid_argument("Action already exists in this GameObject");
		}

		// Transfer ownership of the Action to this GameObject
		actions->Adopt(std::move(action), action->name);
		action->setParentGameObject(this);
	}


}

