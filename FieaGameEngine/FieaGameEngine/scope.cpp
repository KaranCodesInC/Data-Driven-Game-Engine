#pragma once
#include "pch.h"
#include "scope.h"
#include "Datum.h"

using namespace std;

namespace Fiea::GameEngine {

    RTTI_DEFINITIONS(Scope);

    // Constructor
    /*
    * @brief constructor
    * @param starting capacity of the scope
    * */
    Scope::Scope(std::size_t initialCapacity)
        : mEntries(initialCapacity), mParent(nullptr) 
    {
        mEntryPointers.reserve(initialCapacity);

        mEntries.reserve(initialCapacity);
    }

    // Destructor
    /*
    * @brief destructor
    */
    Scope::~Scope()
    {
        Clear();
    }

    // Copy constructor
    /*
    * @brief copy constructor
    */
    Scope::Scope(const Scope& other)
        : mParent(other.mParent)
    {
        for (const auto& entry : other.mEntries)
        {
            Datum* Dp = &mEntries[entry.first];
            if (entry.second.Type() == Datum::DatumType::Table)
            {
                for (size_t i = 0; i < entry.second.Size(); ++i)
                {
                    Scope clonedScope(*entry.second.GetScope(i));
                    clonedScope.mParent = this;
                    Dp->PushBack(&clonedScope);
                }
            }
            else
            {
                *Dp = entry.second; //using Datum's Copy Assignment 
            }
            mEntryPointers.push_back({ entry.first, Dp });
        };
    }


   /**
 * @brief Move constructor Scope class.
 * @param other The Scope object to move from.
 */
   Scope::Scope(Scope&& other) noexcept
       : mEntries(std::move(other.mEntries)), // Move the entries from the source to the new object
       mParent(other.mParent)
   {
       // Transfer ownership of the entry pointers
       mEntryPointers = std::move(other.mEntryPointers);

       // Set the parent of the moved-from object to null
       other.mParent = nullptr;
   }

   /**
    * @brief Move assignment operator for Scope class.
    * @param other The Scope object to move from.
    * @return Reference to the modified Scope object.
    */
   Scope& Scope::operator=(Scope&& other) noexcept
   {
       // Check for self-assignment
       if (this != &other)
       {
           // Clear the current object to release its resources
           Clear();

           // Move the entries from the source to the target object
           mEntries = std::move(other.mEntries);

           // Transfer ownership of the entry pointers
           mEntryPointers = std::move(other.mEntryPointers);

           // Copy the parent pointer from the source object
           mParent = other.mParent;

           // Set the parent of the moved-from object to null
           other.mParent = nullptr;
       }
       return *this;
   }



    //Copy Assignment operator
   /*
   * @brief copy assignment operator
   * @param other scope/source scope
   */
   Scope& Scope::operator=(const Scope& other)
   {
       if (this != &other)
       {
           Clear();
           for (const auto& entry : other.mEntries)
           {
               Datum* Dp = &mEntries[entry.first];
               if (entry.second.Type() == Datum::DatumType::Table)
               {
                   for (size_t i = 0; i < entry.second.Size(); ++i)
                   {
                       Scope clonedScope(*entry.second.GetScope(i));
                       clonedScope.mParent = this;
                       Dp->PushBack(&clonedScope);
                   }
               }
               else
               {
                   *Dp = entry.second; //using Datum's Copy Assignment 
               }
               mEntryPointers.push_back({ entry.first, Dp });
           };
       }
       return *this;
   }

    /**
     * @brief Finds a datum by name in the current scope.
     *
     * This method searches for a datum by name in the current scope.
     *
     * @param name The name of the datum to find.
     * @return A pointer to the found datum, or nullptr if the datum is not found.
     */

    Datum* Scope::Find(const std::string& name)
    {
        auto it = mEntries.find(name);
        return (it != mEntries.end()) ? &(it->second) : nullptr; //end() gives the position past the last element
    }

