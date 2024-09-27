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

namespace FactoryTest
{
	TEST_CLASS(FactoryTest)
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

		TEST_METHOD(MakeFactory)
		{
			FactoryManager<Scope>::addFactory(std::make_unique<AttributedFooFactory>());
			
			Factory<Scope>& factscope = FactoryManager<Scope>::Find("AttributedFoo");

			AttributedFoo* foo = factscope.create()->As<AttributedFoo>();

			AttributedFoo* newfoo = FactoryManager<Scope>::create("AttributedFoo")->As<AttributedFoo>();

			delete foo;
			delete newfoo;
		}
		TEST_METHOD(ParseFromJSON)
		{
			FactoryManager<Scope>::addFactory(std::make_unique<AttributedAssassinFactory>());

			std::string json_input = R"-(
{
	"Assassin": {
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
			//std::shared_ptr<ScopeHandler> handlerScope = std::make_shared<ScopeHandler>();
			std::shared_ptr<FloatHandler> handlerFloat = std::make_shared<FloatHandler>();
			std::shared_ptr<StringHandler> handlerString = std::make_shared<StringHandler>();
			std::shared_ptr<VecHandler> handlerVec = std::make_shared<VecHandler>();
			std::shared_ptr<MatrixHandler> handlerMat = std::make_shared<MatrixHandler>();
			std::shared_ptr<AttributedHandler> handlerAtt = std::make_shared<AttributedHandler>();

			//parser.AddHandler(handlerScope);
			parser.AddHandler(handlerAtt);
			parser.AddHandler(handlerInt);
			parser.AddHandler(handlerFloat);
			parser.AddHandler(handlerString);
			parser.AddHandler(handlerVec);
			parser.AddHandler(handlerMat);
			

			bool deserialized = parser.DeserializeObject(json_input);
			Assert::IsTrue(deserialized);

			AttributedAssassin* a = attributedWrapper.GetRoot()->Find("Assassin")->GetScope(0)->As<AttributedAssassin>();
			Assert::IsNotNull(a);
			
			Assert::AreEqual(40, a->Find("int_DPS")->GetInt(0));
			Assert::AreEqual((float)4.1, a->Find("float_AttackSpeed")->GetFloat(0));
			Assert::AreEqual(27, a->Find("int_Age")->GetInt(0));
			
			//int arrays
			Assert::AreEqual(2, a->Find("int_target_count")->GetInt(0));
			Assert::AreEqual(1, a->Find("int_target_count")->GetInt(1));
			Assert::AreEqual(1, a->Find("int_target_count")->GetInt(2));

			//nested object 1
			auto obj1 = a->Find("obj_skills")->GetScope(0);
			Assert::AreEqual(std::string("shadow_blade"), obj1->Find("string_skill1")->GetString(0));
			Assert::AreEqual(std::string("Sneak"), obj1->Find("string_skill2")->GetString(0));
			Assert::AreEqual(std::string("shadow veil"), obj1->Find("string_skill3")->GetString(0));
			
			//nested object 2
			auto obj2 = obj1->Find("obj_shadowblade")->GetScope(0);
			Assert::AreEqual(std::string("KillToHeal"), obj2->Find("str_Enchantment")->GetString(0));
			Assert::AreEqual(100, obj2->Find("int_DMG")->GetInt(0));

			//float arrays
			Assert::AreEqual((float)45.0, a->Find("float_target_timer")->GetFloat(0));
			Assert::AreEqual((float)60.0, a->Find("float_target_timer")->GetFloat(1));
			Assert::AreEqual((float)30.5, a->Find("float_target_timer")->GetFloat(2));

			//string arrays
			Assert::AreEqual(std::string("guard"), a->Find("str_target_enemy")->GetString(0));
			Assert::AreEqual(std::string("noble"), a->Find("str_target_enemy")->GetString(1));
			Assert::AreEqual(std::string("merchant"), a->Find("str_target_enemy")->GetString(2));

			//vec4 and mat4
			Assert::AreEqual(glm::vec4(1.0, 2.0, 3.0, 4.0), a->Find("vec4_newPosition")->GetVector(0));
			Assert::AreEqual(glm::mat4(1.0), a->Find("mat4_newTransform")->GetMatrix(0));
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}
