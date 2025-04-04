#pragma once

#include <cstdint>
#include "RTTI.h"

namespace Fiea::GameEngine
{
	class Foo : public RTTI
	{
		RTTI_DECLARATIONS(Foo, RTTI);
		//static const RTTI::IdType _typeId;

	public:
		explicit Foo(std::int32_t data = 0);
		Foo(const Foo& rhs);
		Foo(Foo&& rhs) noexcept;
		Foo& operator=(const Foo& rhs);
		Foo& operator=(Foo&& rhs) noexcept;
		virtual ~Foo();

		bool operator==(const Foo& rhs) const;
		bool operator!=(const Foo& rhs) const;

		std::int32_t Data() const;
		void SetData(std::int32_t data);

	private:
		std::int32_t* _data;
	};
}