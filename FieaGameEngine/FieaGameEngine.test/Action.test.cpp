#include "pch.h"
#include "CppUnitTest.h"
#include "scope.h"
#include "Attributed.h"
#include "AttributedFoo.h"
#include "AttributedBar.h"
#include "AttributedAssassin.h"
#include "TypeManager.h"
#include "Factory.h"
#include "FactoryManager.h"
#include "TypeManager.h"
#include "GameObject.h"
#include "ActionIncrement.h"
#include "ActionList.h"
#include "ActionListWhile.h"
#include "Sorcerer.h"
#include "IParseHandler.h"
#include "ParseCoordinator.h"
#include "IntHandler.h"
#include "FloatHandler.h"
#include "StringHandler.h"
#include "AttributedHandler.h"
#include "VecHandler.h"
#include "MatrixHandler.h"
#include "ScopeHandler.h"
#include <sstream>
#include <string.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;
using namespace std::string_literals;
using namespace std;

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
	inline std::wstring ToString<GameObject>(const GameObject* t)
	{
		RETURN_WIDE_STRING(L"GameObject Ptr");
	}

	template<>
	inline std::wstring ToString<GameObject>(GameObject* t)
	{
		RETURN_WIDE_STRING(L"GameObject Ptr");
	}

	template<>
	inline std::wstring ToString(const glm::vec4& vec)
	{
		std::wstringstream ss;
		ss << L"(" << vec.x << L", " << vec.y << L", " << vec.z << L", " << vec.w << L")";
		return ss.str();
	}

	template<>
	inline std::wstring ToString<glm::mat4>(const glm::mat4& matrix)
	{
		// Convert the glm::mat4 to a string representation
		std::wstringstream ss;
		ss << L"[" << matrix[0][0] << L", " << matrix[0][1] << L", " << matrix[0][2] << L", " << matrix[0][3] << L"]\n";
		ss << L"[" << matrix[1][0] << L", " << matrix[1][1] << L", " << matrix[1][2] << L", " << matrix[1][3] << L"]\n";
		ss << L"[" << matrix[2][0] << L", " << matrix[2][1] << L", " << matrix[2][2] << L", " << matrix[2][3] << L"]\n";
		ss << L"[" << matrix[3][0] << L", " << matrix[3][1] << L", " << matrix[3][2] << L", " << matrix[3][3] << L"]\n";
		return ss.str();
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

namespace ActionTest
{
	TEST_CLASS(ActionTest)
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
			TypeManager::instance->AddSignature(AttributedAssassin::TypeIdClass(), AttributedFoo::Signatures());
			TypeManager::instance->AddSignature(AttributedBar::TypeIdClass(), AttributedBar::CombinedSigs());
			TypeManager::instance->AddSignature(GameObject::TypeIdClass(), GameObject::Signatures());
			TypeManager::instance->AddSignature(Action::TypeIdClass(), Action::Signatures());
			TypeManager::instance->AddSignature(ActionIncrement::TypeIdClass(), ActionIncrement::Signatures());
			TypeManager::instance->AddSignature(ActionList::TypeIdClass(), ActionList::Signatures());
			TypeManager::instance->AddSignature(ActionListWhile::TypeIdClass(), ActionList::Signatures());
			TypeManager::instance->AddSignature(Sorcerer::TypeIdClass(), Sorcerer::Signatures());
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			TypeManager::instance->cleanup();
			FactoryManager<Scope>::clear();

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

		TEST_METHOD(ActionFromJSON)
		{
			FactoryManager<Scope>::addFactory(std::make_unique<GameObjectFactory>());
			FactoryManager<Scope>::addFactory(std::make_unique<ActionIncrementFactory>());
			FactoryManager<Scope>::addFactory(std::make_unique<ActionListFactory>());
			FactoryManager<Scope>::addFactory(std::make_unique<ActionListWhileFactory>());

			std::string input = R"-(
{
	"GameObj": {
		"int_DPS": 40,
		"float_AttackSpeed": 4.1,
		"int_Age": 27,
		"obj_skills": {
			"string_skill1": "shadow_blade",
			"string_skill2": "Sneak",
			"string_skill3": "shadow veil",
			"obj_shadowblade": {
				"int_DMG": 100,
				"str_Enchantment": "KillToHeal"
			}
		},
		"str_target_enemy": ["guard", "noble", "merchant"],
		"int_target_count": [2, 1, 1],
		"float_target_timer": [45.0, 60.0, 30.5],
		"vec4_newPosition": [1.0, 2.0, 3.0, 4.0],
		"mat4_newTransform":[[1.0, 0.0, 0.0, 0.0], [0.0, 1.0, 0.0, 0.0], [0.0, 0.0, 1.0, 0.0], [0.0, 0.0, 0.0, 1.0]]
	}
}
)-";
			AttributedHandler::Wrapper attributedWrapper;
			ParseCoordinator parser(attributedWrapper);
			Assert::IsTrue(parser.GetWrapper().Equals(&attributedWrapper));

			std::shared_ptr<IntHandler> handlerInt = std::make_shared<IntHandler>();
			std::shared_ptr<FloatHandler> handlerFloat = std::make_shared<FloatHandler>();
			std::shared_ptr<StringHandler> handlerString = std::make_shared<StringHandler>();
			std::shared_ptr<VecHandler> handlerVec = std::make_shared<VecHandler>();
			std::shared_ptr<MatrixHandler> handlerMat = std::make_shared<MatrixHandler>();
			std::shared_ptr<AttributedHandler> handlerAtt = std::make_shared<AttributedHandler>();

			parser.AddHandler(handlerAtt);
			parser.AddHandler(handlerInt);
			parser.AddHandler(handlerFloat);
			parser.AddHandler(handlerString);
			parser.AddHandler(handlerVec);
			parser.AddHandler(handlerMat);
			
			bool deserialized = parser.DeserializeObject(input);
			Assert::IsTrue(deserialized);

			GameObject* s = attributedWrapper.GetRoot()->Find("GameObj")->GetScope(0)->As<GameObject>();
			Assert::IsNotNull(s);

			s->CreateAction("ActionIncrement");
			s->CreateAction("ActionList");
			s->CreateAction("ActionListWhile");

			ActionIncrement* ActInc =s->getAction("ActionIncrement")->As<ActionIncrement>();
			ActionList* ActList = s->getAction("ActionList")->As<ActionList>();
			ActionListWhile* ActListWhile = s->getAction("ActionListWhile")->As<ActionListWhile>();

			ActInc->key = "b";
			ActInc->i = 20;

			ActList->CreateAction("ActionIncrement");
			ActionIncrement* ActInc_2 = ActList->getAction("ActionIncrement")->As<ActionIncrement>();

			ActInc_2->key = "PowerPoints";
			ActInc_2->i = 100;

			ActionIncrement* firstAction = new ActionIncrement();
			firstAction->key = "ManaPoints";
			firstAction->i = 200;

			ActListWhile->SetPreamble(firstAction);
			ActListWhile->SetIncrement(firstAction);
			ActListWhile->SetCondition(10);
			ActListWhile->CreateAction("ActionIncrement");
			ActionIncrement* whileAction = ActListWhile->getAction("ActionIncrement")->As<ActionIncrement>();

			whileAction->key = "FireBallBurn";
			whileAction->value = 50.0;

			//s->Update(5);

			Assert::AreEqual(100, s->Find("b")->GetInt(0));
			Assert::AreEqual(500, s->Find("PowerPoints")->GetInt(0));
			Assert::AreEqual(500, s->Find("ManaPoints")->GetInt(0));
		}

		TEST_METHOD(addChild)
		{
			
		}
	private:
		inline static _CrtMemState _startMemState;
	};
}
