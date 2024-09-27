#include "pch.h"
#include "CppUnitTest.h"
#include "Temp.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;
using namespace std;

namespace FieaGameEnginetest
{
	TEST_CLASS(FieaGameEnginetest)
	{
	public:
		TEST_METHOD(BOOL)
		{
			Temp a;
			Assert::AreEqual(a.LifelineMain(), true);
		}
	};
}
