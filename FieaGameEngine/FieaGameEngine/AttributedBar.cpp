#include "pch.h"
#include "AttributedBar.h"
#include "TypeManager.h"

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(AttributedBar);

	AttributedBar::AttributedBar() : AttributedFoo(AttributedBar::TypeIdClass()) 
	{
		
	}

	AttributedBar* AttributedBar::Clone() const {
		return new AttributedBar(*this);
	}

	std::vector<signature> AttributedBar::Signatures() {
		return std::vector<signature> {
			{ "Health"s, Datum::DatumType::Integer, 1, offsetof(AttributedBar, Health) },
			{ "Armour"s, Datum::DatumType::Float, 1, offsetof(AttributedBar, Armour) },
		};
	}

	std::vector<signature> AttributedBar::CombinedSigs() {
		std::vector<signature> v1 = AttributedFoo::Signatures();
		std::vector<signature> v2 = AttributedBar::Signatures();

		for (auto& it : v1)
		{
			v2.push_back(it);
		}
		return v2;
	}
}
