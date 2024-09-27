#include "pch.h"
#include "CppUnitTest.h"
#include "Scope.h"
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>
#include "Datum.h"



using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;
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

}

namespace ScopeTest
{
	TEST_CLASS(ScopeTest)
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
		
		TEST_METHOD(ConstructorTest)
		{
	
			std::size_t initialCapacity = 10;  // Set the initial capacity for testing


			Scope scope(initialCapacity); //constructing scope with initialcapacity = 10

			
			Assert::AreEqual(scope.ScopeSize(), (size_t)0); //Size is 0 since no elements
			Assert::IsNull(scope.GetParent(), L"Parent should be nullptr"); //It has no parent
		}

		// Test method for the Adopt function, which adds a child scope to a parent scope with a specified key
		TEST_METHOD(AdoptTest)
		{
			// Arrange
			Scope parentScope; // Create a parent scope
			Scope childScope;  // Create a child scope
			const std::string key = "Child"; // Specify the key for adoption

			// Act
			parentScope.Adopt(childScope, key); // Adopt the child scope by the parent with the specified key

			// Assert
			Assert::IsTrue(childScope.GetParent() == &parentScope); // Check if the child scope's parent is the parent scope
			Assert::IsTrue(parentScope.Find(key) != nullptr); // Check if the parent scope contains the adopted child scope with the specified key
		}


		TEST_METHOD(AdoptAlreadyAdoptedTest)
		{
			// Arrange
			Scope parentScope1; // Create the first parent scope
			Scope parentScope2; // Create the second parent scope
			Scope childScope;   // Create the child scope
			const std::string key = "Child"; // Define a key for adoption

			// Act
			parentScope1.Adopt(childScope, key); // Adopt the child scope by the first parent
			parentScope2.Adopt(childScope, key); // Attempt to adopt the child scope by the second parent

			// Assert
			Assert::IsTrue(childScope.GetParent() == &parentScope2); // Ensure the child scope is now adopted by the second parent
			Assert::IsTrue(parentScope2.Find(key) != nullptr);       // Ensure the child scope can be found in the second parent's hierarchy
		}

		TEST_METHOD(AdoptSelfTest)
		{
			// Arrange
			Scope scope;             // Create a scope
			const std::string key = "Self"; // Define a key for adoption

			// Act
			scope.Adopt(scope, key); // Attempt to adopt the scope by itself

			// Assert
			Assert::IsNull(scope.GetParent());          // Ensure the scope remains without a parent
			Assert::IsNull(scope.Find(key));            // Ensure the scope cannot be found in its own hierarchy using the given key
		}


		TEST_METHOD(Append)
		{
			// Create a Scope
			Scope scope;

			// Append a Datum with a new name
			Datum appendedDatum = scope.Append("NewDatum");

			// Check if the size of the Scope is 1
			Assert::AreEqual((size_t)1, scope.ScopeSize());

			// Append another Datum with the same name
			Datum & existingDatum = scope.Append("NewDatum");

			// Check if the size of the Scope remains 1 (no new entry added)
			Assert::AreEqual<size_t>(1, scope.ScopeSize());

			Assert::AreEqual(appendedDatum, existingDatum);

			Datum& datum1 = scope.Append("test1");

			// Check if the Datum was added correctly
			Assert::IsTrue(scope.Find("test1") != nullptr);
			Assert::IsTrue(&datum1 == scope.Find("test1"));

			// Append a Datum with the same name
			Datum& datum2 = scope.Append("test1");

			// Check if the same Datum is returned
			Assert::IsTrue(&datum1 == &datum2);

			// Append another Datum with a different name
			Datum& datum3 = scope.Append("test2");

			// Check if the Datum was added correctly
			Assert::IsTrue(scope.Find("test2") != nullptr);
			Assert::IsTrue(&datum3 == scope.Find("test2"));
		}

