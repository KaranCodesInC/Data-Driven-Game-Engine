
#pragma once
#include "Scope.h"
#include "RTTI.h"
#include <vector>
#include <string>
#include "signature.h"
#include "Foo.h"

namespace Fiea::GameEngine {

    class Attributed : public Scope{
        RTTI_DECLARATIONS(Attributed, Scope);
    
    public:

        // Destructor
        virtual ~Attributed();

        // Copy constructor
        Attributed(const Attributed& other);

        // Move constructor
        Attributed(Attributed&& other) noexcept;

        // Copy assignment operator
        Attributed& operator=(const Attributed& other);

        // Move assignment operator
        Attributed& operator=(Attributed&& other) noexcept;

        // Populate the Scope with prescribed members
        void PopulatePrescribedMembers();

        virtual Attributed* Clone() const = 0;

        // IsAttribute method
       virtual bool IsAttribute(const std::string& attributeName);

        // IsPrescribedAttribute method
       virtual bool IsPrescribedAttribute(const std::string& attributeName) const;

        // IsAuxiliaryAttribute method
       virtual bool IsAuxiliaryAttribute(const std::string& attributeName) const;

        // AppendAuxiliaryAttribute method
       virtual Datum& AppendAuxiliaryAttribute(const std::string& attributeName);

    protected:
        
        explicit Attributed(size_t id);
        //Attributed(size_t id);

    private:

        // Vector to store prescribed attributes
        static std::vector<std::pair<std::string, Datum>> mPrescribedAttributes;

        vector<std::pair<std::string, Datum>> AuxiliaryAttributes;

        /*static std::vector<signature> Signatures();*/

        std::vector<signature> sigs;

        // Scope to hold the attributes
        Scope mScope;

        size_t TypeId;
        
    };
}
