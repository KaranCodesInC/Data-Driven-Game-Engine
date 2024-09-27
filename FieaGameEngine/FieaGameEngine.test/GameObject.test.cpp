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

namespace GameObjectTest
{
	TEST_CLASS(GameObjectTest)
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

        TEST_METHOD(AddChild)
        {
            // Create a parent GameObject
            GameObject Game;

            // Create child and grandchild GameObjects
            GameObject* child = new GameObject();
            GameObject* grandChild = new GameObject();

            // Add child to parent GameObject
            Game.addChild(child);
            size_t i = 0;
            // Assert that the child is found in the parent's children
            Assert::AreEqual(Game.FindContainedScope(child, i), Game.Find("GameObj"));

            // Add grandchild to child GameObject
            child->addChild(grandChild);
            // Assert that the grandchild is found in the child's children
            Assert::AreEqual(child->FindContainedScope(grandChild, i), child->Find("GameObj"));
            // Assert that the grandchild is not found in the parent's children
            Assert::AreNotEqual(child->FindContainedScope(grandChild, i), Game.Find("GameObj"));

            // Assert child and grandchild relationships
            Assert::AreEqual(Game.getChild(0), child);
            Assert::AreEqual(child->getChild(0), grandChild);
            // Assert that the grandchild is found in the nested structure
            Assert::AreEqual(Game.FindContainedScope(child, i)->GetScope(0)->FindContainedScope(grandChild, i), child->FindContainedScope(grandChild, i));

            // Clean up
            delete child;
            //delete grandChild;
        }

        TEST_METHOD(TwoWayBinding)
        {
            // Create a GameObject
            GameObject Game;

            // Assert initial state of Name attribute
            Assert::AreEqual(Game.Name, Game["Name"].GetString(0));
            // Modify and assert Name attribute
            Game.Name = std::string("hehehehe");
            Assert::AreEqual(Game.Name, Game["Name"].GetString(0));
            // Modify Name attribute using Assign and assert
            Game["Name"].Assign(std::string("pikachu"), 0);
            Assert::AreEqual(Game.Name, std::string("pikachu"));

            // Create another GameObject
            GameObject Game2;
            // Copy state from Game
            Game2 = Game;

            // Assert state of Name attribute in Game2
            Assert::AreEqual((Game2.Name), Game2["Name"].GetString(0));
            // Modify and assert Name attribute in Game2
            Assert::AreEqual((Game2.Name = "rawr"), Game2["Name"].GetString(0));
            // Modify Name attribute in Game2 using Assign and assert
            Game2["Name"].Assign(std::string("TooStrongPleaseNerf"), 0);
            Assert::AreEqual(Game2.Name, Game2["Name"].GetString(0));

            // Move state from Game2 to Game3
            GameObject Game3(std::move(Game2));

            // Assert state of Name attribute in Game3
            Assert::AreEqual((Game3.Name), Game3["Name"].GetString(0));
            // Modify and assert Name attribute in Game2 after move
            Assert::AreEqual((Game2.Name = "Yikes"), Game2["Name"].GetString(0));
            // Modify Name attribute in Game2 after move using Assign and assert
            Game2["Name"].Assign(std::string("Yeehaw"), 0);
            Assert::AreEqual(Game2.Name, Game2["Name"].GetString(0));
        }

