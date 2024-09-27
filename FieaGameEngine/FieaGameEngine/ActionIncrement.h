#pragma once
#include <vector>
#include "Action.h"

namespace Fiea
{
	namespace GameEngine
	{
		class ActionIncrement final : public Action
		{
			RTTI_DECLARATIONS(ActionIncrement, Action);

		public:

			// Default constructor
			ActionIncrement();

			// Copy constructor
			ActionIncrement(const ActionIncrement& other) = default;

			// Move constructor
			ActionIncrement(ActionIncrement&& other) noexcept = default;

			// Copy assignment operator
			ActionIncrement& operator=(const ActionIncrement& rhs) = default;

			// Move assignment operator
			ActionIncrement& operator=(ActionIncrement&& rhs) noexcept = default;

			// Destructor
			virtual ~ActionIncrement() override = default;

			// Update function to be called every frame
			virtual void Update(const GameTime&) override;

			// Get the prescribed attributes of the ActionIncrement class
			static std::vector<signature> Signatures();

			[[nodiscard]] ActionIncrement* Clone() const override;

			std::string ToString() const override { return Attributed::ToString(); }

			std::string key = "ActionInc";
			int i;
			float value;
		};
	}
}