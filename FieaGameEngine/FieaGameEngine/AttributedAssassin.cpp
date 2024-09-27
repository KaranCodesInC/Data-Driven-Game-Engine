#include "pch.h"
#include <vector>
#include "AttributedAssassin.h"
#include "TypeManager.h"

using namespace std::string_literals;

namespace Fiea::GameEngine {
	RTTI_DEFINITIONS(AttributedAssassin);

	std::vector<signature> AttributedAssassin::Signatures() {
		return std::vector<signature> {
			{ "Health"s, Datum::DatumType::Integer, 1, offsetof(AttributedAssassin, Health) },
			{ "MoveSpeed"s, Datum::DatumType::Float, 1, offsetof(AttributedAssassin, MoveSpeed) },
			{ "Boon"s, Datum::DatumType::String, 1, offsetof(AttributedAssassin, Boon) },
			{ "location"s, Datum::DatumType::Vector, 1, offsetof(AttributedAssassin, location) },
		};
	}
	AttributedAssassin::AttributedAssassin() :Attributed(AttributedAssassin::TypeIdClass()) {}

	AttributedAssassin::AttributedAssassin(size_t id) : Attributed(id)
	{

	}

	AttributedAssassin* AttributedAssassin::Clone() const {
		return new AttributedAssassin(*this);
	}
}