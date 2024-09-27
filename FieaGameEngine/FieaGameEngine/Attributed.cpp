#include "pch.h"
#include "Datum.h"
#include "scope.h"
#include "Attributed.h"
#include "signature.h"
#include "TypeManager.h"

namespace Fiea::GameEngine {
    RTTI_DEFINITIONS(Attributed);
    //std::vector<std::pair<std::string, Datum>> Attributed::mPrescribedAttributes;

    /**
     * @brief Returns the signatures of the prescribed attributes.
     * @return The signatures of the prescribed attributes.
     */

    // Constructor

    /**
     * @class Attributed
     * @brief Represents an attributed class inheriting from Scope.
     */
    Attributed::Attributed(size_t id)
    {
        sigs = TypeManager::instance->GetSignature(id);
        RTTI* p = this;
        Append("Self").SetStorage(&p, (size_t)1);
        PopulatePrescribedMembers();
    }

   /* Attributed::Attributed() 
    {
        Datum& thisAttribute = AppendAuxiliaryAttribute("this");
        RTTI* newP = this;
        thisAttribute.SetStorage(&newP, size_t(1));
    }*/

    Attributed::~Attributed() {
    }

    // Copy constructor
    /**
     * @brief Default constructor.
     */
    Attributed::Attributed(const Attributed& other)
    {
        sigs = other.sigs;
        //AuxiliaryAttributes = other.AuxiliaryAttributes;
        for (auto& aux : other.AuxiliaryAttributes)
        {
            AppendAuxiliaryAttribute(aux.first) = const_cast<Attributed&>(other).Append(aux.first);
        }
        RTTI* p = this;
        Append("Self").SetStorage(&p, (size_t)1);
        PopulatePrescribedMembers();
    }


   /**
    * @brief Move constructor.
    * @param other The Attributed object to move.
    */
    Attributed::Attributed(Attributed&& other) noexcept
    {
        sigs = other.sigs;
        //AuxiliaryAttributes = other.AuxiliaryAttributes;
        for (auto& aux : other.AuxiliaryAttributes)
        {
            AppendAuxiliaryAttribute(aux.first) = const_cast<Attributed&>(other).Append(aux.first);
        }
        RTTI* p = this;
        Append("Self").SetStorage(&p, (size_t)1);
        PopulatePrescribedMembers();
        
    }

    /**
     * @brief Copy assignment operator.
     * @param other The Attributed object to copy.
     * @return Reference to the modified Attributed object.
     */
    Attributed& Attributed::operator=(const Attributed& other)
    {
        if (this != &other)
        {
            sigs = other.sigs;
            //AuxiliaryAttributes = other.AuxiliaryAttributes;
            for (auto& aux : other.AuxiliaryAttributes)
            {
                AppendAuxiliaryAttribute(aux.first) = const_cast<Attributed&>(other).Append(aux.first);
            }
            RTTI* p = this;
            Append("Self").SetStorage(&p, (size_t)1);
            PopulatePrescribedMembers();
        }
        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other The Attributed object to move.
     * @return Reference to the modified Attributed object.
     */
    Attributed& Attributed::operator=(Attributed&& other) noexcept
    {
        if (this != &other)
        {
            // Clear current scope
            //mScope.Clear();

            // Move the base Scope part
            Scope::operator=(std::move(other));

            // Move the prescribed attributes
            sigs = std::move(other.sigs);
        }
        return *this;
    }

    /**
     * @brief Populates prescribed members in the scope.
     */
    void Attributed::PopulatePrescribedMembers()
    {
        vector<signature> PrescriptionList = sigs;
        for (auto s : PrescriptionList) {
            char* ptr = reinterpret_cast<char*>(this);
            ptr += s.Offset;

            //size_t ptr = reinterpret_cast<size_t>(this);
            //ptr += s.Offset;

            switch (s._type) {
            case Datum::DatumType::Integer:
                Append(s.name).SetStorage(reinterpret_cast<int*>(ptr), s.size);
                break;
            case Datum::DatumType::Float:
                Append(s.name).SetStorage(reinterpret_cast<float*>(ptr), s.size);
                break;
            case Datum::DatumType::Vector:
                Append(s.name).SetStorage(reinterpret_cast<glm::vec4*>(ptr), s.size);
                break;
            case Datum::DatumType::Matrix:
                Append(s.name).SetStorage(reinterpret_cast<glm::mat4*>(ptr), s.size);
                break;
            case Datum::DatumType::String:
                Append(s.name).SetStorage(reinterpret_cast<std::string*>(ptr), s.size);
                break;
            default:
                break;
            }
        }
    }


    /**
     * @brief Checks if an attribute exists.
     * @param attributeName The name of the attribute.
     * @return True if the attribute exists, false otherwise.
     */
    bool Attributed::IsAttribute(const std::string& attributeName)
    {
        return IsPrescribedAttribute(attributeName) || IsAuxiliaryAttribute(attributeName);
    }

    /**
     * @brief Checks if an auxiliary attribute exists.
     * @param attributeName The name of the attribute.
     * @return True if the auxiliary attribute exists, false otherwise.
     */
    bool Attributed::IsAuxiliaryAttribute(const std::string& attributeName) const
    {
        for (const auto& attribute : AuxiliaryAttributes)
        {
            if (attribute.first == attributeName)
            {
                return true;
            }
        }
        return false;
    }


    /**
     * @brief Checks if a prescribed attribute exists.
     * @param attributeName The name of the attribute.
     * @return True if the prescribed attribute exists, false otherwise.
     */
    bool Attributed::IsPrescribedAttribute(const std::string& attributeName) const
    {
        for (const auto& attribute : sigs)
        {
            if (attribute.name == attributeName)
            {
                return true;
            }
        }
        return false;
    }


    /**
     * @brief Appends an auxiliary attribute.
     * @param attributeName The name of the attribute.
     * @return Reference to the appended Datum.
     */
     Datum& Attributed::AppendAuxiliaryAttribute(const std::string& attributeName)
     {
        if (IsPrescribedAttribute(attributeName))
        {
            throw std::invalid_argument("Cannot append a prescribed attribute as auxiliary.");
        }
            
        Datum& attribute = Append(attributeName);
        AuxiliaryAttributes.push_back({ attributeName, attribute});
        return attribute;
     }

}