    /**
 * @brief Searches for a datum by name in the current scope or its ancestors.
 *
 * This method searches for a datum by name in the current scope. If the datum
 * is not found in the current scope, it recursively searches in the parent scope
 * until the datum is found or until there are no more parent scopes.
 *
 * @param name The name of the datum to search for.
 * @param foundScope Output parameter to store the scope where the datum was found.
 *                   This parameter is set to nullptr if the datum is not found.
 * @return A pointer to the found datum, or nullptr if the datum is not found.
 */

    Datum* Scope::Search(const std::string& name, Scope** foundScope)
    {
        Datum* ptr = Find(name);

        if (ptr == nullptr)
        {
            return mParent->Search(name, foundScope);
        }
        else
        {
            return ptr;
        }
    }

    const Datum* Scope::Search(const std::string& name, Scope** foundScope) const
    {
        // Call the non-const Search method and cast constness
        return const_cast<Scope*>(this)->Search(name, foundScope);
    }

    /**
     * @brief Clears the scope and all its nested scopes.
     *
     * This method removes all entries from the scope, including any nested scopes. It also
     * resets the parent reference of nested scopes to nullptr and deletes them from memory.
     */
    void Scope::Clear()
    {
        if (mParent != nullptr)
        {
            mParent->Orphan(this);
        }
        for (auto& entry : mEntries)
        {
            // Check if the entry is a nested Scope
            if (entry.second.Type() == Datum::DatumType::Table)
            {
                for (std::size_t i = 0; i < entry.second.Size(); ++i)
                {
                    Scope* nestedScope = entry.second.GetScope(i);
                    nestedScope->Clear();
                    nestedScope->mParent = nullptr;
                    delete nestedScope;
                }
            }
        }
        

        mEntries.clear();
        mEntryPointers.clear();
        // Orphan itself from the parent
    }

    /**
     * @brief Equality comparison operator.
     *
     * Compares two Scope objects for equality. Two scopes are considered equal if they have
     * the same number of entries and each entry in one scope has a corresponding entry in
     * the other scope with the same key and value. Nested scopes are recursively compared.
     *
     * @param other The scope to compare with.
     * @return True if the scopes are equal, false otherwise.
     */
    bool Scope::operator==(const Scope& other) const
    {
        // Check if the sizes of the scopes match
        if (mEntries.size() != other.mEntries.size())
        {
            return false;
        }

        // Check each entry in this scope
        for (size_t i = 0; i < mEntryPointers.size(); ++i)
        {
            if (mEntryPointers[i].first != other.mEntryPointers[i].first)
            {
                return false;
            }
            if (*mEntryPointers[i].second != *other.mEntryPointers[i].second)
            {
                return false;
            }
        }
        return true;
    }

    /**
    * @brief Inequality comparison operator.
    *
    * Compares two Scope objects for inequality. Two scopes are considered unequal if they
    * have different numbers of entries or if any entry in one scope does not have a
    * corresponding entry in the other scope with the same key and value. Nested scopes
    * are recursively compared.
    *
    * @param other The scope to compare with.
    * @return True if the scopes are unequal, false if they are equal.
    */
    bool Scope::operator!=(const Scope& other) const
    {
        // Return the negation of the equality comparison result
        return !(*this == other);
    }

    /**
    * @brief Get the parent scope.
    *
    * Returns a pointer to the parent scope of the current scope. If the current scope
    * does not have a parent (i.e., it is the root scope), nullptr is returned.
    *
    * @return A pointer to the parent scope, or nullptr if the current scope is the root scope.
    */
    Scope* Scope::GetParent() const
    {
        return mParent;
    }

    /**
    * @brief Access or create a Datum associated with the provided name.
    *
    * This operator allows access to a Datum associated with the provided name within
    * the current scope. If a Datum with the provided name does not exist, it will be
    * created and returned. If a Datum with the provided name already exists, it will
    * be returned.
    *
    * @param name The name associated with the Datum to access or create.
    * @return A reference to the Datum associated with the provided name.
    */
    Datum& Scope::operator[](const std::string& name)
    {
        return Append(name);
    }

