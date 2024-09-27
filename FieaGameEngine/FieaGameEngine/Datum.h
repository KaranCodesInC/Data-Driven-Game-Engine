#pragma once

#include <string>
#include <stdexcept>
#include <glm/glm.hpp>
#include "RTTI.h"

namespace Fiea::GameEngine {

    class Scope; // Forward declaration of Scope class

    class Datum
    {
    public:
        enum class DatumType
        {
            Unknown,
            Integer,
            Float,
            Vector,
            Matrix,
            String,
            Table,
            Pointer
            // Add more types as needed
        };

        // probably for this implementation, you would want each of these to be a (dynamically allocated
        //  array, instead of having an array of DatumUnion... this would allow you to store your values
        //  as an array, like Vector
        union DatumUnion {
            void* Unknown = nullptr;
            int* Integer;
            float* Float;
            glm::vec4* Vector;
            glm::mat4* Matrix;
            std::string* String;
            Scope** Table;
            RTTI** Pointer;
        };

        // Default constructor
        Datum();

        // Constructor with external storage
        template <typename T>
        Datum(T* externalStorage, size_t size);

        // Destructor
        ~Datum();

        // Copy constructor
        Datum(const Datum& other);

        // Move constructor
        Datum(Datum&& other) noexcept;

        // Copy assignment operator
        Datum& operator=(const Datum& other);

        // Move assignment operator
        Datum& operator=(Datum&& other) noexcept;

        // Type accessor
        DatumType Type() const;

        // Comparison operators
        bool operator==(const Datum& other) const;
        bool operator!=(const Datum& other) const;

        // Move semantics
        Datum& operator=(int&& value);
        Datum& operator=(float&& value);
        Datum& operator=(glm::vec4&& value);
        Datum& operator=(glm::mat4&& value);
        Datum& operator=(std::string&& value);

        // Size/Count accessor
        size_t Size() const;
        size_t Capacity() const;

        // Push, Pop
        void PushBack(const int value);
        void PushBack(const float value);
        void PushBack(const glm::vec4& value);
        void PushBack(const glm::mat4& value);
        void PushBack(const std::string value);
        void PushBack(Scope* value);

        void PushBack(RTTI* value);
  
        void PopBack();

        // SetSize/Resize
        void Resize(size_t newSize);
        
        void SetType(DatumType type);

        Scope& operator[](std::uint32_t index);

        // Clear the array without shrinking the capacity
        void Clear();

        // Get item directly by reference
        int& GetInt(std::size_t index) const;
        float& GetFloat(std::size_t index) const;
        const glm::vec4& GetVector(std::size_t index) const;
        const glm::mat4& GetMatrix(std::size_t index) const;
        const std::string& GetString(std::size_t index) const;
        
        //Get for scope
        Scope* GetScope(size_t index) const;
        
        //Get item as string reference
        std::string GetIntAsString(std::size_t index) const;
        std::string GetFloatAsString(std::size_t index) const;
        std::string GetMatrixAsString(std::size_t index) const;
        std::string GetVectorAsString(std::size_t index) const;

        //Set for scope
        void SetScope(Scope* value, size_t index);
        void Assign(const int& value, const size_t idx);
        void Assign(const float& value, const size_t idx);
        void Assign(const glm::vec4& value, const size_t idx);
        void Assign(const glm::mat4& value, const size_t idx);
        void Assign(const std::string& value, const size_t idx);

        void SetStorage(int* externalArray, size_t size);
        void SetStorage(float* externalArray, size_t size);
        void SetStorage(glm::vec4* externalArray, size_t size);
        void SetStorage(glm::mat4* externalArray, size_t size);
        void SetStorage(std::string* externalArray, size_t size);

        void SetStorage(Scope** externalArray, size_t size);

        void SetStorage(RTTI** externalArray, size_t size);
        
        void RemoveAt(size_t index);
        std::string mattostring(const glm::mat4& matrix)const;

    private:

        DatumType _type;

        DatumUnion mData;

        size_t mSize = 0;
        size_t mCapacity = 0;

        std::string vectostring(const glm::vec4& vector)const;

        bool isExternal = false;

        void reserve(size_t capacity);
    };

} // namespace Fiea::GameEngine
#include "Datum.inl"