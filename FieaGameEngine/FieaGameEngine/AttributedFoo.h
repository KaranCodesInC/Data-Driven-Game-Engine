#pragma once

#include <stdexcept>
#include <algorithm>
#include "Attributed.h"
#include "Foo.h"
#include<glm/glm.hpp>
#include "RTTI.h"

namespace Fiea::GameEngine
{
	class AttributedFoo : public Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, Attributed);

	public:
		static const std::size_t arraySize = 5;

		AttributedFoo();
		AttributedFoo(size_t id);
		AttributedFoo(const AttributedFoo& rhs) = default;
		AttributedFoo(AttributedFoo&& rhs) noexcept = default;
		AttributedFoo& operator=(const AttributedFoo& rhs) = default;
		AttributedFoo& operator=(AttributedFoo&& rhs) noexcept = default;
		~AttributedFoo() = default;
		[[nodiscard]] AttributedFoo* Clone() const override;

		int externalInteger = 100;
		float externalFloat = 200;
		std::string externalString = "String";
		glm::vec4 externalVector = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
		glm::mat4 externalMatrix = glm::mat4(1.0f);
		RTTI* externalPointer = nullptr;
		int externalIntegerArray[arraySize];
		float externalFloatArray[arraySize];
		std::string externalStringArray[arraySize];
		glm::vec4 externalVectorArray[arraySize];
		glm::mat4 externalMatrixArray[arraySize];
		RTTI* externalPointerArray[arraySize];

		//bool Equals(const RTTI* rhs) const override;
		std::string ToString() const override { return Attributed::ToString(); }



		static std::vector<signature> Signatures();
	};
}