    /**
     * @brief Access the Datum at the specified index.
     *
     * This operator allows access to the Datum stored at the specified index within
     * the scope's internal array of entry pointers. If the provided index is out of
     * bounds, an out_of_range exception is thrown.
     *
     * @param index The index of the Datum to access.
     * @return A reference to the Datum at the specified index.
     * @throws std::out_of_range if the index is out of bounds.
     */
    Datum& Scope::operator[](std::size_t index)
    {
        if (index < mEntryPointers.size())
        {
            // Return the reference to the Datum at the specified index
            return *mEntryPointers[index].second;
        }
        else
        {
            // Handle out-of-bounds access (you might want to throw an exception or handle it differently)
            throw std::out_of_range("Index out of bounds");
        }
    }

 /**
 * @brief Append a Datum with the specified name to the scope.
 *
 * This method adds a new Datum to the scope with the given name if it does not
 * already exist. If a Datum with the same name already exists, it returns a
 * reference to the existing Datum.
 *
 * @param name The name of the Datum to append.
 * @return A reference to the newly created or existing Datum.
 */
    Datum& Scope::Append(const std::string& name)
    {
        // Check if the name already exists
        auto it = mEntries.find(name);
        if (it != mEntries.end())
        {
            // If it exists, return the existing Datum
            return it->second;
        }

        // If it doesn't exist, create a new Datum and store it in the unordered_map
        //Datum D;
        mEntries[name] = Datum();
        mEntryPointers.push_back({name, &mEntries[name]});

        // Return a reference to the newly created Datum
        return mEntries[name];
    }
/**
 * @brief Append a new Scope under the specified name to the current Scope.
 *
 * This method creates a new Scope and adds it as a child to the Datum with the
 * specified name in the current Scope. If the Datum does not exist, it creates
 * a new one. If the Datum already exists and is not of type DatumType::Table,
 * it throws a runtime_error.
 *
 * @param name The name under which to append the new Scope.
 * @return A reference to the newly appended Scope.
 * @throws std::runtime_error If the existing Datum is not of type DatumType::Table.
 */
    Scope& Scope::AppendScope(const std::string& name)
    {
        Datum& existingDatum = Append(name);
        existingDatum.SetType(Datum::DatumType::Table);
        if (existingDatum.Type() != Datum::DatumType::Table)
        {
            throw std::runtime_error("BROOOOOOOO wrong type");
        }
        else
        {
            // Append a new Scope to the existing Datum
            Scope* newScope = new Scope();
            newScope->mParent = this; // Set the parent reference
            existingDatum.PushBack(newScope);
            _scopeP.push_back(newScope);
            return *newScope;
        }
    }


/**
 * @brief Get the number of entries in the current Scope.
 *
 * This method returns the number of entries (i.e., Datum objects) in the current Scope.
 *
 * @return The number of entries in the current Scope.
 */

    size_t Scope::ScopeSize() const
    {
        return mEntries.size();
    }

 /**
 * @brief Find the Datum containing the given nested Scope within the current Scope.
 *
 * This method searches for the Datum object that contains the given nested Scope within the current Scope.
 * If found, it updates the provided index parameter with the index of the nested Scope within the Datum.
 *
 * @param scope A pointer to the nested Scope to search for.
 * @param index [out] Upon successful finding of the nested Scope, this parameter is updated with the index of the nested Scope within its containing Datum.
 *
 * @return A pointer to the Datum containing the nested Scope if found, nullptr otherwise.
 */
    Datum* Scope::FindContainedScope(Scope* scope, std::size_t& index) 
    {
        for (auto& entry : mEntries)
        {
            if (entry.second.Type() == Datum::DatumType::Table)
            {
                for (size_t i = 0; i < entry.second.Size(); ++i)
                {
                    if (entry.second.GetScope(i) == scope)
                        index = i;
                        return &entry.second;
                }
            }
        }
        return nullptr;
    }

/**
 * @brief Orphan a child Scope from the current Scope.
 *
 * This method finds the Datum containing the specified child Scope within the current Scope.
 * If found, it removes the child Scope from the Datum.
 *
 * @param child A pointer to the child Scope to orphan.
 */