		TEST_METHOD(AppendScope)
		{
			// Create a Scope
			Scope scope;

			// Append a new Scope with a given name
			Scope& appendedScope = scope.AppendScope("ChildScope");

			// Verify that the parent of the appended scope is the original scope
			Assert::AreEqual(&scope, appendedScope.GetParent());

			// Verify that the appended scope exists in the original scope's entries
			Assert::IsTrue(scope.Find("ChildScope") != nullptr);
		}
		TEST_METHOD(IsAncestorOf_Test)
		{
			// Create a hierarchy of scopes
			Scope root;
			Scope child1;
			Scope child2;
			Scope grandchild;

			root.Adopt(child1, "Child1");
			root.Adopt(child2, "Child2");
			child1.Adopt(grandchild, "Grandchild");

			// Test cases
			Assert::IsTrue(root.IsAncestorOf(child1));       // Root is ancestor of child1
			Assert::IsTrue(root.IsAncestorOf(child2));       // Root is ancestor of child2
			Assert::IsTrue(root.IsAncestorOf(grandchild));   // Root is ancestor of grandchild
			Assert::IsFalse(child1.IsAncestorOf(root));      // Child1 is not ancestor of root
			Assert::IsFalse(child2.IsAncestorOf(child1));    // Child2 is not ancestor of child1
		}

		TEST_METHOD(IsDescendantOf_Test)
		{
			// Create a hierarchy of scopes
			Scope root;
			Scope child1;
			Scope child2;
			Scope grandchild;

			root.Adopt(child1, "Child1");
			root.Adopt(child2, "Child2");
			child1.Adopt(grandchild, "Grandchild");

			// Test cases
			Assert::IsTrue(child1.IsDescendantOf(root));        // Child1 is descendant of root
			Assert::IsTrue(child2.IsDescendantOf(root));        // Child2 is descendant of root
			Assert::IsTrue(grandchild.IsDescendantOf(root));    // Grandchild is descendant of root
			Assert::IsFalse(root.IsDescendantOf(child1));       // Root is not descendant of child1
			Assert::IsFalse(child1.IsDescendantOf(child2));
		}

		TEST_METHOD(OperatorEquals_SameScope_ReturnsTrue)
		{
			Scope scope1;
			Scope scope2;

			bool result = (scope1 == scope2);

			Assert::IsTrue(result);
		}

		TEST_METHOD(OperatorEquals_DifferentScopes_ReturnsFalse)
		{
			
			Scope scope1;
			Scope scope2;

			// Add some entries to the scopes
			scope1.Append("key1");
			scope2.Append("key2");

			bool result = (scope1 == scope2);

			Assert::IsFalse(result);
		}

		TEST_METHOD(OperatorEquals_EqualScopesWithDifferentEntries_ReturnsFalse)
		{
			
			Scope scope1;
			Scope scope2;

			// Add some entries to the scopes
			scope1.Append("key1");
			scope2.Append("key1");

			// Modify the entries in one of the scopes
			(*scope1.Find("key1")).PushBack(10);
			(*scope2.Find("key1")).PushBack(20);

			bool result = (scope1 == scope2);

			Assert::IsFalse(result);
		}

		TEST_METHOD(OperatorNotEquals_SameScope_ReturnsFalse)
		{
			
			Scope scope1;
			Scope scope2;

			
			bool result = (scope1 != scope2);

			
			Assert::IsFalse(result);
		}

		TEST_METHOD(OperatorNotEquals_DifferentScopes_ReturnsTrue)
		{
			
			Scope scope1;
			Scope scope2;

			// Add some entries to the scopes
			scope1.Append("key1");
			scope2.Append("key2");

			
			bool result = (scope1 != scope2);

			
			Assert::IsTrue(result);
		}

		TEST_METHOD(OperatorNotEquals_EqualScopesWithDifferentEntries_ReturnsTrue)
		{
			
			Scope scope1;
			Scope scope2;

			// Add some entries to the scopes
			scope1.Append("key1");
			scope2.Append("key1");

			// Modify the entries in one of the scopes
			(*scope1.Find("key1")).PushBack(10);
			(*scope2.Find("key1")).PushBack(20);

			
			bool result = (scope1 != scope2);

			
			Assert::IsTrue(result);
		}

		TEST_METHOD(OperatorIndex_ValidIndex_ReturnsDatumReference)
		{
			
			Scope scope;
			scope.Append("key1").PushBack(10);
			scope.Append("key2").PushBack(20);
			scope.Append("key3").PushBack(30);

			
			Datum& result = scope[1]; // Get the Datum at index 1

			
			Assert::AreEqual(20, result.GetInt(0)); // Check if the value matches the expected value
		}

		TEST_METHOD(OperatorIndex_InvalidIndex_ThrowsException)
		{
			
			Scope scope;
			scope.Append("key1").PushBack(10);
			scope.Append("key2").PushBack(20);
			scope.Append("key3").PushBack(30);

			//Assert
			Assert::ExpectException<std::out_of_range>([&scope]
				{
					Datum& result = scope[3]; // Accessing index out of bounds should throw an exception
				});
		}

