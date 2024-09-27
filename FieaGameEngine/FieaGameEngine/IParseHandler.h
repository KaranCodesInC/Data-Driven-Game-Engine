#pragma once
#include "pch.h"
#include <string>
#include <json/json.h>
#include "ParseCoordinator.h"

namespace Fiea::GameEngine
{
    class IParseHandler {
    public:
        virtual ~IParseHandler() = default;

        virtual void Initialize() = 0;

        virtual bool StartHandler(ParseCoordinator::Wrapper& wrapper, const std::string& key, Json::Value& value, bool isArrayElement) = 0;

        virtual bool EndHandler(Json::Value& value, ParseCoordinator::Wrapper& wrapper, const std::string& key) = 0;

        //virtual void Cleanup() = 0;

        /**
         * @brief Checks if the attribute exists in the current scope.
         * @param key The key to check.
         * @param attribute The attribute to look for.
         * @return True if the attribute exists, false otherwise.
         */
        bool hasAttribute(const std::string& key, const std::string& attribute)
        {
            if (key.length() < attribute.length())
                return false;
            return key.substr(0, attribute.length()) == attribute;
        }
    };
}