    void Scope::Orphan(Scope* child)
    {
        size_t i = 0;
        Datum* foundDatum = FindContainedScope(child, i);
        if (foundDatum)
            foundDatum->RemoveAt(i);
    }

/**
 * @brief Adopt a child Scope under the current Scope with a specified key.
 *
 * This method sets the specified child Scope's parent to the current Scope.
 * If the child already has a parent, it is removed from its current parent before adoption.
 * The child is added to the current Scope's entries with the specified key.
 *
 * @param child A reference to the child Scope to be adopted.
 * @param key The key under which the child Scope will be stored.
 */
    void Scope::Adopt(Scope& child, const std::string& key)
    {
        if (&child == this)
        {
            // Trying to adopt itself, do nothing
            return;
        }

        // Check if the child already has a parent
        if (child.mParent != nullptr)
        {
            // Remove the child from its current parent
            child.mParent->Orphan(&child);
        }

        // Set the parent of the child to this Scope
        child.mParent = this;
        Datum& datum = Append(key);
        datum.PushBack(&child);
    }

    void Scope::Adopt(Scope*&& child, const std::string& key)
    {
        if (child == this)
        {
            return;
        }

        if (child->mParent != nullptr)
        {
            child->mParent->Orphan(child);
        }

        child->mParent = this;
        Datum& datum = Append(key);
        if (datum.Type() == Datum::DatumType::Unknown || datum.Type() == Datum::DatumType::Table)
        {
            Scope* scope = std::move(child);
            datum.PushBack(scope);
            scope->mParent = this;
        }
        else
        {
            throw std::runtime_error("Datum type isn't scope");
        }
    }
    
/**
 * @brief Check if the current Scope is an ancestor of a potential descendant Scope.
 *
 * This method traverses the parent chain of the potential descendant to check if
 * any of its ancestors are equal to the current Scope. If an ancestor is found,
 * it means that the current Scope is an ancestor of the potential descendant.
 *
 * @param PotentialDescendant The potential descendant Scope to check against.
 * @return True if the current Scope is an ancestor of the potential descendant, false otherwise.
 */
    bool Scope::IsAncestorOf(Scope& PotentialDescendant) const {
        // Get the parent of the potential descendant
        const Scope* currentParent = PotentialDescendant.GetParent();

        // Traverse the parent chain
        while (currentParent != nullptr)
        {
            // Check if the current parent is equal to this scope
            if (currentParent == this)
            {
                return true; // Found an ancestor
            }

            // Move up to the next parent
            currentParent = currentParent->GetParent();
        }

        // Reached the root without finding this scope
        return false;
    }

/**
 * @brief Check if the current Scope is a descendant of a potential parent Scope.
 *
 * This method traverses the parent chain of the current Scope to check if any of its
 * ancestors are equal to the potential parent Scope. If a matching ancestor is found,
 * it means that the current Scope is a descendant of the potential parent.
 *
 * @param PotentialParent The potential parent Scope to check against.
 * @return True if the current Scope is a descendant of the potential parent, false otherwise.
 */
    bool Scope::IsDescendantOf(Scope& PotentialParent) const
    {
        Scope* currentParent = GetParent();

        // Traverse the parent chain
        while (currentParent != nullptr)
        {
            // Check if the current parent is equal to the potential ancestor
            if (currentParent == &PotentialParent)
            {
                return true; // Found a descendant
            }

            // Move up to the next parent
            currentParent = currentParent->GetParent();
        }

        // Reached the root without finding the potential ancestor
        return false;
    }

    void Scope::OrderElements()
    {
        for (auto element : mEntries)
        {
            if (element.second.Type() == Datum::DatumType::Table)
            {
                mEntryPointers.push_back({ element.first, &element.second });
            }
        }
    }

}



