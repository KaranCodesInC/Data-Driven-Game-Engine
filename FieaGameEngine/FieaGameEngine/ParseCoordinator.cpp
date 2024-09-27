#include "pch.h"
#include "IParseHandler.h"
#include <iostream>
#include "ParseCoordinator.h"
#include <fstream>
#include <assert.h>

using namespace std;
namespace Fiea::GameEngine
{
    /**
    * Constructor for ParseCoordinator.
    * Initializes ParseCoordinator with a wrapper object.
    * @param w The wrapper object containing parsing information.
    */
    ParseCoordinator::ParseCoordinator(Wrapper& w) : wrapper(w) {}

    ParseCoordinator::~ParseCoordinator()
    {
        handlers.clear();
    }

    /**
    * Adds a parse handler to the coordinator.
    * @param Handler A shared pointer to the parse handler to add.
    */
    void ParseCoordinator::AddHandler(const shared_ptr<IParseHandler>& Handler) {
        //std::shared_ptr<IParseHandler> shared_ptr(Handler);
        //handlers[handlerIndex] = std::move(shared_ptr);
        Handler->Initialize();
        handlers[handlerIndex] = Handler;
        ++handlerIndex;
    }

    /**
    * Removes a parse handler from the coordinator.
    * @param handler The parse handler to remove.
    */
    void ParseCoordinator::RemoveHandler(IParseHandler& handler) {
        for (auto it = handlers.begin(); it != handlers.end(); ++it) {
            if (it->second.get() == &handler) {
                handlers.erase(it);
                return;
            }
        }
    }

    /**
     * Deserialize JSON data from a string.
     * @param jsonData The JSON data string to deserialize.
     * @return True if the deserialization was successful, false otherwise.
     */
    bool ParseCoordinator::DeserializeObject(const std::string& jsonData) {
        Json::Value root;
        Json::CharReaderBuilder builder;
        std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
        JSONCPP_STRING errs;
        if (reader->parse(jsonData.c_str(), jsonData.c_str() + jsonData.length(), &root, &errs)) {
            // Parsing was successful, proceed with parsing members
            ParseMembers(root);
            return true; // Return true indicating successful parsing
        }
        else {
            // Parsing failed, log an error message
            std::cerr << "Failed to parse JSON data: " << errs << std::endl;
            return false; // Return false indicating parsing failure
        }
    }

        /**
     * @brief Deserialize JSON data from an input stream.
     *
     * This function reads JSON data from the provided input stream, converts it to a string,
     * and then calls the DeserializeObject function with the string data.
     *
     * @param jsonData The input stream containing the JSON data.
     * @return True if the deserialization is successful, false otherwise.
     */
    bool ParseCoordinator::DeserializeObject(std::istream& jsonData) {
        // Read the JSON data from the input stream
        std::stringstream buffer;
        buffer << jsonData.rdbuf();
        std::string jsonDataStr = buffer.str();

        // Deserialize the JSON data
        return DeserializeObject(jsonDataStr);
    }

        /**
     * @brief Deserialize JSON data from a file.
     *
     * This function opens the specified file containing JSON data, reads the data, and then
     * calls the DeserializeObject function with the file contents.
     *
     * @param filename The path to the file containing the JSON data.
     * @return True if the deserialization is successful, false otherwise.
     */
    bool ParseCoordinator::DeserializeObjectFromFile(const std::string& filename) {
        // Open the file
        std::ifstream file(filename);
        if (!file.is_open()) {
            // Failed to open the file, log an error message
            std::cerr << "Failed to open file: " << filename << std::endl;
            return false;
        }

        // Read the JSON data from the file
        /*std::stringstream buffer;
        buffer << file.rdbuf();
        std::string jsonData = buffer.str();

        // Close the file
        file.close();*/

        // Deserialize the JSON data
        return DeserializeObject(file);
    }

        /**
     * @brief Parse members of a JSON object or array recursively.
     *
     * This function is responsible for parsing the members of a JSON object or array recursively.
     * If the input JSON value is an object, it iterates through its members and calls the Parse function
     * for each member. If the input JSON value is an array, it recursively calls ParseMembers for each element.
     *
     * @param value The JSON value to parse.
     */
    void ParseCoordinator::ParseMembers(Json::Value value)
    {
        if (value.isObject()) {
            for (const auto& member : value.getMemberNames())
            {
                //assert(value.getMemberNames() != nullptr);
                Parse(value[member], member, false);
            }
        }
        else if (value.isArray())
        {
            for (const auto& element : value)
            {
                ParseMembers(element); 
            }
        }
    }

        /**
     * @brief Parse a JSON value using registered handlers.
     *
     * This function iterates through registered handlers and attempts to start parsing the given JSON value
     * using each handler. If a handler successfully starts parsing, it calls ParseMembers to parse the nested
     * members of the JSON value. After parsing the members, it decrements the depth and calls the EndHandler
     * function of the handler.
     *
     * @param value The JSON value to parse.
     * @param key The key associated with the JSON value.
     * @param isArrayElement Flag indicating whether the value is an element of an array.
     */
    void ParseCoordinator::Parse(Json::Value& value, const std::string& key, bool isArrayElement)
    {
        for (const auto& helper : handlers) 
        {
            if (helper.second->StartHandler(wrapper, key, value, isArrayElement))
            {
                // Increment the depth before parsing members
                wrapper.IncrementDepth();
                ParseMembers(value);
                // Decrement the depth after parsing members
                wrapper.DecrementDepth();
                helper.second->EndHandler(value, wrapper, key);
                return;
            }
        }
    }
}
