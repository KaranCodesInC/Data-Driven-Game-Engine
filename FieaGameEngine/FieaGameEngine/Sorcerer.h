#pragma once
#include <vector>
#include "GameObject.h"

namespace Fiea
{
	namespace GameEngine
	{
		class Sorcerer final : public GameObject
		{
			RTTI_DECLARATIONS(Sorcerer, GameObject);

		public:

			// Default constructor
			Sorcerer();

			// Copy constructor
			Sorcerer(const Sorcerer& other) = default;

			// Move constructor
			Sorcerer(Sorcerer&& other) noexcept = default;

			// Copy assignment operator
			Sorcerer& operator=(const Sorcerer& rhs) = default;

			// Move assignment operator
			Sorcerer& operator=(Sorcerer&& rhs) noexcept = default;

			// Destructor
			virtual ~Sorcerer() override = default;

			static std::vector<signature> Signatures();

			//RTTI Overrides
			virtual std::string ToString() const override;

			std::string name = "Sorcerer";
		};
	}
}
