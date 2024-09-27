#pragma once
#include <memory>
#include <map>
#include <string>
#include <json/json.h>
#include "RTTI.h"

using std::unique_ptr;
using std::shared_ptr;

namespace Fiea
{
	namespace GameEngine
	{
		class IParseHandler;
		class ParseCoordinator
		{
		public:

			class Wrapper : public RTTI {
			public:
				virtual ~Wrapper() = default;
				virtual void IncrementDepth() = 0;
				virtual void DecrementDepth() = 0;
				virtual size_t Depth() const = 0;

			protected:
				 
				size_t depth = 0;
			};

			ParseCoordinator(Wrapper& Wrapper);
			virtual ~ParseCoordinator();

			ParseCoordinator(const ParseCoordinator&) = delete;
			ParseCoordinator(ParseCoordinator&&) = delete;
			ParseCoordinator& operator=(const ParseCoordinator&) = delete;
			ParseCoordinator& operator=(ParseCoordinator&&) = delete;

			void AddHandler(const std::shared_ptr<IParseHandler>& Handler);
			void RemoveHandler(IParseHandler& Handler);

			bool DeserializeObject(const std::string& jsonData);
			bool DeserializeObjectFromFile(const std::string& filename);
			bool DeserializeObject(std::istream& jsonData);

			Wrapper& GetWrapper() const
			{
				return wrapper;
			}

		private:
			void ParseMembers(Json::Value value);
			void Parse(Json::Value& value, const std::string& key, bool isArrayElement);

			Wrapper& wrapper;

			size_t handlerIndex = 0;

			std::map<size_t,shared_ptr<IParseHandler>> handlers;
		};
	}
}
