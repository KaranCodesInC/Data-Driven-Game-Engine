#include "pch.h"
#include <vector>
#include "AttributedFoo.h"
#include "TypeManager.h"

using namespace std::string_literals;

namespace Fiea::GameEngine {
	RTTI_DEFINITIONS(AttributedFoo);

	std::vector<signature> AttributedFoo::Signatures() {
		return std::vector<signature> {
			{ "externalInteger"s, Datum::DatumType::Integer, 1, offsetof(AttributedFoo, externalInteger) },
			{ "externalFloat"s, Datum::DatumType::Float, 1, offsetof(AttributedFoo, externalFloat) },
			{ "externalString"s, Datum::DatumType::String, 1, offsetof(AttributedFoo, externalString) },
			{ "externalVector"s, Datum::DatumType::Vector, 1, offsetof(AttributedFoo, externalVector) },
			{ "externalMatrix"s, Datum::DatumType::Matrix, 1, offsetof(AttributedFoo, externalMatrix) },
			{ "externalPointer"s, Datum::DatumType::Pointer, 1, offsetof(AttributedFoo, externalPointer) },
			{ "externalIntegerArray"s, Datum::DatumType::Integer, arraySize, offsetof(AttributedFoo, externalIntegerArray) },
			{ "externalFloatArray"s, Datum::DatumType::Float, arraySize, offsetof(AttributedFoo, externalFloatArray) },
			{ "externalStringArray"s, Datum::DatumType::String, arraySize, offsetof(AttributedFoo, externalStringArray) },
			{ "externalVectorArray"s, Datum::DatumType::Vector, arraySize, offsetof(AttributedFoo, externalVectorArray) },
			{ "externalMatrixArray"s, Datum::DatumType::Matrix, arraySize, offsetof(AttributedFoo, externalMatrixArray) },
			{ "externalPointerArray"s, Datum::DatumType::Pointer, arraySize, offsetof(AttributedFoo, externalPointerArray) }
		};
	}
	AttributedFoo::AttributedFoo() :Attributed(AttributedFoo::TypeIdClass()) {}

	AttributedFoo::AttributedFoo(size_t id) : Attributed(id)
	{
		
	}

	AttributedFoo* AttributedFoo::Clone() const {
		return new AttributedFoo(*this);
	}
}