#pragma once
#pragma once

#include <stdexcept>
#include <algorithm>
#include "Attributed.h"
#include<glm/glm.hpp>
#include "RTTI.h"

namespace Fiea::GameEngine
{
	class AttributedAssassin final : public Attributed
	{
		RTTI_DECLARATIONS(AttributedAssassin, Attributed);

	public:
		static const std::size_t arraySize = 5;

		AttributedAssassin();
		AttributedAssassin(size_t id);
		AttributedAssassin(const AttributedAssassin& rhs) = default;
		AttributedAssassin(AttributedAssassin&& rhs) noexcept = default;
		AttributedAssassin& operator=(const AttributedAssassin& rhs) = default;
		AttributedAssassin& operator=(AttributedAssassin&& rhs) noexcept = default;
		~AttributedAssassin() = default;
		[[nodiscard]] AttributedAssassin* Clone() const override;

		int Health = 100;
		float MoveSpeed = 200;
		std::string Boon = "AgileHunt";
		glm::vec4 location = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
		RTTI* externalPointer = nullptr;

		//bool Equals(const RTTI* rhs) const override;
		std::string ToString() const override { return Attributed::ToString(); }

		static std::vector<signature> Signatures();
	};
}
