#include "pch.h"
#include "CppUnitTest.h"
#include "AttributedFoo.h"
#include "AttributedBar.h"
#include "TypeManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;
//using namespace Fiea::GameEngine::test;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Scope>(const Scope& v)
	{
		RETURN_WIDE_STRING(0);
	}
	template<>
	inline std::wstring ToString<Scope>(const Scope* v)
	{
		RETURN_WIDE_STRING(v);
	}
	template<>
	inline std::wstring ToString<Scope>(Scope* v)
	{
		RETURN_WIDE_STRING(v);
	}
	template<>
	inline std::wstring ToString<Fiea::GameEngine::Datum::DatumType>(const Fiea::GameEngine::Datum::DatumType& type)
	{
		switch (type)
		{
		case Fiea::GameEngine::Datum::DatumType::Integer:
			return L"Integer";
		case Fiea::GameEngine::Datum::DatumType::Float:
			return L"Float";
		case Fiea::GameEngine::Datum::DatumType::Vector:
			return L"Vector";
		case Fiea::GameEngine::Datum::DatumType::Matrix:
			return L"Matrix";
		case Fiea::GameEngine::Datum::DatumType::String:
			return L"String";
		case Fiea::GameEngine::Datum::DatumType::Table:
			return L"Table";
		case Fiea::GameEngine::Datum::DatumType::Pointer:
			return L"Pointer";
		default:
			return L"Unknown";
		}
	}
	template<>
	inline std::wstring ToString<Datum>(const Datum& v)
	{
		RETURN_WIDE_STRING("Datum[" << v.Size() << "]");
	}
	template<>
	inline std::wstring ToString<Datum>(const Datum* v)
	{
		RETURN_WIDE_STRING(v);
	}
	template<>
	inline std::wstring ToString<Datum>(Datum* v)
	{
		RETURN_WIDE_STRING(v);
	}
	template<>
	inline std::wstring ToString<Fiea::GameEngine::Attributed>(const Fiea::GameEngine::Attributed& attributed)
	{
		return L"Attributed Object";
	}
}