        TEST_METHOD(ParseFromJSON)
        {
            // JSON input string
            std::string json_input = R"-(
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
        "GameObj":{
            "str_target_enemy": ["guard", "noble", "merchant"],
            "int_target_count": [2, 1, 1],
            "float_target_timer": [45.0, 60.0, 30.5],
            "vec4_newPosition": [1.0, 2.0, 3.0, 4.0],
            "mat4_newTransform":[[1.0, 0.0, 0.0, 0.0], [0.0, 1.0, 0.0, 0.0], [0.0, 0.0, 1.0, 0.0], [0.0, 0.0, 0.0, 1.0]]
        }
    }
}
)-";

            // Register GameObjectFactory with FactoryManager
            FactoryManager<Scope>::addFactory(std::make_unique<GameObjectFactory>());
            // Create ParseCoordinator with AttributedHandler wrapper
            AttributedHandler::Wrapper attributedWrapper;
            ParseCoordinator parser(attributedWrapper);
            // Assert that GetWrapper returns correct wrapper
            Assert::IsTrue(parser.GetWrapper().Equals(&attributedWrapper));

            // Create handlers for various data types
            std::shared_ptr<IntHandler> handlerInt = std::make_shared<IntHandler>();
            std::shared_ptr<FloatHandler> handlerFloat = std::make_shared<FloatHandler>();
            std::shared_ptr<StringHandler> handlerString = std::make_shared<StringHandler>();
            std::shared_ptr<VecHandler> handlerVec = std::make_shared<VecHandler>();
            std::shared_ptr<MatrixHandler> handlerMat = std::make_shared<MatrixHandler>();
            std::shared_ptr<AttributedHandler> handlerAtt = std::make_shared<AttributedHandler>();

            // Add handlers to parser
            parser.AddHandler(handlerAtt);
            parser.AddHandler(handlerInt);
            parser.AddHandler(handlerFloat);
            parser.AddHandler(handlerString);
            parser.AddHandler(handlerVec);
            parser.AddHandler(handlerMat);

            // Deserialize JSON input
            bool deserialized = parser.DeserializeObject(json_input);
            Assert::IsTrue(deserialized);

            // Retrieve GameObject
            GameObject* a = attributedWrapper.GetRoot()->Find("GameObj")->GetScope(0)->As<GameObject>();
            Assert::IsNotNull(a);

            // Assert values of attributes
            Assert::AreEqual(40, a->Find("int_DPS")->GetInt(0));
            Assert::AreEqual((float)4.1, a->Find("float_AttackSpeed")->GetFloat(0));
            Assert::AreEqual(27, a->Find("int_Age")->GetInt(0));

            // Assert nested objects
            auto obj1 = a->Find("obj_skills")->GetScope(0);
            Assert::AreEqual(std::string("shadow_blade"), obj1->Find("string_skill1")->GetString(0));
            Assert::AreEqual(std::string("Sneak"), obj1->Find("string_skill2")->GetString(0));
            Assert::AreEqual(std::string("shadow veil"), obj1->Find("string_skill3")->GetString(0));

            auto obj2 = obj1->Find("obj_shadowblade")->GetScope(0);
            Assert::AreEqual(std::string("KillToHeal"), obj2->Find("str_Enchantment")->GetString(0));
            Assert::AreEqual(100, obj2->Find("int_DMG")->GetInt(0));

            // Assert array values
            GameObject* GameObj2 = a->Find("GameObj")->GetScope(0)->As<GameObject>();
            Assert::AreEqual(2, GameObj2->Find("int_target_count")->GetInt(0));
            Assert::AreEqual(1, GameObj2->Find("int_target_count")->GetInt(1));
            Assert::AreEqual(1, GameObj2->Find("int_target_count")->GetInt(2));

            Assert::AreEqual((float)45.0, GameObj2->Find("float_target_timer")->GetFloat(0));
            Assert::AreEqual((float)60.0, GameObj2->Find("float_target_timer")->GetFloat(1));
            Assert::AreEqual((float)30.5, GameObj2->Find("float_target_timer")->GetFloat(2));

            Assert::AreEqual(std::string("guard"), GameObj2->Find("str_target_enemy")->GetString(0));
            Assert::AreEqual(std::string("noble"), GameObj2->Find("str_target_enemy")->GetString(1));
            Assert::AreEqual(std::string("merchant"), GameObj2->Find("str_target_enemy")->GetString(2));

            Assert::AreEqual(glm::vec4(1.0, 2.0, 3.0, 4.0), GameObj2->Find("vec4_newPosition")->GetVector(0));
            Assert::AreEqual(glm::mat4(1.0), GameObj2->Find("mat4_newTransform")->GetMatrix(0));

            // Assert attribute modifications
            Assert::AreEqual((GameObj2->Name), GameObj2->Find("Name")->GetString(0));
            Assert::AreEqual((GameObj2->Name = "rawr"), GameObj2->Find("Name")->GetString(0));
            GameObj2->Find("Name")->Assign(std::string("TooStrongPleaseNerf"), 0);
            Assert::AreEqual(GameObj2->Name, GameObj2->Find("Name")->GetString(0));

            // After Moving
            GameObject* GameObj3(std::move(GameObj2));

            // Assert state after move
            Assert::AreEqual((GameObj3->Name), GameObj3->Find("Name")->GetString(0));
            Assert::AreEqual((GameObj3->Name = "rawr"), GameObj3->Find("Name")->GetString(0));
            GameObj2->Find("Name")->Assign(std::string("TooStrongPleaseNerf"), 0);
            Assert::AreEqual(GameObj3->Name, GameObj3->Find("Name")->GetString(0));
        }
        TEST_METHOD(Update)
{
    // Create a parent GameObject
    GameObject Game;

    // Create child GameObjects
    GameObject* g1 = new GameObject();
    GameObject* g2 = new GameObject();
    GameObject* g3 = new GameObject();

    // Add children to parent
    Game.addChild(g1);
    Game.addChild(g2);
    Game.addChild(g3);

    // Call Update on parent GameObject
    Game.Update(7);

    // Clean up
    delete g1;
    delete g2;
    delete g3;
}

TEST_METHOD(RemoveChild)
{
    // Create a parent GameObject
    GameObject Game;

    // Create child GameObjects
    GameObject* g1 = new GameObject();
    GameObject* g2 = new GameObject();
    GameObject* g3 = new GameObject();

    // Add children to parent
    Game.addChild(g1);
    Game.addChild(g2);
    Game.addChild(g3);
    
    // Assert initial order of children
    Assert::AreEqual(Game["GameObj"].GetScope(0)->As<GameObject>(), g1);

    // Remove children and assert new order
    Game.removeChild(0);
    Assert::AreEqual(Game["GameObj"].GetScope(0)->As<GameObject>(), g2);
    Game.removeChild(0);
    Assert::AreEqual(Game["GameObj"].GetScope(0)->As<GameObject>(), g3);
    Game.removeChild(0);
   
    // Clean up
    delete g1;
    delete g2;
    delete g3;
}

	private:
		inline static _CrtMemState _startMemState;
	};
}
