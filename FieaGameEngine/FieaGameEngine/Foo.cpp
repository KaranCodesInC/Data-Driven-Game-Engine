#include "pch.h"
#include "Foo.h"

using namespace std;

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(Foo);
	//static const RTTI::IdType _typeId;

	Foo::Foo(int32_t data) :
		_data(new int32_t(data))
	{
	}

	Foo::Foo(const Foo& rhs) :
		_data(new int32_t(*rhs._data))
	{
	}

	Foo::Foo(Foo&& rhs) noexcept :
		_data(rhs._data)
	{
		rhs._data = nullptr;
	}

	Foo& Foo::operator=(const Foo& rhs)
	{
		if (this != &rhs)
		{
			*_data = *rhs._data;
		}

		return *this;
	}

	Foo& Foo::operator=(Foo&& rhs) noexcept
	{
		if (this != &rhs)
		{
			delete _data;
			_data = rhs._data;
			rhs._data = nullptr;
		}

		return *this;
	}

	Foo::~Foo()
	{
		delete _data;
	}

	bool Foo::operator==(const Foo& rhs) const
	{
		return *_data == *rhs._data;
	}

	bool Foo::operator!=(const Foo& rhs) const
	{
		return !operator==(rhs);
	}

	int32_t Foo::Data() const
	{
		return *_data;
	}

	void Foo::SetData(int32_t data)
	{
		*_data = data;
	}
}