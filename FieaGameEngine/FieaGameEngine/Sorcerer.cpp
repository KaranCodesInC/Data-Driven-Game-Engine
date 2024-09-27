#include"pch.h"
#include "Sorcerer.h"

using namespace std::string_literals;

namespace Fiea
{
	namespace GameEngine
	{
		RTTI_DEFINITIONS(Sorcerer);


		Sorcerer::Sorcerer() : GameObject(Sorcerer::TypeIdClass()) {}


		std::vector<signature> Sorcerer::Signatures()
		{
			return std::vector<signature>
			{
				{ "this"s, Datum::DatumType::Pointer, 1, 0 },
				{ "GameTag"s, Datum::DatumType::String, 1, offsetof(Sorcerer, name) }
			};
		}

		std::string Sorcerer::ToString() const
		{
			return "Sorcerer"s;
		}
	}
}