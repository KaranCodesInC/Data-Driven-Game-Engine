#pragma once

#include "pch.h"
#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include "Datum.h"
#include "RTTI.h"
#include <memory>

namespace Fiea::GameEngine {

    class Scope : public RTTI
    {
        RTTI_DECLARATIONS(Scope, RTTI); //declare RTTI for scope

    public:
        // Constructor which takes an optional unsigned integer
        explicit Scope(std::size_t initialCapacity = 0);

        // Destructor
        ~Scope();

        // Copy constructor
        Scope(const Scope& other);

        // Assignment operator
        Scope& operator=(const Scope& other);

        Scope(Scope&& other) noexcept;

        Scope& operator=(Scope&& other) noexcept;

        // Find, which takes a constant string and returns the address of a Datum
        Datum* Find(const std::string& name);

        // Search, which takes a constant string and the address of a Scope double pointer variable
        Datum* Search(const std::string& name, Scope** foundScope);

        const Datum* Search(const std::string& name, Scope** foundScope) const;

        // Append, which takes a constant string and returns a reference to a Datum
        Datum& Append(const std::string& name);

        // AppendScope, which takes a constant string and returns a reference to Scope
        Scope& AppendScope(const std::string& name);

        // Adopt, which takes a reference to a Scope
        void Adopt(Scope& child, const std::string& key);

        // GetParent, which returns the address of the Scope which contains this one
        Scope* GetParent() const;
        
        size_t ScopeSize() const;

        size_t ScopeCapacity() const;

        // Check if the current scope is an ancestor of the given scope
        bool IsAncestorOf(Scope& PotentialDescendant) const;

        // Check if the current scope is a descendant of the given scope
        bool IsDescendantOf(Scope& PotentialAncestor) const;

        // operator[]
        Datum& operator[](const std::string& name); //overload for constant string, wraps Append for syntactic convenience
        
        Datum& operator[](std::size_t index);       //overload for unsigned integer, returns a reference to a Datum at the given index
        
        // Const version of operator[]
        const Datum& operator[](std::size_t index) const;

        // operator== overload for comparing the contents of two Scope objects
        bool operator==(const Scope& other) const;

        // operator!= overload, returns the negation of operator==
        bool operator!=(const Scope& other) const;

        // Clear routine which deletes all memory allocated by this object
        void Clear();

        // Orphan routine that "orphans" children
        void Orphan(Scope* child);

        // Helper method for finding a contained Scope
        Datum* FindContainedScope(Scope* scope, std::size_t& index);

        virtual Scope* Clone() const { return new Scope(*this); }

        void OrderElements();

        void Adopt(Scope*&& child, const std::string& key);


    private:
        
        // HashMap entry type
        //using EntryType = std::pair<std::string, Datum>;

        // Vector of pointers to HashMap entries
        std::vector<std::pair<string, Datum*>> mEntryPointers;

        // Unordered map of key-value pairs
        std::unordered_map<std::string, Datum> mEntries;

        //Parent Scope
        Scope* mParent; 

        vector<Scope*> _scopeP;

        // Clone method for deep copies
        Scope Clone(const Scope& other);
    };

} // namespace Fiea::GameEngine
