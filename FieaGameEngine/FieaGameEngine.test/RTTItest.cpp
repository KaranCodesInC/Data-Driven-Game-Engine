#pragma once
#include "pch.h"
#include "CppUnitTest.h"
#include "RTTI.h"
#include "Foo.h"
#include "Derived.h"
#include "Empty.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;
using Fiea::GameEngine::RTTI;
using Fiea::GameEngine::Foo;
using Fiea::GameEngine::test::Empty;
using Fiea::GameEngine::test::derived;

namespace RTTITest
{
	TEST_CLASS(RTTI_test)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(IsTest)
		{
			Foo foo;


			Empty empty;
			Assert::IsTrue(empty.Is(Empty::TypeIdClass()));
			Assert::IsFalse(empty.Is(Foo::TypeIdClass()));
		}

		TEST_METHOD(AsTest)
		{
			Foo foo;
			Assert::IsNotNull(foo.As<Foo>());
			Assert::IsNull(foo.As<Empty>());

			Empty empty;
			Assert::IsNotNull(empty.As<Empty>());
			Assert::IsNull(empty.As<Foo>());
		}
	private:
		inline static _CrtMemState _startMemState;
	};
}

