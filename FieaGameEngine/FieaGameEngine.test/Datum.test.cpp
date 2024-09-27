#include "pch.h"
#include "CppUnitTest.h"
#include "Datum.h"
#include "RTTI.h"
#include <glm/glm.hpp>
#include <iostream>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
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
	inline std::wstring ToString<Datum::DatumType>(const Datum::DatumType& q)
	{

		switch (q)
		{
		case Fiea::GameEngine::Datum::DatumType::Unknown:
			return L"Unknown";
		case Fiea::GameEngine::Datum::DatumType::Integer:
			return L"Int";
		case Fiea::GameEngine::Datum::DatumType::Float:
			return L"Float";
		case Fiea::GameEngine::Datum::DatumType::String:
			return L"String";
		case Fiea::GameEngine::Datum::DatumType::Pointer:
			return L"Pointer";//more cases as needed for enum values
		default:
			return L"Invalid DatumType"; 

		}
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
	inline std::wstring ToString<glm::vec4>(const glm::vec4& vector)
	{
		// Convert the glm::vec4 to a string representation
		std::wstringstream ss;
		ss << L"[" << vector.x << L", " << vector.y << L", " << vector.z << L", " << vector.w << L"]";
		return ss.str();
	}

}

namespace DatumTest
{
	TEST_CLASS(DatumTest)
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