		TEST_METHOD(FindContainedScope)
		{
			
			Scope parent;
			Scope child1;
			Scope child2;
			parent.AppendScope("child1").Adopt(child1, "child1");
			parent.AppendScope("child2").Adopt(child2, "child2");

			
			std::size_t index = 0;
			Datum* foundDatum = parent.FindContainedScope(&child1, index);

			
			Assert::IsNotNull(foundDatum); // Check if a Datum containing the child scope is found
			Assert::AreEqual((size_t)0, index); // Check if the index is correct
		}

		TEST_METHOD(Clear)
		{
			Scope scope;
			Scope& nestedScope1 = scope.AppendScope("nested1");
			Scope& nestedScope2 = scope.AppendScope("nested2");
			nestedScope1.Append("key1");
			nestedScope2.Append("key2");

			scope.Clear();

			Assert::AreEqual(size_t(0), scope.ScopeSize()); // Check if all entries are removed
		}

		TEST_METHOD(Find)
		{
			Scope scope;
			scope.Append("key1");
			scope.Append("key2");

			Datum* foundDatum = scope.Find("key1");

			Assert::IsNotNull(foundDatum);
			Assert::AreEqual(Datum::DatumType::Unknown, foundDatum->Type());
			Assert::AreEqual((size_t)0, foundDatum->Size());
		}

		TEST_METHOD(Search_InCurrentScope_EntryExists_ReturnsPointerToDatum)
		{
			
			Scope scope;
			Datum& datum = scope.Append("key");
			int value = 10;
			datum.PushBack(value);

			
			Scope* foundScope = nullptr;
			Datum* foundDatum = scope.Search("key", &foundScope);

			
			Assert::IsNotNull(foundDatum);
			Assert::IsNull(foundScope); // Since the entry is found in the current scope
			Assert::AreEqual(value, foundDatum->GetInt(0));
		}
		
		TEST_METHOD(CopyConstructor)
		{
			
			Scope originalScope;
			Scope& nestedScope1 = originalScope.AppendScope("nested1");
			nestedScope1.Append("key1").PushBack(10);
			nestedScope1.Append("key2").PushBack(20);
			Scope& nestedScope2 = originalScope.AppendScope("nested2");
			nestedScope2.Append("key3").PushBack(30);
			nestedScope2.Append("key4").PushBack(40);

			
			Scope copiedScope(originalScope);

			
			Assert::AreEqual((size_t)2, copiedScope.ScopeSize()); // Ensure size matches original
		}

		TEST_METHOD(CopyAssignmentOperator)
		{
			{
				Scope emptyScope;
				Scope assignedScope;


				assignedScope = emptyScope;


				Assert::AreEqual((size_t)0, assignedScope.ScopeSize());
			}
			{
				Scope originalScope;
				Scope& nestedScope1 = originalScope.AppendScope("nested1");
				nestedScope1.Append("key1").PushBack(10);
				nestedScope1.Append("key2").PushBack(20);
				Scope& nestedScope2 = originalScope.AppendScope("nested2");
				nestedScope2.Append("key3").PushBack(30);
				nestedScope2.Append("key4").PushBack(40);
				Scope assignedScope;


				assignedScope = originalScope;


				Assert::AreEqual((size_t)2, assignedScope.ScopeSize());
				//Assert::AreEqual()
			}
		}
		TEST_METHOD(CopyToScope){ 
			Datum D;

			std::unordered_map<std::string,Datum> Entries;

			Entries["brub"] = D;
			std::vector<Datum*> Vec;
			Vec.push_back(&Entries["brub"]);
		}
		TEST_METHOD(DatumCopy) {
			Datum D;
			Datum E = D;
		}

		TEST_METHOD(MoveConstructor)
		{
			Scope A, B;

			A.Append("key1").PushBack(1);
			A.Append("key2").PushBack(2);

			B = std::move(A);

			Assert::AreEqual(B["key1"].GetInt(0), 1);
			Assert::AreEqual(B["key2"].GetInt(0), 2);

			Assert::IsTrue(A.Find("key1") == nullptr);
		}

		TEST_METHOD(RemoveAtforScope)
		{
			Scope S;
			Scope S2;

			Datum D;

			D.PushBack(&S);
			D.PushBack(&S2);

			D.RemoveAt(1);

			Assert::AreEqual(*D.GetScope(0), S2);

		}


	private:
		inline static _CrtMemState _startMemState;

	};
}