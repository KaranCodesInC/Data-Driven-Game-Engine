#pragma once

#include "AttributedFoo.h"
#include "RTTI.h"

namespace Fiea::GameEngine
{
	class AttributedBar : public AttributedFoo
	{
		RTTI_DECLARATIONS(AttributedBar, AttributedFoo);

	public:

		AttributedBar();
		AttributedBar(const AttributedBar& rhs) = default;
		AttributedBar(AttributedBar&& rhs) noexcept = default;
		AttributedBar& operator=(const AttributedBar& rhs) = default;
		AttributedBar& operator=(AttributedBar&& rhs) noexcept = default;
		~AttributedBar() = default;
		[[nodiscard]] AttributedBar* Clone() const override;

		int Health = 100;
		float Armour = 100;

		std::string ToString() const override { return Attributed::ToString(); }

		static std::vector<signature> Signatures();
		static std::vector<signature> CombinedSigs();
	};
}