		TEST_METHOD(Constructor)
		{
			Datum D; //Calling Default constructor

			Assert::AreEqual(D.Type(), Datum::DatumType::Unknown);//default construction creates empty datum hence datatype should be unknown.
			Assert::AreEqual(D.Size(), (size_t)0);
			Assert::AreEqual(D.Capacity(), (size_t)0);
		}
		TEST_METHOD(PushBack)
		{
			// Creating Datum instances for testing
			Datum D, D2, D3, D4, D5;

			// Pushing an integer into Datum
			D.PushBack(2);
			Assert::AreEqual(D.Type(), Datum::DatumType::Integer); // Datum type should be Integer
			Assert::AreEqual(D.Size(), (size_t)1); // Size should be 1 after pushing one integer
			Assert::AreEqual(D.Capacity(), (size_t)1); // Capacity should be 1 after pushing one integer

			// Expect an exception when trying to push a float into an Integer Datum
			Assert::ExpectException<std::invalid_argument>([&D] { D.PushBack(3.0f); });
			// Ensure that type, size, and capacity remain unchanged
			Assert::AreEqual(D.Size(), (size_t)1);
			Assert::AreEqual(D.Capacity(), (size_t)1);


			// Pushing another integer
			D.PushBack(3);
			Assert::AreEqual(D.Size(), (size_t)2); // Size should increase to 2 after pushing another integer
			Assert::AreEqual(D.Capacity(), (size_t)2); // Capacity should remain the same
			Assert::AreEqual(D.GetInt(0), 2);

			// Pushing another integer
			D.PushBack(4);
			Assert::AreEqual(D.Size(), (size_t)3); // Size should increase to 3 after pushing another integer
			Assert::AreEqual(D.Capacity(), (size_t)4); // Capacity should double to 4 after pushing another integer

			// Pushing a float into Datum
			D2.PushBack(1.0f);
			Assert::AreEqual(D2.Type(), Datum::DatumType::Float); // Datum type should be Float
			Assert::AreEqual(D2.Size(), (size_t)1); // Size should be 1 after pushing one float
			Assert::AreEqual(D2.Capacity(), (size_t)1); // Capacity should be 1 after pushing one float

			// Expect an exception when trying to push an integer into a Float Datum
			Assert::ExpectException<std::invalid_argument>([&D2] { D2.PushBack(2); });
			// Ensure that type, size, and capacity remain unchanged
			Assert::AreEqual(D2.Size(), (size_t)1);
			Assert::AreEqual(D2.Capacity(), (size_t)1);

			// Pushing another float
			D2.PushBack(2.0f);
			Assert::AreEqual(D2.Size(), (size_t)2); // Size should increase to 2 after pushing another float
			Assert::AreEqual(D2.Capacity(), (size_t)2); // Capacity should remain the same

			// Pushing another float
			D2.PushBack(3.0f);
			Assert::AreEqual(D2.Size(), (size_t)3); // Size should increase to 3 after pushing another float
			Assert::AreEqual(D2.Capacity(), (size_t)4); // Capacity should double to 4 after pushing another float

			// Pushing glm::vec4 values into Datum
			glm::vec4 vec1(1.0f, 2.0f, 3.0f, 4.0f);
			glm::vec4 vec2(5.0f, 6.0f, 7.0f, 8.0f);
			D4.PushBack(vec1);
			D4.PushBack(vec2);
			Assert::AreEqual(D4.Type(), Datum::DatumType::Vector); // Datum type should be Vector
			Assert::AreEqual(D4.Size(), (size_t)2); // Size should be 2 after pushing two glm::vec4 values
			Assert::AreEqual(D4.Capacity(), (size_t)2); // Capacity should be 2
			Assert::ExpectException<std::invalid_argument>([&D4]
				{D4.PushBack(5);
				});
			// Pushing glm::mat4 values into Datum
			glm::mat4 mat1(1.0f); // Identity Matrix
			glm::mat4 mat2(2.0f);
			D5.PushBack(mat1);
			D5.PushBack(mat2);
			Assert::AreEqual(D5.Type(), Datum::DatumType::Matrix); // Datum type should be Matrix
			Assert::AreEqual(D5.Size(), (size_t)2); // Size should be 2 after pushing two glm::mat4 values
			Assert::AreEqual(D5.Capacity(), (size_t)2); // Capacity should be 2

			glm::mat4 mat3(3.0f);
		
			// Pushing a string into Datum
			std::string h = "hello";
			D3.PushBack(h);
			Assert::AreEqual(D3.Type(), Datum::DatumType::String); // Datum type should be String
			Assert::AreEqual(D3.Size(), (size_t)1); // Size should be 1 after pushing one string
			Assert::AreEqual(D3.Capacity(), (size_t)1); // Capacity should be 1 after pushing one string

			std::string b = "GetGud";
			D3.PushBack(b);
			Assert::AreEqual(D3.Size(), (size_t)2);
			Assert::AreEqual(D3.Capacity(), (size_t)2);
			Assert::AreEqual(D3.GetString(0), h);
		}
			
		
		TEST_METHOD(CopyConstructor)
		{
			{
				Datum D;
				D.PushBack(1);
				D.PushBack(2);
				D.PushBack(3);
				D.PushBack(4);

				// Creating a copy using the copy constructor
				Datum Dcoy(D);

				// Check if the copied Datum is equal to the original
				Assert::AreEqual(D, Dcoy);
				Assert::AreEqual(D.GetInt(1), Dcoy.GetInt(1));
				Assert::AreEqual(D.GetInt(2), Dcoy.GetInt(2));
				Assert::AreEqual(D.GetInt(3), Dcoy.GetInt(3));
			}
			// Copy constructor for Float type
			{
				Datum D;
				D.PushBack(1.0f);
				D.PushBack(2.0f);
				D.PushBack(3.0f);
				D.PushBack(4.0f);

				// Creating a copy using the copy constructor
				Datum Dcoy(D);

				// Check if the copied Datum is equal to the original
				Assert::AreEqual(D, Dcoy);
			}
			// Copy constructor for Vector type
			{
				Datum D;
				glm::vec4 vec1(1.0f, 2.0f, 3.0f, 4.0f);
				glm::vec4 vec2(5.0f, 6.0f, 7.0f, 8.0f);
				D.PushBack(vec1);
				D.PushBack(vec2);

				// Creating a copy using the copy constructor
				Datum Dcoy(D);

				// Check if the copied Datum is equal to the original
				Assert::AreEqual(D, Dcoy);
			}
			// Copy constructor for Matrix type
			{
				Datum D;
				glm::mat4 mat1(1.0f);
				glm::mat4 mat2(2.0f);
				D.PushBack(mat1);
				D.PushBack(mat2);

				// Creating a copy using the copy constructor
				Datum Dcoy(D);

				// Check if the copied Datum is equal to the original
				Assert::AreEqual(D, Dcoy);
			}
		
			// Copy constructor for String type
			 {
				Datum D;
				std::string str1 = "hello";
				std::string str2 = "world";
				D.PushBack(str1);
				D.PushBack(str2);

				//Creating a copy using the copy constructor
				Datum Dcoy(D);

				//Check if the copied Datum is equal to the original
				Assert::AreEqual(D, Dcoy);
			}

		}
		TEST_METHOD(MoveConstructor)
		{
			// Integer type test
			Datum D1;
			D1.PushBack(42); // Pushing an integer value into Datum D1

			// Move constructor to create Datum D2 from D1
			Datum D2(std::move(D1));

			// D1 should be in an unknown state after moving its content to D2
			Assert::AreEqual(D1.Type(), Datum::DatumType::Unknown); // D1 type should be unknown
			Assert::AreEqual(D1.Size(), static_cast<size_t>(0)); // D1 size should be 0
			Assert::AreEqual(D1.Capacity(), static_cast<size_t>(0)); // D1 capacity should be 0

			// D2 should contain the integer value 42 after moving from D1
			Assert::AreEqual(D2.Type(), Datum::DatumType::Integer); // D2 type should be Integer
			Assert::AreEqual(D2.Size(), static_cast<size_t>(1)); // D2 size should be 1
			Assert::AreEqual(D2.Capacity(), static_cast<size_t>(1)); // D2 capacity should be 1

			// Float type test
			D1.Clear(); // Clearing Datum D1
			D1.PushBack(3.14f); // Pushing a float value into Datum D1
			Datum D3(std::move(D1)); // Move constructor to create Datum D3 from D1

			// D1 should be in an unknown state after moving its content to D3
			Assert::AreEqual(D1.Type(), Datum::DatumType::Unknown); // D1 type should be unknown
			Assert::AreEqual(D1.Size(), static_cast<size_t>(0)); // D1 size should be 0
			Assert::AreEqual(D1.Capacity(), static_cast<size_t>(0)); // D1 capacity should be 0

			// D3 should contain the float value 3.14 after moving from D1
			Assert::AreEqual(D3.Type(), Datum::DatumType::Float); // D3 type should be Float
			Assert::AreEqual(D3.Size(), static_cast<size_t>(1)); // D3 size should be 1
			Assert::AreEqual(D3.Capacity(), static_cast<size_t>(1)); // D3 capacity should be 1

			// Vector type test
			D1.Clear(); // Clearing Datum D1
			D1.PushBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f)); // Pushing a glm::vec4 value into Datum D1
			Datum D4(std::move(D1)); // Move constructor to create Datum D4 from D1

			// D1 should be in an unknown state after moving its content to D4
			Assert::AreEqual(D1.Type(), Datum::DatumType::Unknown); // D1 type should be unknown
			Assert::AreEqual(D1.Size(), static_cast<size_t>(0)); // D1 size should be 0
			Assert::AreEqual(D1.Capacity(), static_cast<size_t>(0)); // D1 capacity should be 0

			// D4 should contain the glm::vec4 value (1.0f, 2.0f, 3.0f, 4.0f) after moving from D1
			Assert::AreEqual(D4.Type(), Datum::DatumType::Vector); // D4 type should be Vector
			Assert::AreEqual(D4.Size(), static_cast<size_t>(1)); // D4 size should be 1
			Assert::AreEqual(D4.Capacity(), static_cast<size_t>(1)); // D4 capacity should be 1

			// Matrix type test
			D1.Clear(); // Clearing Datum D1
			D1.PushBack(glm::mat4(1.0f)); // Pushing a glm::mat4 value into Datum D1
			Datum D5(std::move(D1)); // Move constructor to create Datum D5 from D1

			// D1 should be in an unknown state after moving its content to D5
			Assert::AreEqual(D1.Type(), Datum::DatumType::Unknown); // D1 type should be unknown
			Assert::AreEqual(D1.Size(), static_cast<size_t>(0)); // D1 size should be 0
			Assert::AreEqual(D1.Capacity(), static_cast<size_t>(0)); // D1 capacity should be 0

			// D5 should contain the glm::mat4 value (1.0f) after moving from D1
			Assert::AreEqual(D5.Type(), Datum::DatumType::Matrix); // D5 type should be Matrix
			Assert::AreEqual(D5.Size(), static_cast<size_t>(1)); // D5 size should be 1
			Assert::AreEqual(D5.Capacity(), static_cast<size_t>(1)); // D5 capacity should be 1

			// String type test
			D1.Clear(); // Clearing Datum D1
			D1.PushBack("Hello"); // Pushing a string value into Datum D1
			Datum D6(std::move(D1)); // Move constructor to create Datum D6 from D1

			// D1 should be in an unknown state after moving its content to D6
			Assert::AreEqual(D1.Type(), Datum::DatumType::Unknown); // D1 type should be unknown
			Assert::AreEqual(D1.Size(), static_cast<size_t>(0)); // D1 size should be 0
			Assert::AreEqual(D1.Capacity(), static_cast<size_t>(0)); // D1 capacity should be 0

			// D6 should contain the string value "Hello" after moving from D1
			Assert::AreEqual(D6.Type(), Datum::DatumType::String); // D6 type should be String
			Assert::AreEqual(D6.Size(), static_cast<size_t>(1)); // D6 size should be 1
			Assert::AreEqual(D6.Capacity(), static_cast<size_t>(1)); // D6 capacity should be 1
		}

		TEST_METHOD(MoveAssignment)
		{
			// Create Datum instances for different types
			Datum sourceInteger, sourceFloat, sourceVector, sourceMatrix, sourceString;
			Datum destinationInteger, destinationFloat, destinationVector, destinationMatrix, destinationString;

			// Populate the source Datums with values
			sourceInteger.PushBack(1); // Integer
			sourceFloat.PushBack(2.5f); // Float
			sourceVector.PushBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f)); // Vector
			sourceMatrix.PushBack(glm::mat4(1.0f)); // Matrix
			sourceString.PushBack("hello"); // String

			// Move Assignment for Integer type
			destinationInteger = std::move(sourceInteger);

			// Ensure the destination Datum has the correct properties after move assignment
			Assert::AreEqual(destinationInteger.Size(), (size_t)1);
			Assert::AreEqual(destinationInteger.Capacity(), (size_t)1);
			Assert::AreEqual(destinationInteger.Type(), Datum::DatumType::Integer);
			Assert::AreEqual(destinationInteger.GetInt(0), 1);
			Assert::AreEqual(sourceInteger.Type(), Datum::DatumType::Unknown);

			// Move Assignment for Float type
			destinationFloat = std::move(sourceFloat);

			// Ensure the destination Datum has the correct properties after move assignment
			Assert::AreEqual(destinationFloat.Size(), (size_t)1);
			Assert::AreEqual(destinationFloat.Capacity(), (size_t)1);
			Assert::AreEqual(destinationFloat.Type(), Datum::DatumType::Float);
			Assert::AreEqual(destinationFloat.GetFloat(0), 2.5f);

			// Move Assignment for Vector type
			destinationVector = std::move(sourceVector);

			// Ensure the destination Datum has the correct properties after move assignment
			Assert::AreEqual(destinationVector.Size(), (size_t)1);
			Assert::AreEqual(destinationVector.Capacity(), (size_t)1);
			Assert::AreEqual(destinationVector.Type(), Datum::DatumType::Vector);
			Assert::AreEqual(destinationVector.GetVector(0), glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));

			// Move Assignment for Matrix type
			destinationMatrix = std::move(sourceMatrix);

			// Ensure the destination Datum has the correct properties after move assignment
			Assert::AreEqual(destinationMatrix.Size(), (size_t)1);
			Assert::AreEqual(destinationMatrix.Capacity(), (size_t)1);
			Assert::AreEqual(destinationMatrix.Type(), Datum::DatumType::Matrix);
			Assert::AreEqual(destinationMatrix.GetMatrix(0), glm::mat4(1.0f));

			// Move Assignment for String type
			destinationString = std::move(sourceString);

			// Ensure the destination Datum has the correct properties after move assignment
			Assert::AreEqual(destinationString.Size(), (size_t)1);
			Assert::AreEqual(destinationString.Capacity(), (size_t)1);
			Assert::AreEqual(destinationString.Type(), Datum::DatumType::String);
			Assert::AreEqual(destinationString.GetString(0), std::string("hello"));
			

			// Ensure the source Datum is in a valid moved-from state
			Assert::AreEqual(sourceString.Size(), (size_t)0);
			Assert::AreEqual(sourceString.Capacity(), (size_t)0);
			Assert::AreEqual(sourceString.Type(), Datum::DatumType::Unknown);
		}
		TEST_METHOD(CopyAssignmentOperator)
		{
			// Create Datum instances for different types
			Datum sourceInteger, sourceFloat, sourceVector, sourceMatrix, sourceString;
			Datum destinationInteger, destinationFloat, destinationVector, destinationMatrix, destinationString;

			// Populate the source Datums with values
			sourceInteger.PushBack(1); // Integer
			sourceFloat.PushBack(2.5f); // Float
			sourceVector.PushBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f)); // Vector
			sourceMatrix.PushBack(glm::mat4(1.0f)); // Matrix
			sourceString.PushBack("hello"); // String

			// Copy Assignment for Integer type
			destinationInteger = sourceInteger;
			Assert::AreEqual(destinationInteger.Size(), (size_t)1); // Check if the size is 1
			Assert::AreEqual(destinationInteger.Capacity(), (size_t)1); // Check if the capacity is 1
			Assert::AreEqual(destinationInteger.Type(), Datum::DatumType::Integer); // Check if the type is Integer
			Assert::AreEqual(destinationInteger.GetInt(0), sourceInteger.GetInt(0)); // Check if the value is correctly copied

			// Copy Assignment for Float type
			destinationFloat = sourceFloat;
			Assert::AreEqual(destinationFloat.Size(), (size_t)1); // Check if the size is 1
			Assert::AreEqual(destinationFloat.Capacity(), (size_t)1); // Check if the capacity is 1
			Assert::AreEqual(destinationFloat.Type(), Datum::DatumType::Float); // Check if the type is Float
			Assert::AreEqual(destinationFloat.GetFloat(0), sourceFloat.GetFloat(0)); // Check if the value is correctly copied

			// Copy Assignment for Vector type
			destinationVector = sourceVector;
			Assert::AreEqual(destinationVector.Size(), (size_t)1); // Check if the size is 1
			Assert::AreEqual(destinationVector.Capacity(), (size_t)1); // Check if the capacity is 1
			Assert::AreEqual(destinationVector.Type(), Datum::DatumType::Vector); // Check if the type is Vector
			Assert::AreEqual(destinationVector.GetVector(0), sourceVector.GetVector(0)); // Check if the value is correctly copied

			// Copy Assignment for Matrix type
			destinationMatrix = sourceMatrix;
			Assert::AreEqual(destinationMatrix.Size(), (size_t)1); // Check if the size is 1
			Assert::AreEqual(destinationMatrix.Capacity(), (size_t)1); // Check if the capacity is 1
			Assert::AreEqual(destinationMatrix.Type(), Datum::DatumType::Matrix); // Check if the type is Matrix
			Assert::AreEqual(destinationMatrix.GetMatrix(0), sourceMatrix.GetMatrix(0)); // Check if the value is correctly copied

			// Copy Assignment for String type
			destinationString = sourceString;
			Assert::AreEqual(destinationString.Size(), (size_t)1); // Check if the size is 1
			Assert::AreEqual(destinationString.Capacity(), (size_t)1); // Check if the capacity is 1
			Assert::AreEqual(destinationString.Type(), Datum::DatumType::String); // Check if the type is String
			Assert::AreEqual(destinationString.GetString(0), sourceString.GetString(0)); // Check if the value is correctly copied
		}
		
		TEST_METHOD(PopBack)
		{
			// Create Datum instances for different types
			Datum intDatum, floatDatum, vectorDatum, matrixDatum, stringDatum;

			// Push back values to each Datum
			intDatum.PushBack(5); // Integer
			floatDatum.PushBack(3.14f); // Float
			vectorDatum.PushBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f)); // Vector
			matrixDatum.PushBack(glm::mat4(1.0f)); // Matrix
			stringDatum.PushBack("hello"); // String

			// Pop back values from each Datum
			intDatum.PopBack();
			floatDatum.PopBack();
			vectorDatum.PopBack();
			matrixDatum.PopBack();
			stringDatum.PopBack();

			// Check sizes after PopBack
			Assert::AreEqual(intDatum.Size(), (size_t)0);
			Assert::AreEqual(floatDatum.Size(), (size_t)0);
			Assert::AreEqual(vectorDatum.Size(), (size_t)0);
			Assert::AreEqual(matrixDatum.Size(), (size_t)0);
			Assert::AreEqual(stringDatum.Size(), (size_t)0);

			// Handle exceptions for empty datums
			Assert::ExpectException<std::out_of_range>([&intDatum] { intDatum.PopBack(); });
			Assert::ExpectException<std::out_of_range>([&floatDatum] { floatDatum.PopBack(); });
			Assert::ExpectException<std::out_of_range>([&vectorDatum] { vectorDatum.PopBack(); });
			Assert::ExpectException<std::out_of_range>([&matrixDatum] { matrixDatum.PopBack(); });
			Assert::ExpectException<std::out_of_range>([&stringDatum] { stringDatum.PopBack(); });
		}
		TEST_METHOD(GetInt)
		{
			// Create a Datum with an integer
			Datum D;
			D.PushBack(42);

			// Retrieve the integer using GetInt
			const int& result = D.GetInt(0);

			// Validate the result
			Assert::AreEqual(result, 42);
		}
		TEST_METHOD(GetFloat)
		{
			// Create a Datum with a float
			Datum D;
			D.PushBack(3.14f);

			// Retrieve the float using GetFloat
			const float& result = D.GetFloat(0);

			// Validate the result
			Assert::AreEqual(result, 3.14f); // Allowing for a small epsilon for floating-point comparisons
		}
		TEST_METHOD(GetString)
		{
			// Create a Datum with a string
			Datum D;
			std::string stringValue = "Hello, World!";
			D.PushBack(stringValue);

			// Retrieve the string using GetString
			const std::string& result = D.GetString(0);

			// Validate the result
			Assert::AreEqual(result, stringValue);
		}
		TEST_METHOD(GetMatrix)
		{
			// Create a Datum with a matrix
			Datum D;
			glm::mat4 matrixValue(1.0f);
			D.PushBack(matrixValue);

			// Retrieve the matrix using GetMatrix
			const glm::mat4& result = D.GetMatrix(0);

			// Validate the result
			Assert::AreEqual(result, matrixValue);
		}
		TEST_METHOD(GetVector)
		{
			// Create a Datum with a vector
			Datum D;
			glm::vec4 vectorValue(1.0f, 2.0f, 3.0f, 4.0f);
			D.PushBack(vectorValue);

			// Retrieve the vector using GetVector
			const glm::vec4& result = D.GetVector(0);

			// Validate the result
			Assert::AreEqual(result, vectorValue);
		}
		TEST_METHOD(ScalarAssignmentOperator)
		{
			// Create Datum instances for different types
			Datum intDatum, floatDatum, vectorDatum, matrixDatum, stringDatum;

			// Push back values to each Datum
			intDatum.PushBack(10); // Integer
			floatDatum.PushBack(3.14f); // Float
			vectorDatum.PushBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f)); // Vector
			matrixDatum.PushBack(glm::mat4(1.0f)); // Matrix
			stringDatum.PushBack("hello"); // String

			// Assign new values using the assignment operator for each Datum
			intDatum = 5; // Assign an integer
			floatDatum = 2.5f; // Assign a float
			vectorDatum = glm::vec4(5.0f, 6.0f, 7.0f, 8.0f); // Assign a vector
			matrixDatum = glm::mat4(2.0f); // Assign a matrix
			stringDatum = "world"; // Assign a string

			// Verify sizes after assignment
			Assert::AreEqual(intDatum.Size(), (size_t)1);
			Assert::AreEqual(floatDatum.Size(), (size_t)1);
			Assert::AreEqual(vectorDatum.Size(), (size_t)1);
			Assert::AreEqual(matrixDatum.Size(), (size_t)1);
			Assert::AreEqual(stringDatum.Size(), (size_t)1);

			// Verify values after assignment
			Assert::AreEqual(intDatum.GetInt(0), 5);
			Assert::AreEqual(floatDatum.GetFloat(0), 2.5f);
			Assert::AreEqual(vectorDatum.GetVector(0), glm::vec4(5.0f, 6.0f, 7.0f, 8.0f));
			Assert::AreEqual(matrixDatum.GetMatrix(0), glm::mat4(2.0f));
			Assert::AreEqual(stringDatum.GetString(0), std::string("world"));
		}
		// Test resizing to a smaller size
		TEST_METHOD(ResizeSmallerInteger)
		{
			Datum integerDatum;
			integerDatum.PushBack(1);
			integerDatum.PushBack(2);
			integerDatum.PushBack(3);

			// Resize to a smaller size
			integerDatum.Resize(2);

			// Ensure the size is updated
			Assert::AreEqual(integerDatum.Size(), (size_t)2);

			// Ensure the first elements remain
			Assert::AreEqual(integerDatum.GetInt(0), 1);
			Assert::AreEqual(integerDatum.GetInt(1), 2);

			//Float Datum
			Datum floatDatum;
			floatDatum.PushBack(1.5f);
			floatDatum.PushBack(2.5f);
			floatDatum.PushBack(3.5f);

			// Resize to a smaller size
			floatDatum.Resize(2);

			// Ensure the size is updated
			Assert::AreEqual(floatDatum.Size(), (size_t)2);

			// Ensure the first elements remain
			Assert::AreEqual(floatDatum.GetFloat(0), 1.5f);
			Assert::AreEqual(floatDatum.GetFloat(1), 2.5f);

			// Test resizing to a smaller size for Vector Datum
			Datum vectorDatum;
			vectorDatum.PushBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
			vectorDatum.PushBack(glm::vec4(5.0f, 6.0f, 7.0f, 8.0f));
			vectorDatum.PushBack(glm::vec4(9.0f, 10.0f, 11.0f, 12.0f));

			// Resize to a smaller size
			vectorDatum.Resize(2);

			// Ensure the size is updated
			Assert::AreEqual(vectorDatum.Size(), (size_t)2);

			// Ensure the first elements remain
			Assert::AreEqual(vectorDatum.GetVector(0), glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
			Assert::AreEqual(vectorDatum.GetVector(1), glm::vec4(5.0f, 6.0f, 7.0f, 8.0f));

			// Test resizing to a smaller size for Matrix Datum
			Datum matrixDatum;
			matrixDatum.PushBack(glm::mat4(1.0f));
			matrixDatum.PushBack(glm::mat4(2.0f));
			matrixDatum.PushBack(glm::mat4(3.0f));

			// Resize to a smaller size
			matrixDatum.Resize(2);

			// Ensure the size is updated
			Assert::AreEqual(matrixDatum.Size(), (size_t)2);

			// Ensure the first elements remain
			Assert::AreEqual(matrixDatum.GetMatrix(0), glm::mat4(1.0f));
			Assert::AreEqual(matrixDatum.GetMatrix(1), glm::mat4(2.0f));

			// Test resizing to a smaller size for String Datum
			Datum stringDatum;
			stringDatum.PushBack("apple");
			stringDatum.PushBack("banana");
			stringDatum.PushBack("orange");

			// Resize to a smaller size
			stringDatum.Resize(2);

			// Ensure the size is updated
			Assert::AreEqual(stringDatum.Size(), (size_t)2);

			// Ensure the first elements remain
			Assert::AreEqual(stringDatum.GetString(0), std::string("apple"));
			Assert::AreEqual(stringDatum.GetString(1), std::string("banana"));
		}
		// Test resizing to a larger size with equal capacity for integer Datum
		/*TEST_METHOD(ResizeToLargerSize)
		{
			Datum integerDatum;
			integerDatum.PushBack(1);

			// Resize to a larger size with equal capacity
			integerDatum.Resize(3);

			// Ensure the size is updated
			Assert::AreEqual(integerDatum.Size(), (size_t)3);

			// Ensure the first element remains
			Assert::AreEqual(integerDatum.GetInt(0), 1);

			// Ensure the new elements are default-initialized
			Assert::AreEqual(integerDatum.GetInt(1), 0);
			Assert::AreEqual(integerDatum.GetInt(2), 0);

			//Float
			Datum floatDatum;
			floatDatum.PushBack(1.5f);

			// Resize to a larger size with equal capacity
			floatDatum.Resize(3);

			// Ensure the size is updated
			Assert::AreEqual(floatDatum.Size(), (size_t)3);

			// Ensure the first element remains
			Assert::AreEqual(floatDatum.GetFloat(0), 1.5f);

			// Ensure the new elements are default-initialized
			Assert::AreEqual(floatDatum.GetFloat(1), 0.0f);
			Assert::AreEqual(floatDatum.GetFloat(2), 0.0f);

			// Vector
			Datum vectorDatum;
			vectorDatum.PushBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));

			// Resize to a larger size with equal capacity
			vectorDatum.Resize(3);

			// Ensure the size is updated
			Assert::AreEqual(vectorDatum.Size(), (size_t)3);

			// Ensure the first element remains
			Assert::IsTrue(glm::all(glm::equal(vectorDatum.GetVector(0), glm::vec4(1.0f, 2.0f, 3.0f, 4.0f))));

			// Ensure the new elements are default-initialized
			Assert::AreEqual(vectorDatum.GetVector(1), glm::vec4(0.0f));
			Assert::AreEqual(vectorDatum.GetVector(2), glm::vec4(0.0f));

			// Matrix
			Datum matrixDatum;
			matrixDatum.PushBack(glm::mat4(1.0f));

			// Resize to a larger size with equal capacity
			matrixDatum.Resize(3);

			// Ensure the size is updated
			Assert::AreEqual(matrixDatum.Size(), (size_t)3);

			// Ensure the matrices are equal
			Assert::AreEqual(matrixDatum.GetMatrix(0), glm::mat4(1.0f));

			// Ensure the new elements are default-initialized
			Assert::AreEqual(matrixDatum.GetMatrix(1), glm::mat4(0.0f));
			Assert::AreEqual(matrixDatum.GetMatrix(2), glm::mat4(0.0f));


			// String
			Datum stringDatum;
			stringDatum.PushBack("apple");

			// Resize to a larger size with equal capacity
			stringDatum.Resize(3);

			// Ensure the size is updated
			Assert::AreEqual(stringDatum.Size(), (size_t)3);

			// Ensure the first element remains
			Assert::AreEqual(stringDatum.GetString(0), std::string("apple"));

			// Ensure the new elements are default-initialized
			Assert::AreEqual(stringDatum.GetString(1), std::string());
			Assert::AreEqual(stringDatum.GetString(2), std::string());
		}*/
		// Test resizing to an equal size with different capacity for integer Datum
		TEST_METHOD(ResizeToSameSize)
		{
			Datum integerDatum;
			integerDatum.PushBack(1);
			integerDatum.PushBack(2);

			// Resize to an equal size with different capacity
			integerDatum.Resize(2);

			// Ensure the size is updated
			Assert::AreEqual(integerDatum.Size(), (size_t)2);

			// Ensure the elements remain unchanged
			Assert::AreEqual(integerDatum.GetInt(0), 1);
			Assert::AreEqual(integerDatum.GetInt(1), 2);

			Datum floatDatum;
			floatDatum.PushBack(1.5f);
			floatDatum.PushBack(2.5f);

			// Resize to an equal size with different capacity
			floatDatum.Resize(2);

			// Ensure the size is updated
			Assert::AreEqual(floatDatum.Size(), (size_t)2);

			// Ensure the elements remain unchanged
			Assert::AreEqual(floatDatum.GetFloat(0), 1.5f);
			Assert::AreEqual(floatDatum.GetFloat(1), 2.5f);

			Datum vectorDatum;
			vectorDatum.PushBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
			vectorDatum.PushBack(glm::vec4(5.0f, 6.0f, 7.0f, 8.0f));

			// Resize to an equal size with different capacity
			vectorDatum.Resize(2);

			// Ensure the size is updated
			Assert::AreEqual(vectorDatum.Size(), (size_t)2);

			// Ensure the elements remain unchanged
			Assert::AreEqual(vectorDatum.GetVector(0), glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
			Assert::AreEqual(vectorDatum.GetVector(1), glm::vec4(5.0f, 6.0f, 7.0f, 8.0f));

			Datum matrixDatum;
			matrixDatum.PushBack(glm::mat4(1.0f));
			matrixDatum.PushBack(glm::mat4(2.0f));

			// Resize to an equal size with different capacity
			matrixDatum.Resize(2);

			// Ensure the size is updated
			Assert::AreEqual(matrixDatum.Size(), (size_t)2);

			// Ensure the elements remain unchanged
			Assert::AreEqual(matrixDatum.GetMatrix(0), glm::mat4(1.0f));
			Assert::AreEqual(matrixDatum.GetMatrix(1), glm::mat4(2.0f));

			Datum stringDatum;
			stringDatum.PushBack("hello");
			stringDatum.PushBack("world");

			// Resize to an equal size with different capacity
			stringDatum.Resize(2);

			// Ensure the size is updated
			Assert::AreEqual(stringDatum.Size(), (size_t)2);

			// Ensure the elements remain unchanged
			Assert::AreEqual(stringDatum.GetString(0), std::string("hello"));
			Assert::AreEqual(stringDatum.GetString(1), std::string("world"));
		}
		TEST_METHOD(BetweenSizeAndCapacity)
		{
			// Integer
			{
				Datum D;

				D.PushBack(1);
				D.PushBack(2);
				D.PushBack(3);
				D.PushBack(4);
				D.PushBack(5);

				// Assert initial state
				Assert::AreEqual(D.Size(), (size_t)5);
				Assert::AreEqual(D.Capacity(), (size_t)8);

				// PopBack and assert
				D.PopBack();
				D.PopBack();
				D.PopBack();

				Assert::AreEqual(D.Size(), (size_t)2);
				Assert::AreEqual(D.Capacity(), (size_t)8);

				// Resize and assert
				D.Resize(3);

				Assert::AreEqual(D.Size(), (size_t)3);
				Assert::AreEqual(D.Capacity(), (size_t)8);

				// Assert elements
				Assert::AreEqual(D.GetInt(0), 1);
				Assert::AreEqual(D.GetInt(1), 2);
				Assert::AreEqual(D.GetInt(2), 0); // New default int value after resize.
			}

			// Float
			{
				Datum D;

				D.PushBack(1.0f);
				D.PushBack(2.0f);
				D.PushBack(3.0f);
				D.PushBack(4.0f);
				D.PushBack(5.0f);

				// Assert initial state
				Assert::AreEqual(D.Size(), (size_t)5);
				Assert::AreEqual(D.Capacity(), (size_t)8);

				// PopBack and assert
				D.PopBack();
				D.PopBack();
				D.PopBack();

				Assert::AreEqual(D.Size(), (size_t)2);
				Assert::AreEqual(D.Capacity(), (size_t)8);

				// Resize and assert
				D.Resize(3);

				Assert::AreEqual(D.Size(), (size_t)3);
				Assert::AreEqual(D.Capacity(), (size_t)8);

				// Assert elements
				Assert::AreEqual(D.GetFloat(0), 1.0f);
				Assert::AreEqual(D.GetFloat(1), 2.0f);
				Assert::AreEqual(D.GetFloat(2), 0.0f); // New default float value after resize.
			}

			// Vector
			{
				Datum D;

				glm::vec4 arr[5] = { glm::vec4(1.0f, 2.0f, 3.0f, 4.0f), glm::vec4(5.0f, 6.0f, 7.0f, 8.0f), glm::vec4(9.0f, 10.0f, 11.0f, 12.0f) };

				D.PushBack(arr[0]);
				D.PushBack(arr[1]);
				D.PushBack(arr[2]);
				D.PushBack(arr[3]);
				D.PushBack(arr[4]);

				// Assert initial state
				Assert::AreEqual(D.Size(), (size_t)5);
				Assert::AreEqual(D.Capacity(), (size_t)8);

				// PopBack and assert
				D.PopBack();
				D.PopBack();
				D.PopBack();

				Assert::AreEqual(D.Size(), (size_t)2);
				Assert::AreEqual(D.Capacity(), (size_t)8);

				// Resize and assert
				D.Resize(3);

				Assert::AreEqual(D.Size(), (size_t)3);
				Assert::AreEqual(D.Capacity(), (size_t)8);

				// Assert elements
				Assert::IsTrue(D.GetVector(0) == arr[0]);
				Assert::IsTrue(D.GetVector(1) == arr[1]);
				Assert::IsTrue(D.GetVector(2) == glm::vec4(0.0f)); // New default vector value after resize.
			}

			// Matrix
			{
				Datum D;

				glm::mat4 arr[5] = { glm::mat4(1.0f), glm::mat4(2.0f), glm::mat4(3.0f) };

				D.PushBack(arr[0]);
				D.PushBack(arr[1]);
				D.PushBack(arr[2]);

				// Assert initial state
				Assert::AreEqual(D.Size(), (size_t)3);
				Assert::AreEqual(D.Capacity(), (size_t)4);

				// PopBack and assert
				D.PopBack();
				D.PopBack();

				Assert::AreEqual(D.Size(), (size_t)1);
				Assert::AreEqual(D.Capacity(), (size_t)4);

				// Resize and assert
				D.Resize(3);

				Assert::AreEqual(D.Size(), (size_t)3);
				Assert::AreEqual(D.Capacity(), (size_t)4);

				// Assert elements
				Assert::AreEqual(D.GetMatrix(0), arr[0]);
				Assert::AreEqual(D.GetMatrix(1), glm::mat4(0.0f)); // New default matrix value after resize.
				Assert::AreEqual(D.GetMatrix(2), glm::mat4(0.0f)); // New default matrix value after resize.

				// String
				{
					Datum D;

					std::string arr[3] = { "apple", "banana", "cherry" };

					D.PushBack(arr[0]);
					D.PushBack(arr[1]);
					D.PushBack(arr[2]);

					// Assert initial state
					Assert::AreEqual(D.Size(), (size_t)3);
					Assert::AreEqual(D.Capacity(), (size_t)4);

					// PopBack and assert
					D.PopBack();
					D.PopBack();

					Assert::AreEqual(D.Size(), (size_t)1);
					Assert::AreEqual(D.Capacity(), (size_t)4);

					// Resize and assert
					D.Resize(3);

					Assert::AreEqual(D.Size(), (size_t)3);
					Assert::AreEqual(D.Capacity(), (size_t)4);

					// Assert elements
					Assert::AreEqual(D.GetString(0), arr[0]);
					Assert::IsTrue(D.GetString(1) == ""); // New default string value after resize.
					Assert::IsTrue(D.GetString(2) == ""); // New default string value after resize.
				}
			}
		}

		// Clear for Integer Datum
		TEST_METHOD(ClearInteger)
		{
			Datum integerDatum;
			integerDatum.PushBack(1);
			integerDatum.PushBack(2);

			// Clear the Datum
			integerDatum.Clear();

			// Ensure the size is updated
			Assert::AreEqual(integerDatum.Size(), (size_t)0);
			// The capacity should remain unchanged
			Assert::AreEqual(integerDatum.Capacity(), (size_t)0);
			// Type should be set to Unknown after clear
			Assert::AreEqual(integerDatum.Type(), Datum::DatumType::Unknown);
		}
		// Clear for Float Datum
		TEST_METHOD(ClearFloat)
		{
			Datum floatDatum;
			floatDatum.PushBack(1.5f);
			floatDatum.PushBack(2.5f);

			// Clear the Datum
			floatDatum.Clear();

			// Ensure the size is updated
			Assert::AreEqual(floatDatum.Size(), (size_t)0);
			// The capacity should remain unchanged
			Assert::AreEqual(floatDatum.Capacity(), (size_t)0);
			// Type should be set to Unknown after clear
			Assert::AreEqual(floatDatum.Type(), Datum::DatumType::Unknown);
		}
		// Clear for Vector Datum
		TEST_METHOD(ClearVector)
		{
			Datum vectorDatum;
			vectorDatum.PushBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
			vectorDatum.PushBack(glm::vec4(5.0f, 6.0f, 7.0f, 8.0f));

			// Clear the Datum
			vectorDatum.Clear();

			// Ensure the size is updated
			Assert::AreEqual(vectorDatum.Size(), (size_t)0);
			// The capacity should remain unchanged
			Assert::AreEqual(vectorDatum.Capacity(), (size_t)0);
			// Type should be set to Unknown after clear
			Assert::AreEqual(vectorDatum.Type(), Datum::DatumType::Unknown);
		}
		// Clear for Matrix Datum
		TEST_METHOD(ClearMatrix)
		{
			Datum matrixDatum;
			matrixDatum.PushBack(glm::mat4(1.0f));
			matrixDatum.PushBack(glm::mat4(2.0f));

			// Clear the Datum
			matrixDatum.Clear();

			// Ensure the size is updated
			Assert::AreEqual(matrixDatum.Size(), (size_t)0);
			// The capacity should remain unchanged
			Assert::AreEqual(matrixDatum.Capacity(), (size_t)0);
			// Type should be set to Unknown after clear
			Assert::AreEqual(matrixDatum.Type(), Datum::DatumType::Unknown);
		}
		// Clear for String Datum
		TEST_METHOD(ClearString)
		{
			Datum stringDatum;
			stringDatum.PushBack("hello");
			stringDatum.PushBack("world");

			// Clear the Datum
			stringDatum.Clear();

			// Ensure the size is updated
			Assert::AreEqual(stringDatum.Size(), (size_t)0);
			// The capacity should remain unchanged
			Assert::AreEqual(stringDatum.Capacity(), (size_t)0);
			// Type should be set to Unknown after clear
			Assert::AreEqual(stringDatum.Type(), Datum::DatumType::Unknown);
		}
		TEST_METHOD(EqualityOperator)
		{
			// Create Datum instances for different types
			Datum integerDatum1, integerDatum2;
			Datum floatDatum1, floatDatum2;
			Datum vectorDatum1, vectorDatum2;
			Datum matrixDatum1, matrixDatum2;
			Datum stringDatum1, stringDatum2;

			// Populate Datum instances with values
			integerDatum1.PushBack(42);
			integerDatum2.PushBack(42);

			floatDatum1.PushBack(3.14f);
			floatDatum2.PushBack(3.14f);

			vectorDatum1.PushBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
			vectorDatum2.PushBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));

			matrixDatum1.PushBack(glm::mat4(1.0f));
			matrixDatum2.PushBack(glm::mat4(1.0f));

			stringDatum1.PushBack("hello");
			stringDatum2.PushBack("hello");

			// Test equality for Integer type
			Assert::IsTrue(integerDatum1 == integerDatum2);

			// Test equality for Float type
			Assert::IsTrue(floatDatum1 == floatDatum2);

			// Test equality for Vector type
			Assert::IsTrue(vectorDatum1 == vectorDatum2);

			// Test equality for Matrix type
			Assert::IsTrue(matrixDatum1 == matrixDatum2);

			// Test equality for String type
			Assert::IsTrue(stringDatum1 == stringDatum2);

			// Create Datum instances with different values
			integerDatum2.Clear();
			integerDatum2.PushBack(24);

			floatDatum2.Clear();
			floatDatum2.PushBack(2.718f);

			vectorDatum2.Clear();
			vectorDatum2.PushBack(glm::vec4(4.0f, 3.0f, 2.0f, 1.0f));

			matrixDatum2.Clear();
			matrixDatum2.PushBack(glm::mat4(0.0f));

			stringDatum2.Clear();
			stringDatum2.PushBack("world");

			// Test inequality for Integer type
			Assert::IsFalse(integerDatum1 == integerDatum2);

			// Test inequality for Float type
			Assert::IsFalse(floatDatum1 == floatDatum2);

			// Test inequality for Vector type
			Assert::IsFalse(vectorDatum1 == vectorDatum2);

			// Test inequality for Matrix type
			Assert::IsFalse(matrixDatum1 == matrixDatum2);

			// Test inequality for String type
			Assert::IsFalse(stringDatum1 == stringDatum2);
		}
		TEST_METHOD(InequalityOperator)
		{
			// Create Datum instances for different types
			Datum integerDatum1, integerDatum2;
			Datum floatDatum1, floatDatum2;
			Datum vectorDatum1, vectorDatum2;
			Datum matrixDatum1, matrixDatum2;
			Datum stringDatum1, stringDatum2;

			// Populate Datum instances with values
			integerDatum1.PushBack(42);
			integerDatum2.PushBack(42);

			floatDatum1.PushBack(3.14f);
			floatDatum2.PushBack(3.14f);

			vectorDatum1.PushBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
			vectorDatum2.PushBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));

			matrixDatum1.PushBack(glm::mat4(1.0f));
			matrixDatum2.PushBack(glm::mat4(1.0f));

			stringDatum1.PushBack("hello");
			stringDatum2.PushBack("hello");

			// Test inequality for Integer type
			Assert::IsFalse(integerDatum1 != integerDatum2);

			// Test inequality for Float type
			Assert::IsFalse(floatDatum1 != floatDatum2);

			// Test inequality for Vector type
			Assert::IsFalse(vectorDatum1 != vectorDatum2);

			// Test inequality for Matrix type
			Assert::IsFalse(matrixDatum1 != matrixDatum2);

			// Test inequality for String type
			Assert::IsFalse(stringDatum1 != stringDatum2);

			// Create Datum instances with different values
			integerDatum2.Clear();
			integerDatum2.PushBack(24);

			floatDatum2.Clear();
			floatDatum2.PushBack(2.718f);

			vectorDatum2.Clear();
			vectorDatum2.PushBack(glm::vec4(4.0f, 3.0f, 2.0f, 1.0f));

			matrixDatum2.Clear();
			matrixDatum2.PushBack(glm::mat4(0.0f));

			stringDatum2.Clear();
			stringDatum2.PushBack("world");

			// Test inequality for Integer type
			Assert::IsTrue(integerDatum1 != integerDatum2);

			// Test inequality for Float type
			Assert::IsTrue(floatDatum1 != floatDatum2);

			// Test inequality for Vector type
			Assert::IsTrue(vectorDatum1 != vectorDatum2);

			// Test inequality for Matrix type
			Assert::IsTrue(matrixDatum1 != matrixDatum2);

			// Test inequality for String type
			Assert::IsTrue(stringDatum1 != stringDatum2);
		}
		TEST_METHOD(MoveSemantics)
		{
			// Test move assignment for int
			{
				Datum intDatum;
				intDatum = std::move(42); // Move assignment operator for int
				Assert::AreEqual(intDatum.Size(), (size_t)1);
				Assert::AreEqual(intDatum.Type(), Datum::DatumType::Integer);
				Assert::AreEqual(intDatum.GetInt(0), 42);
			}

			// Test move assignment for float
			{
				Datum floatDatum;
				floatDatum = std::move(3.14f); // Move assignment operator for float
				Assert::AreEqual(floatDatum.Size(), (size_t)1);
				Assert::AreEqual(floatDatum.Type(), Datum::DatumType::Float);
				Assert::AreEqual(floatDatum.GetFloat(0), 3.14f);
			}

			// Test move assignment for glm::vec4
			{
				Datum vec4Datum;
				vec4Datum = std::move(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f)); // Move assignment operator for glm::vec4
				Assert::AreEqual(vec4Datum.Size(), (size_t)1);
				Assert::AreEqual(vec4Datum.Type(), Datum::DatumType::Vector);
				Assert::AreEqual(vec4Datum.GetVector(0), glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
			}

			// Test move assignment for glm::mat4
			{
				Datum mat4Datum;
				mat4Datum = std::move(glm::mat4(1.0f)); // Move assignment operator for glm::mat4
				Assert::AreEqual(mat4Datum.Size(), (size_t)1);
				Assert::AreEqual(mat4Datum.Type(), Datum::DatumType::Matrix);
				Assert::AreEqual(mat4Datum.GetMatrix(0), glm::mat4(1.0f));
			}

			// Test move assignment for std::string
			{
				Datum stringDatum;
				stringDatum = std::move(std::string("Move Semantics Test")); // Move assignment operator for std::string
				Assert::AreEqual(stringDatum.Size(), (size_t)1);
				Assert::AreEqual(stringDatum.Type(), Datum::DatumType::String);
				Assert::AreEqual(stringDatum.GetString(0), std::string("Move Semantics Test"));
			}
		}

		TEST_METHOD(AssignIntWithIndex)
		{
			Datum datum;
			datum.PushBack(10);
			datum.PushBack(15);

			int newValue = 20;
			datum.Assign(newValue, 1);

			Assert::AreEqual(datum.GetInt(1), newValue);
		}

		TEST_METHOD(AssignFloatWithIndex)
		{
			Datum datum;
			datum.PushBack(3.14f);
			datum.PushBack(2.718f);

			float newValue = 1.234f;
			datum.Assign(newValue, 1);

			Assert::AreEqual(datum.GetFloat(1), newValue);
		}

		TEST_METHOD(AssignVectorWithIndex)
		{
			Datum datum;
			datum.PushBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
			datum.PushBack(glm::vec4(5.0f, 6.0f, 7.0f, 8.0f));

			glm::vec4 newValue(9.0f, 10.0f, 11.0f, 12.0f);
			datum.Assign(newValue, 1);

			Assert::AreEqual(datum.GetVector(1), newValue);
		}

		TEST_METHOD(AssignMatrixWithIndex)
		{
			Datum datum;
			datum.PushBack(glm::mat4(1.0f));
			datum.PushBack(glm::mat4(2.0f));

			glm::mat4 newValue(3.0f);
			datum.Assign(newValue, 1);

			Assert::AreEqual(datum.GetMatrix(1), newValue);
		}

		TEST_METHOD(AssignStringWithIndex)
		{
			Datum datum;
			datum.PushBack("Hello");
			datum.PushBack("World");

			std::string newValue = "Universe";
			datum.Assign(newValue, 1);

			Assert::AreEqual(datum.GetString(1), newValue);
		}

		TEST_METHOD(GetIntAsString)
		{
			Datum intDatum;
			intDatum.PushBack(42);

			// Assuming GetIntAsString returns const char*, convert it to std::string
			Assert::AreEqual(std::string(intDatum.GetIntAsString(0)), std::string("42"));
		}

		// GetFloatAsString
		TEST_METHOD(GetFloatAsString)
		{ 
			Datum floatDatum;
			floatDatum.PushBack(3.14f);

			// Assuming GetFloatAsString returns const char*, convert it to std::string
			Assert::AreEqual(std::string(floatDatum.GetFloatAsString(0)), std::string("3.140000"));
		}

		// GetVectorAsString
		TEST_METHOD(GetVectorAsString)
		{
			Datum vectorDatum;
			vectorDatum.PushBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));

			// Assuming GetVectorAsString returns const char*, convert it to std::string
			Assert::AreEqual(std::string(vectorDatum.GetVectorAsString(0)), std::string("(1.000000, 2.000000, 3.000000, 4.000000)"));
		}

		// GetMatrixAsString
		TEST_METHOD(GetMatrixAsString)
		{
			Datum matrixDatum;
			matrixDatum.PushBack(glm::mat4(1.0f));

			// Assuming GetMatrixAsString returns const char*, convert it to std::string
			Assert::AreEqual(std::string(matrixDatum.GetMatrixAsString(0)), matrixDatum.mattostring(glm::mat4(1.0f)));
		}
		//SetStorage
		TEST_METHOD(SetStorageInt)
		{
			Datum D,D2;
			int arr[3] = { 1, 2, 3 };

			D.SetStorage(arr, 3);

			Assert::AreEqual(D.Type(), Datum::DatumType::Integer);
			Assert::AreEqual(D.Size(), (size_t)3);
			Assert::AreEqual(D.Capacity(), (size_t)3);

			for (size_t i = 0; i < 3; ++i)
			{
				Assert::AreEqual(D.GetInt(i), arr[i]);
			}

		}

		TEST_METHOD(SetStorageFloat)
		{
			Datum D;
			float arr[4] = { 1.0f, 2.0f, 3.0f, 4.0f };

			D.SetStorage(arr, 4);

			Assert::AreEqual(D.Type(), Datum::DatumType::Float);
			Assert::AreEqual(D.Size(), (size_t)4);
			Assert::AreEqual(D.Capacity(), (size_t)4);

			for (size_t i = 0; i < 4; ++i)
			{
				Assert::AreEqual(D.GetFloat(i), arr[i]);
			}
		}

		TEST_METHOD(SetStorageVector)
		{
			Datum D;
			glm::vec4 arr[2] = { glm::vec4(1.0f, 2.0f, 3.0f, 4.0f), glm::vec4(5.0f, 6.0f, 7.0f, 8.0f) };

			D.SetStorage(arr, 2);

			Assert::AreEqual(D.Type(), Datum::DatumType::Vector);
			Assert::AreEqual(D.Size(), (size_t)2);
			Assert::AreEqual(D.Capacity(), (size_t)2);

			for (size_t i = 0; i < 2; ++i)
			{
				Assert::IsTrue(D.GetVector(i) == arr[i]);
			}
		}

		TEST_METHOD(SetStorageMatrix)
		{
			Datum D;
			glm::mat4 arr[2] = { glm::mat4(1.0f), glm::mat4(2.0f) };
			D.SetStorage(arr, 2);

			Assert::AreEqual(D.Type(), Datum::DatumType::Matrix);
			Assert::AreEqual(D.Size(), (size_t)2);
			Assert::AreEqual(D.Capacity(), (size_t)2);

			for (size_t i = 0; i < 2; ++i)
			{
				Assert::IsTrue(D.GetMatrix(i) == arr[i]);
			}
		}

		TEST_METHOD(SetStorageString)
		{
			Datum D;
			std::string arr[3] = { "apple", "banana", "cherry" };

			D.SetStorage(arr, 3);

			Assert::AreEqual(D.Type(), Datum::DatumType::String);
			Assert::AreEqual(D.Size(), (size_t)3);
			Assert::AreEqual(D.Capacity(), (size_t)3);

			for (size_t i = 0; i < 3; ++i)
			{
				Assert::AreEqual(D.GetString(i), arr[i]);
			}

		}
		TEST_METHOD(RemoveAt_IntegerType)
		{
			Datum datum;
			datum.PushBack(10);
			datum.PushBack(20);
			datum.PushBack(30);
			datum.PushBack(40);
			datum.PushBack(50);

			datum.RemoveAt(1);

			Assert::AreEqual((size_t)4, datum.Size()); // Check if the size is decremented
			Assert::AreEqual(10, datum.GetInt(0)); // Check if the first element is unchanged
			Assert::AreEqual(30, datum.GetInt(1)); // Check if the third element shifted to the correct position 
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}