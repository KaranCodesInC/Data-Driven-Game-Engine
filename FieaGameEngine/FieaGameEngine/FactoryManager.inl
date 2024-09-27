/**
 * @file FactoryManager.h
 * @brief Contains the implementation of the FactoryManager template class.
 */
#pragma once
#include "FactoryManager.h"

namespace Fiea
{
	namespace GameEngine
	{
		/**
		* @brief Finds the factory associated with the given class name.
		* @param classname The name of the class.
		* @return Reference to the found factory.
		* @exception std::runtime_error Thrown when the factory is not found for the given class name.
		*/
		template<class BaseType>
		Factory<BaseType>& FactoryManager<BaseType>::Find(const std::string& classname)
		{
			auto it = factories.find(classname);
			if (it != factories.end())
			{
				return *it->second;
			}
			else
			{
				// Handle the case when the factory is not found
				throw std::runtime_error("Factory not found for class name: " + classname);
			}
		}
		/**
		* @brief Adds a factory to the manager.
		* @param factory The factory to add.
		*/
		template<class BaseType>
		void FactoryManager<BaseType>::addFactory(std::unique_ptr<Factory<BaseType>>&& factory)
		{
			for (auto& it : factories)
			{
				if (it.second == factory)
				{
					return;
				}
			}
			factories[factory->className()] = std::move(factory);
		}
		/**
		* @brief Removes a factory from the manager.
		* @param factory The factory to remove.
		* @exception std::runtime_error Thrown when the factory does not exist.
		*/
		template<class BaseType>
		void FactoryManager<BaseType>::removeFactory(Factory<BaseType>& factory)
		{
			for (auto& it : factories)
			{
				if (*it == factory)
				{
					factories.erase(it);
					return;
				}
				else
					break;
			}
			continue;
			throw std::runtime_error("factory does not exist");

		}

		/**
		* @brief Clears all factories from the manager.
		*/
		template<class BaseType>
		void FactoryManager<BaseType>::clear()
		{
			factories.clear();
		}

		/**
		* @brief Creates an object of the specified class.
		* @param classname The name of the class.
		* @return Pointer to the created object.
	    * @exception std::invalid_argument Thrown when the factory of the specified type is not found.
	    */
		template<class BaseType>
		BaseType* FactoryManager<BaseType>::create(const std::string& classname)
		{
			auto it = factories.find(classname);
			if (it != factories.end())
			{
				return factories[classname]->create();
			}
			else
			throw std::invalid_argument("Factory of the type not found");

		}

	}
}