namespace AttributedTests
{
	TEST_CLASS(AttributedTest)
	{
	public:

		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif		
			TypeManager::Instance();
			TypeManager::instance->AddSignature(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
			TypeManager::instance->AddSignature(AttributedBar::TypeIdClass(), AttributedBar::CombinedSigs());
		}
		TEST_METHOD_CLEANUP(Cleanup)
		{
			TypeManager::instance->cleanup();
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
		// Test constructor
		TEST_METHOD(Constructor)
		{
			AttributedBar AttBar;
			AttributedFoo a;

			Assert::IsTrue(a.IsPrescribedAttribute("externalInteger"));
			Assert::AreEqual(100, a["externalInteger"].GetInt(0));

			Assert::AreEqual(a.externalInteger = 5, a["externalInteger"].GetInt(0));
			Assert::AreEqual(a.externalInteger, 5);
			a["externalInteger"].Assign(10, 0);
			Assert::AreEqual(a.externalInteger, a["externalInteger"].GetInt(0));
			a.AppendAuxiliaryAttribute("Smash");
			AttBar.AppendAuxiliaryAttribute("Damage");

			a["Smash"].PushBack(10);
			AttBar["Damage"].PushBack(50);

			Assert::IsTrue(a.IsAuxiliaryAttribute("Smash"));

			// Check if the 'this' attribute contains the correct pointer value
			Datum* thisAttribute = a.Find("Self");
			Datum* thatAttribute = AttBar.Find("Self");
			////a.externalInteger = 6;

			//AttributedFoo
			Assert::IsNotNull(thisAttribute);
			Assert::AreEqual(Datum::DatumType::Pointer, thisAttribute->Type());
			Assert::AreEqual((size_t)1, thisAttribute->Size());

			//AttributedBar
			Assert::IsNotNull(thatAttribute);
			Assert::AreEqual(Datum::DatumType::Pointer, thatAttribute->Type());
			Assert::AreEqual((size_t)1, thatAttribute->Size());
		}

		// Test copy constructor
		TEST_METHOD(CopyConstructorTest)
		{
			AttributedFoo original;
			original.AppendAuxiliaryAttribute("PhysicalPower").PushBack(50);
			original.AppendAuxiliaryAttribute("Movement").PushBack(40);
			original.AppendAuxiliaryAttribute("DPS").PushBack(6);

			// Copy the attributes from original to copy
			AttributedFoo copy(original);

			// Check if attributes were copied correctly
			Assert::AreEqual(copy["PhysicalPower"].GetInt(0), 50);
			Assert::AreEqual(copy["Movement"].GetInt(0), 40);
			Assert::AreEqual(copy["DPS"].GetInt(0), 6);
			Assert::IsNull(copy.Find("Mana"));
			Assert::IsNull(copy.Find("Health"));

			//AttributedBar
			AttributedBar bar;
			bar.AppendAuxiliaryAttribute("Yolo").PushBack(5);
			bar.AppendAuxiliaryAttribute("MeleeDmg").PushBack(10);
			bar.AppendAuxiliaryAttribute("Charm").PushBack(50);

			AttributedBar copyBar(bar);
			Assert::AreEqual(copyBar["Yolo"].GetInt(0), 5);
			Assert::AreEqual(copyBar["MeleeDmg"].GetInt(0), 10);
			Assert::AreEqual(copyBar["Charm"].GetInt(0), 50);
		}

		// Test copy assignment operator
		TEST_METHOD(CopyAssignment)
		{
			// Create an original Attributed object
			AttributedFoo original;
			original.AppendAuxiliaryAttribute("PhysicalPower").PushBack(50);
			original.AppendAuxiliaryAttribute("Movement").PushBack(40);
			original.AppendAuxiliaryAttribute("DPS").PushBack(6);

			// Create a copy Attributed object
			AttributedFoo copy;
			copy.AppendAuxiliaryAttribute("CritChance").PushBack(5);
			copy.AppendAuxiliaryAttribute("Penetration").PushBack(40);
			copy.AppendAuxiliaryAttribute("Mana").PushBack(200);
			copy.AppendAuxiliaryAttribute("Health").PushBack(300);

			// Copy the attributes from original to copy
			copy = original;

			// Check if attributes were copied correctly
			Assert::AreEqual(copy["PhysicalPower"].GetInt(0), 50);
			Assert::AreEqual(copy["Movement"].GetInt(0), 40);
			Assert::AreEqual(copy["DPS"].GetInt(0), 6);

			//AttributedBar

			AttributedBar bar;
			bar.AppendAuxiliaryAttribute("Yolo").PushBack(5);
			bar.AppendAuxiliaryAttribute("MeleeDmg").PushBack(10);
			bar.AppendAuxiliaryAttribute("Charm").PushBack(50);

			AttributedBar copyBar;
			copy.AppendAuxiliaryAttribute("CritChance").PushBack(5);
			copy.AppendAuxiliaryAttribute("Penetration").PushBack(40);
			copy.AppendAuxiliaryAttribute("Mana").PushBack(200);
			copy.AppendAuxiliaryAttribute("Health").PushBack(300);


			copyBar = bar;

			Assert::AreEqual(copyBar["Yolo"].GetInt(0), 5);
			Assert::AreEqual(copyBar["MeleeDmg"].GetInt(0), 10);
			Assert::AreEqual(copyBar["Charm"].GetInt(0), 50);
			Assert::AreNotEqual(copyBar["Health"].GetInt(0), 300);
		}

		// Test move constructor
		TEST_METHOD(MoveConstructor)
		{
			// Create an original Attributed object
			AttributedFoo original;
			original.AppendAuxiliaryAttribute("PhysicalPower");
			original.AppendAuxiliaryAttribute("Movement").PushBack(40);
			original.AppendAuxiliaryAttribute("DPS").PushBack(6);

			Assert::IsTrue(original.Find("PhysicalPower") != nullptr);
			// Create a new Attributed object using move constructor
			AttributedFoo newAtt(std::move(original));

			Assert::IsTrue(newAtt.IsPrescribedAttribute("externalInteger"));
			Assert::IsTrue(newAtt.IsAuxiliaryAttribute("PhysicalPower"));

			// Check if attributes were moved correctly
			//Assert::IsTrue(original.Find("PhysicalPower") == nullptr);
			//Assert::AreEqual(newAtt["PhysicalPower"].GetInt(0), 50);
			Assert::IsTrue(newAtt.Find("Movement") != nullptr);
			Assert::AreEqual(newAtt["Movement"].GetInt(0), 40);
			Assert::IsTrue(newAtt.Find("DPS") != nullptr);
			Assert::AreEqual(newAtt["DPS"].GetInt(0), 6);
		}

		// Test IsAttribute method
		TEST_METHOD(IsAttribute)
		{
			// Create an Attributed object
			AttributedFoo a;

			// Add auxiliary attributes to the object
			a.AppendAuxiliaryAttribute("Stealth").PushBack(25);
			a.AppendAuxiliaryAttribute("PowerPoints").PushBack(30);

			// Check if IsAttribute method correctly identifies attributes
			Assert::IsTrue(a.IsAttribute("Stealth"));
			Assert::IsTrue(a.IsAttribute("PowerPoints"));
			Assert::AreEqual(a["Stealth"].GetInt(0), 25);
		}

		// Test IsAuxiliaryAttribute method
		TEST_METHOD(IsAuxiliaryAttribute)
		{
			// Create an Attributed object
			AttributedFoo a;

			// Add auxiliary attributes to the object
			a.AppendAuxiliaryAttribute("StunDuration");
			a.AppendAuxiliaryAttribute("SleepDuration");

			Assert::AreEqual(a.Find("StunDuration")->Type(), Datum::DatumType::Unknown);
			Assert::IsTrue(a.Find("StunDuration") != nullptr);

			a.Find("StunDuration")->PushBack(5);
			Assert::AreEqual(a.Find("StunDuration")->GetInt(0), 5);
			// Check if IsAuxiliaryAttribute method correctly identifies auxiliary attributes
			Assert::IsTrue(a.IsAuxiliaryAttribute("StunDuration"));
			Assert::IsTrue(a.IsAuxiliaryAttribute("SleepDuration"));

			AttributedBar b;

			Assert::IsFalse(b.IsPrescribedAttribute("StunDuration"));
			Assert::IsFalse(b.IsAuxiliaryAttribute("StunDuration"));

			b.AppendAuxiliaryAttribute("StunDuration");
			b.Find("StunDuration")->PushBack(5);
			Assert::AreEqual(a.Find("StunDuration")->GetInt(0), 5);
		}

		// Test move assignment operator
		TEST_METHOD(MoveAssignment)
		{
			// Create an original Attributed object
			AttributedFoo original;
			original.AppendAuxiliaryAttribute("PhysicalPower");
			original.AppendAuxiliaryAttribute("Movement").PushBack(40);
			original.AppendAuxiliaryAttribute("DPS").PushBack(6);

			original["PhysicalPower"].PushBack(50);
			Assert::AreEqual(original["PhysicalPower"].GetInt(0), 50);
			Assert::IsTrue(original.Find("PhysicalPower") != nullptr);

			AttributedFoo newAtt;
		    newAtt = std::move(original);

			Assert::IsTrue(newAtt.IsPrescribedAttribute("externalInteger"));

			// Check if attributes were moved correctly
			//Assert::IsTrue(newAtt.Find("PhysicalPower") != nullptr);
			Assert::AreEqual(newAtt["PhysicalPower"].GetInt(0), 50);
			Assert::IsTrue(newAtt.Find("Movement") != nullptr);
			Assert::AreEqual(newAtt["Movement"].GetInt(0), 40);
			Assert::IsTrue(newAtt.Find("DPS") != nullptr);
			Assert::AreEqual(newAtt["DPS"].GetInt(0), 6);

			AttributedBar bar;
			bar.AppendAuxiliaryAttribute("MagicalPower").PushBack(100);
			bar.AppendAuxiliaryAttribute("CCDuration").PushBack(3);

			AttributedFoo newBar;
			newBar = std::move(bar);

			Assert::IsTrue(newBar.Find("MagicalPower") != nullptr);
			Assert::AreEqual(newBar["MagicalPower"].GetInt(0), 100);
			Assert::IsTrue(newBar.Find("CCDuration") != nullptr);
			Assert::AreEqual(newBar["CCDuration"].GetInt(0), 3);
		}

		TEST_METHOD(TwoWayBinding)
		{
			AttributedBar Bar;
			//AttributedBar copyBar;

			Assert::AreEqual(Bar.externalInteger, Bar["externalInteger"].GetInt(0));
			Bar.externalInteger = 50;
			Assert::AreEqual(Bar["externalInteger"].GetInt(0), Bar.externalInteger);
			Bar.externalInteger = 80;
			Assert::AreEqual(Bar["externalInteger"].GetInt(0), Bar.externalInteger);
			Bar["externalInteger"].Assign(5, 0);
			Assert::AreEqual(Bar["externalInteger"].GetInt(0), Bar.externalInteger);

			AttributedFoo copyBar(Bar);

			Assert::AreEqual(copyBar.externalInteger, Bar.externalInteger);
			Assert::AreEqual(copyBar["externalInteger"].GetInt(0), Bar["externalInteger"].GetInt(0));
			copyBar.externalInteger = 5;
			Assert::AreEqual(copyBar["externalInteger"].GetInt(0), copyBar.externalInteger);
			copyBar["externalInteger"].Assign(10, 0);
			Assert::AreEqual(copyBar["externalInteger"].GetInt(0), copyBar.externalInteger);
		}

	private:
		inline static _CrtMemState _startMemState;

	};
}