#pragma once
#include "Action.h"

namespace Fiea
{
	namespace GameEngine
	{
		class ActionList : public Action
		{
			RTTI_DECLARATIONS(ActionList, Action);


		public:

			// Default constructor
			ActionList();

			// Copy constructor
			ActionList(const ActionList& other) = default;

			// Move constructor
			ActionList(ActionList&& other) noexcept = default;

			// Copy assignment operator
			ActionList& operator=(const ActionList& rhs) = default;

			// Move assignment operator
			ActionList& operator=(ActionList&& rhs) noexcept = default;

			// Destructor
			virtual ~ActionList() override = default;

			// Update function to be called every frame
			virtual void Update(const GameTime&) override;

			// Create and add an Action class from class name
			Action* CreateAction(const std::string& className);

			// Add an action to the current object
			void addAction(Action* action, const std::string& key = "");

			// Remove an action from the current object by key
			void removeAction(const std::string& key);

			// Get action by key
			Action* getAction(const string& classname);

			static std::vector<signature> Signatures();

			std::string ToString() const override { return Attributed::ToString(); }

			[[nodiscard]] ActionList* Clone() const override;

		protected:
			Scope* actions;

			ActionList(size_t id);
		};
	}
}