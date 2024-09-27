#pragma once
#include "ActionList.h"

namespace Fiea
{
	namespace GameEngine
	{
		class ActionListWhile final : public ActionList
		{
			RTTI_DECLARATIONS(ActionListWhile, ActionList);

		public:
			// Default constructor
			ActionListWhile();

			//ActionListWhile(size_t id);

			// Copy constructor
			ActionListWhile(const ActionListWhile& other) = default;

			// Move constructor
			ActionListWhile(ActionListWhile&& other) noexcept = default;

			// Copy assignment operator
			ActionListWhile& operator=(const ActionListWhile& rhs) = default;

			// Move assignment operator
			ActionListWhile& operator=(ActionListWhile&& rhs) noexcept = default;

			// Destructor
			~ActionListWhile() override = default;

			// Update function to be called every frame
			void Update(const GameTime&) override;

			// Set the preamble of the while loop
			void SetPreamble(Action* action);

			// Set the increment of the while loop
			void SetIncrement(Action* action);

			// Get the prescribed attributes of the ActionListWhile class
			static std::vector<signature> Signatures();

			void SetCondition(int n);

			std::string ToString() const override { return Attributed::ToString(); }

			[[nodiscard]] ActionListWhile* Clone() const;

		private:
			int condition = 0;
			Action* preamble = nullptr;
			Action* increment = nullptr;

			bool isConditionSet = false;
		};
	}
}
