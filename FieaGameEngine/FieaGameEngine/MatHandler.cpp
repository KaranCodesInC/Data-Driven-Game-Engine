#include "pch.h"
#include "MatrixHandler.h"
#include "ScopeHandler.h"
#include <glm/glm.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <regex>
#include <json/json.h>
#include "AttributedHandler.h"

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(MatrixHandler::Wrapper);

	void MatrixHandler::Initialize()
	{
		initializeCalled = true;
	}

	void MatrixHandler::Wrapper::IncrementDepth()
	{
		++depth;
	}

	void MatrixHandler::Wrapper::DecrementDepth()
	{
		--depth;
	}

	size_t MatrixHandler::Wrapper::Depth() const
	{
		return depth;
	}

		/**
	 * @brief Handles the start of parsing for a matrix.
	 *
	 * @param wrapper The wrapper object for handling scope and depth.
	 * @param key The key associated with the parsed value.
	 * @param value The JSON value being parsed.
	 * @param isArrayElement A flag indicating if the value is an element of an array.
	 * @return true if the start of parsing is successful, false otherwise.
	 */
	bool MatrixHandler::StartHandler(ParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArrayElement)
	{
		ScopeHandler::Wrapper* scopeWrapper = wrapper.As<ScopeHandler::Wrapper>();
		AttributedHandler::Wrapper* attwrapper = wrapper.As<AttributedHandler::Wrapper>();
		if (scopeWrapper != nullptr)
		{
			if (hasAttribute(key, "mat"))
			{
				if (scopeWrapper->Depth() >= scopeWrapper->maxDepth)
				{
					scopeWrapper->maxDepth = scopeWrapper->Depth();
				}
				if (isMat(value))
				{

					Datum& D = scopeWrapper->GetCurrentScope()->Append(key);
					D.PushBack(JsonToMat4(value));
					++startCount;
					return true;
				}

			}
		}
		if(attwrapper != nullptr)
		{
			if (hasAttribute(key, "mat"))
			{
				if (attwrapper->Depth() >= attwrapper->maxDepth)
				{
					attwrapper->maxDepth = attwrapper->Depth();
				}
				if (isMat(value))
				{

					Datum& D = attwrapper->GetCurrentScope()->Append(key);
					D.PushBack(JsonToMat4(value));
					++startCount;
					return true;
				}
			}

		}
		return false;
	}

		/**
	 * @brief Handles the end of parsing for a matrix.
	 *
	 * @param value The JSON value being parsed.
	 * @param wrapper The wrapper object for handling scope and depth.
	 * @param key The key associated with the parsed value.
	 * @return true if the end of parsing is successful, false otherwise.
	 */
	bool MatrixHandler::EndHandler(Json::Value& value, ParseCoordinator::Wrapper& wrapper, const std::string& key)
	{
		ScopeHandler::Wrapper* scopeWrapper = wrapper.As<ScopeHandler::Wrapper>();
		AttributedHandler::Wrapper* attwrapper = wrapper.As<AttributedHandler::Wrapper>();
		if (scopeWrapper && attwrapper == nullptr)
		{
			return false;
		}

		++endCount;
		return true;
	}

		/**
	 * @brief Checks whether the given JSON value represents a 4x4 matrix.
	 *
	 * @param value The JSON value to check.
	 * @return true if the value represents a 4x4 matrix, false otherwise.
	 */
	bool MatrixHandler::isMat(Json::Value& value)
	{
		// Check if the value is an array
		if (!value.isArray())
		{
			return false;
		}

		// Check if the array has 4 elements (representing rows)
		if (value.size() != 4)
		{
			return false;
		}

		// Check if each row is an array of 4 numerical values (representing columns)
		for (unsigned int i = 0; i < 4; ++i)
		{
			const Json::Value& row = value[i];

			// Check if the row is an array
			if (!row.isArray())
			{
				return false;
			}

			// Check if the row has 4 elements (representing columns)
			if (row.size() != 4)
			{
				return false;
			}

			// Check if all elements in the row are numerical values
			for (unsigned int j = 0; j < 4; ++j)
			{
				if (!row[j].isNumeric())
				{
					return false;
				}
			}
		}

		// If all conditions are met, it is likely a mat4
		return true;
	}


		/**
	 * @brief Converts a JSON value representing a 4x4 matrix to a glm::mat4.
	 *
	 * @param value The JSON value representing the matrix.
	 * @return The glm::mat4 equivalent of the JSON matrix.
	 */
	glm::mat4 MatrixHandler::JsonToMat4(const Json::Value& value)
	{
		glm::mat4 mat = glm::mat4(0.0f);

		// Check if the value is an array with 4 rows
		if (value.isArray() && value.size() == 4)
		{
			for (unsigned int i = 0; i < 4; ++i)
			{
				const Json::Value& row = value[i];

				// Check if each row is an array with 4 columns
				if (row.isArray() && row.size() == 4)
				{
					for (unsigned int j = 0; j < 4; ++j)
					{
						// Extract numerical value and assign it to the corresponding element in the matrix
						mat[i][j] = row[j].asFloat();
					}
				}
			}
		}

		return mat;
	}
}
