#pragma once
#include "Datum.h"

namespace Fiea::GameEngine
{
	class signature
	{
	public:
		string name;
		Datum::DatumType _type;
		size_t size = 0;
		size_t Offset = 0;
	};
}