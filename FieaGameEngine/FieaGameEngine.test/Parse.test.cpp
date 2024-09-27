#include "pch.h"
#include "CppUnitTest.h"
#include "ParseCoordinator.h"
#include "IParseHandler.h"
#include "TestParseHandler.h"
#include "scope.h"
#include "Attributed.h"
#include "TestIntHandler.h"
#include "ScopeHandler.h"
#include "IntHandler.h"
#include <sstream>
#include <string>
#include "FloatHandler.h"
#include "StringHandler.h"
#include "VecHandler.h"
#include "MatrixHandler.h"

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

namespace ParseTests
{
	TEST_CLASS(ParseTest)
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

		// This method tests the deserialization of nested JSON objects.
		// It creates a JSON string representing a nested object, parses it using ParseCoordinator,
		// and asserts that the parsing was successful and all the expected members were parsed correctly.
		TEST_METHOD(NestedObject) {
			// Define the nested JSON string
			std::string nested = R"( 
    {
        "int":1,
        "obj":{
            "int":1,
            "obj":{
                "int":1,
                "str":"abc"
            },
            "str":"abc"
        },
        "obj2":{
            "int":1,
            "obj":{
                "int":1,
                "str":"abc"
            },
            "str":"abc"
        },
        "str":"abc"
    }
)";
			// Create a wrapper for the parse handler
			TestParseHandler::Wrapper wrapper;
			// Create a ParseCoordinator instance with the wrapper
			ParseCoordinator parser(wrapper);
			// Create a TestParseHandler instance and add it to the parser
			std::shared_ptr<TestParseHandler> handler = std::make_shared<TestParseHandler>();
			parser.AddHandler(handler);
			// Deserialize the JSON string
			bool deserialized = parser.DeserializeObject(nested);
			//handler->Cleanup(); 
			Assert::IsTrue(deserialized);					// Assert that deserialization was successful
			Assert::IsTrue(handler->initializeCalled);		// Assert that the handler's initialize function was called
			//Assert::IsTrue(handler.cleanupCalled);		
			Assert::AreEqual(14, int(handler->startCount)); // Assert that the expected number of start and end calls were made
			Assert::AreEqual(14, int(handler->endCount));
			//Assert::AreEqual(3, int(wrapper.maxDepth));
		}

		TEST_METHOD(SimpleInt)
		{
			std::string integer = R"(
    {
        "int1":1,
		"int2":2
	}
)";

			TestIntHandler::Wrapper wrapper;
			ParseCoordinator parser(wrapper);
			std::shared_ptr<TestIntHandler> handler = std::make_shared<TestIntHandler>();
			parser.AddHandler(handler);
			bool deserialized = parser.DeserializeObject(integer);
			//handler->Cleanup();
			Assert::IsTrue(deserialized);
			Assert::IsTrue(handler->initializeCalled);
			Assert::AreEqual(2, int(handler->startCount));
			Assert::AreEqual(2, int(handler->endCount));
			//Assert::AreEqual(2, int(wrapper.maxDepth));
		}
		TEST_METHOD(ScopeHandling)
		{
			std::string json_input = R"(
	{
		"int1": 42,
		"float1": 3.14,
		"string1": "MozambiqueHere",
		"vector": [0.5, 0.2, 0.8, 1.0],
		"matrix": [
			[1.0, 0.2, 0.3, 0.0],
			[0.4, 1.0, 0.5, 0.0],
			[0.6, 0.7, 1.0, 0.0],
			[0.8, 0.9, 0.2, 1.0]
	],
    "obj1": {
        "int2": 1,
        "obj2": {
            "int3": 1,
            "string2": "Caustic"
        },
        "string3": "Mirage Voyage"
    },
    "obj3": {
        "int4": 1,
        "obj4": {
            "int5": 1,
            "string4": "i got a feeling"
        },
        "string5": "Tonight's gon be a good night"
    },
    "string6": "good good night"

	}
)";

			Scope scope;
			ScopeHandler::Wrapper wrapper(&scope);
			ParseCoordinator parser(wrapper);
			Assert::IsTrue(parser.GetWrapper().Equals(&wrapper));
			Assert::AreEqual(&scope, wrapper.GetRoot());

			std::shared_ptr<IntHandler> handlerInt = std::make_shared<IntHandler>();
			std::shared_ptr<ScopeHandler> handlerScope = std::make_shared<ScopeHandler>();
			std::shared_ptr<FloatHandler> handlerFloat = std::make_shared<FloatHandler>();
			std::shared_ptr<StringHandler> handlerString = std::make_shared<StringHandler>();
			std::shared_ptr<VecHandler> handlerVec = std::make_shared<VecHandler>();
			std::shared_ptr<MatrixHandler> handlerMat = std::make_shared<MatrixHandler>();

			parser.AddHandler(handlerScope);
			parser.AddHandler(handlerInt);
			parser.AddHandler(handlerFloat);
			parser.AddHandler(handlerVec);
			parser.AddHandler(handlerString);
			parser.AddHandler(handlerMat);
			
			bool deserialized = parser.DeserializeObject(json_input);

			Assert::IsTrue(deserialized);

			Assert::AreEqual(42, scope.Find("int1")->GetInt(0));
			Assert::AreEqual((float)3.14, scope.Find("float1")->GetFloat(0));
			Assert::AreEqual(std::string("MozambiqueHere"), scope.Find("string1")->GetString(0));
			//Assert::AreEqual(glm::vec4(0.5, 0.2, 0.8, 1.0), scope.Find("vector")->GetVector(0));
			glm::mat4 expectedMat4 = {
		{1.0f, 0.2f, 0.3f, 0.0f},
		{0.4f, 1.0f, 0.5f, 0.0f},
		{0.6f, 0.7f, 1.0f, 0.0f},
		{0.8f, 0.9f, 0.2f, 1.0f}
			};
			//Assert::AreEqual(expectedMat4, scope.Find("matrix")->GetMatrix(0));

			//check contents of the first nested object
			auto obj1 = scope.Find("obj1")->GetScope(0);
			Assert::AreEqual(1, obj1->Find("int2")->GetInt(0));
			Assert::AreEqual(std::string("Mirage Voyage"), obj1->Find("string3")->GetString(0));
			Assert::AreEqual(std::string("good good night"), scope.Find("string6")->GetString(0)); //this is actualy an assert for root
			
			//check contents of the object in obj1
			auto obj2 = obj1->Find("obj2")->GetScope(0);
			Assert::AreEqual(1, obj2->Find("int3")->GetInt(0));
			Assert::AreEqual(std::string("Caustic"), obj2->Find("string2")->GetString(0));

			//pop out of obj1 and 2 back into the root
			auto obj3 = scope.Find("obj3")->GetScope(0);
			Assert::AreEqual(1, obj3->Find("int4")->GetInt(0));
			Assert::AreEqual(std::string("Tonight's gon be a good night"), obj3->Find("string5")->GetString(0));

			//go into obj3 and check contents
			auto obj4 = obj3->Find("obj4")->GetScope(0);

			Assert::AreEqual(1, obj4->Find("int5")->GetInt(0));
			Assert::AreEqual(std::string("i got a feeling"), obj4->Find("string4")->GetString(0));


		}
		TEST_METHOD(vectorHandling)
		{

			std::string json_input = R"(
	{
		"vector": [0.5, 0.2, 0.8, 1.0]
	}
)";
			Scope scope;
			ScopeHandler::Wrapper wrapper(&scope);
			ParseCoordinator parser(wrapper);
			Assert::IsTrue(parser.GetWrapper().Equals(&wrapper));
			Assert::AreEqual(&scope, wrapper.GetRoot());

			std::shared_ptr<VecHandler> handlerVec = std::make_shared<VecHandler>();
			parser.AddHandler(handlerVec);
			bool deserialized = parser.DeserializeObject(json_input);
			Assert::IsTrue(deserialized);
			Assert::AreEqual(glm::vec4(0.5, 0.2, 0.8, 1.0), scope.Find("vector")->GetVector(0));
		}
		TEST_METHOD(MatrixHandling)
		{
			std::string json_input = R"(
	{
		"mat4_1": [
        [1.0, 0.2, 0.3, 0.0],
        [0.4, 1.0, 0.5, 0.0],
        [0.6, 0.7, 1.0, 0.0],
        [0.8, 0.9, 0.2, 1.0]
    ]
	}
)";
			Scope scope;
			ScopeHandler::Wrapper wrapper(&scope);
			ParseCoordinator parser(wrapper);
			Assert::IsTrue(parser.GetWrapper().Equals(&wrapper));
			Assert::AreEqual(&scope, wrapper.GetRoot());

			std::shared_ptr<MatrixHandler> handlerMat = std::make_shared<MatrixHandler>();
			parser.AddHandler(handlerMat);
			bool deserialized = parser.DeserializeObject(json_input);
			glm::mat4 expectedMat4 = {
		{1.0f, 0.2f, 0.3f, 0.0f},
		{0.4f, 1.0f, 0.5f, 0.0f},
		{0.6f, 0.7f, 1.0f, 0.0f},
		{0.8f, 0.9f, 0.2f, 1.0f}
			};
			Assert::AreEqual(expectedMat4, scope.Find("mat4_1")->GetMatrix(0));
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}