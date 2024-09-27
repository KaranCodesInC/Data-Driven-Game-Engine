#include "pch.h"
#include "Datum.h"
#include <assert.h>
#include <glm/glm.hpp>
#include "scope.h"

using namespace std;

namespace Fiea::GameEngine
{ 
    //Default Constructor
    Datum::Datum() : _type(DatumType::Unknown), mSize(0), mCapacity(0), isExternal(false)
    {
    }

    // Copy Constructor
    Datum::Datum(const Datum& other) : _type(other._type), mSize(other.mSize), mCapacity(other.mSize), isExternal(false)
    {
            switch (_type)
            {
            case DatumType::Integer:
                mData.Integer = new int[mSize];
                for (size_t i = 0; i < mSize; ++i)
                {
                    new(&mData.Integer[i])int(other.mData.Integer[i]);
                }
                break;
            case DatumType::Float:
                mData.Float = new float[mSize];
                for (size_t i = 0; i < mSize; ++i)
                {
                    new(&mData.Float[i])float(other.mData.Float[i]);
                }
                break;
            case DatumType::Vector:
                mData.Vector = new glm::vec4[mSize];
                for (size_t i = 0; i < mSize; ++i)
                {
                    new(&mData.Vector[i])glm::vec4(other.mData.Vector[i]);
                }
                break;
            case DatumType::Matrix:
                mData.Matrix = new glm::mat4[mSize];
                for (size_t i = 0; i < mSize; ++i)
                {
                    new(&mData.Matrix[i])glm::mat4(other.mData.Matrix[i]);
                }
                break;
            case DatumType::String:
                mData.String = new std::string[mSize];
                for (size_t i = 0; i < mSize; ++i)
                {
                    mData.String[i] = std::string(other.mData.String[i]);
                }
                break;
            case DatumType::Table:
                mData.Table = new Scope * [mSize];
                for (size_t i = 0; i < mSize; ++i)
                {
                    new(mData.Table[i])Scope* (other.mData.Table[i]);
                }
                break;
                case DatumType::Pointer:
                    mData.Pointer = new RTTI * [mSize];
                    for (size_t i = 0; i < mSize; ++i)
                    {
                        new(mData.Pointer[i])RTTI* (other.mData.Pointer[i]);
                    }
                    break;
            default:
                break;
            }
    }

    //Destructor
    Datum::~Datum()
    {
        Clear();
    }

    //Clear all the values in the datum
    void Datum::Clear()
    {
        if (isExternal == false)
        {
            // Clean up dynamically allocated memory
            switch (_type)
            {
            case DatumType::Integer:
                delete mData.Integer;
                mData.Integer = nullptr;
                break;
            case DatumType::Float:
                delete mData.Float;
                mData.Float = nullptr;
                break;
            case DatumType::Vector:
                delete mData.Vector;
                mData.Vector = nullptr;
                break;
            case DatumType::Matrix:
                delete mData.Matrix;
                mData.Matrix = nullptr;
                break;
            case DatumType::String:
                for (size_t i = 0; i < mSize; i++)
                {
                    //mData.String[i].~basic_string();
                    //delete mData.String[i];
                    //delete mData.String;
                }
                delete[] mData.String;
                mData.String = nullptr;
                break;
            case DatumType::Pointer:
                delete mData.Pointer;
                mData.Pointer = nullptr;
                break;
            case DatumType::Table:
                delete mData.Table;
                mData.Table = nullptr;
                break;
            default:
                // Unsupported type
                break;
            }

        }

        // Reset size and type
        mSize = 0;
        mCapacity = 0;
        _type = DatumType::Unknown;
    }

    //Reserve
    void Datum::reserve(size_t capacity)
    {
        if (capacity > mCapacity)
        {
            DatumUnion newData;

            // Copy existing data to the new memory
            switch (_type)
            {
            case DatumType::Integer:
                newData.Integer = new int[capacity];
                for (size_t i = 0; i < mSize; ++i)
                {
                    newData.Integer[i] = mData.Integer[i];
                }
                delete mData.Integer;
                mData.Integer = newData.Integer;
                break;
            case DatumType::Float:
                newData.Float  = new float[capacity];
                for (size_t i = 0; i < mSize; ++i)
                {
                   newData.Float[i] = mData.Float[i];
                }
                delete mData.Float;
                mData.Float = newData.Float;
                break;
            case DatumType::Vector:
                newData.Vector = new glm::vec4[capacity];
                for (size_t i = 0; i < mSize; ++i)
                {
                   newData.Vector[i] = mData.Vector[i];
                }
                delete mData.Vector;
                mData.Vector = newData.Vector;
                break;
            case DatumType::Matrix:
                newData.Matrix = new glm::mat4[capacity];
                for (size_t i = 0; i < mSize; ++i)
                {
                    newData.Matrix[i] = mData.Matrix[i];
                }
                delete mData.Matrix;
                mData.Matrix = newData.Matrix;
                break;
            case DatumType::String:
                newData.String = new std::string[capacity];
                for (size_t i = 0; i < mSize; ++i)
                {
                   newData.String[i] = mData.String[i];
                }
                delete[] mData.String;
                mData.String = newData.String;
                break;
            case DatumType::Table:
                newData.Table = new Scope * [capacity];
                for (size_t i = 0; i < mSize; ++i)
                {
                    newData.Table[i] = mData.Table[i];
                }
                delete mData.Table;
                mData.Table = newData.Table;
                break;
            case DatumType::Pointer:
                newData.Pointer = new RTTI * [capacity];
                for (size_t i = 0; i < mSize; ++i)
                {
                    newData.Pointer[i] = mData.Pointer[i];
                }
                delete mData.Pointer;
                mData.Pointer = newData.Pointer;
                break;
            default:
                // Unsupported type
                break;
            }

            // Update data pointer to the new memory and capacity
            //mData = newData;
            mCapacity = capacity;
        }
    }

    //PushBack Overloads
    void Datum::PushBack(const int value)
    {
        if (_type == DatumType::Unknown)
        {
            _type = DatumType::Integer;
        }
        if (_type != DatumType::Integer)
        {
            throw std::invalid_argument("Invalid assignment type for Datum");
        }
        if (mSize == mCapacity)
        {
            reserve(mCapacity == 0 ? 1 : mCapacity * 2);
        }
        //new(&mData.Integer[mSize])DatumUnion();
        new(&mData.Integer[mSize])int(value);
        ++mSize;
    }

    void Datum::PushBack(const float value)
    {
        if (_type == DatumType::Unknown)
        {
            _type = DatumType::Float;
        }
        if (_type != DatumType::Float)
        {
            throw std::invalid_argument("Invalid assignment type for Datum");
        }
        if (mSize == mCapacity)
        {
            reserve(mCapacity == 0 ? 1 : mCapacity * 2);
        }
        new(&mData.Float[mSize])float(value);
        ++mSize;
    }

    void Datum::PushBack(const glm::vec4& value)
    {
        if (_type == DatumType::Unknown)
        {
            _type = DatumType::Vector;
        }
        if (_type != DatumType::Vector)
        {
            throw std::invalid_argument("Invalid assignment type for Datum");
        }
        if (mSize == mCapacity)
        {
            reserve(mCapacity == 0 ? 1 : mCapacity * 2);
        }
        new(&mData.Vector[mSize])glm::vec4(value);
        ++mSize;
    }

    void Datum::PushBack(const glm::mat4& value)
    {
        if (_type == DatumType::Unknown)
        {
            _type = DatumType::Matrix;
        }
        if (_type != DatumType::Matrix)
        {
            throw std::invalid_argument("Invalid assignment type for Datum");
        }
        if (mSize == mCapacity)
        {
            reserve(mCapacity == 0 ? 1 : mCapacity * 2);
        }
        new(&mData.Matrix[mSize])glm::mat4(value);
        ++mSize;
    }

    void Datum::PushBack(const std::string value)
    {
        if (_type == DatumType::Unknown)
        {
            _type = DatumType::String;
        }
        if (_type != DatumType::String)
        {
            throw std::invalid_argument("Invalid assignment type for Datum");
        }
        if (mSize == mCapacity)
        {
            reserve(mCapacity == 0 ? 1 : mCapacity * 2);
        }
        // Placement new to construct the string in the allocated memory
        mData.String[mSize] = std::string(value);
        ++mSize;
    }

    void Datum::PushBack(Scope* value)
    {
        if (_type == DatumType::Unknown)
        {
            _type = DatumType::Table;
        }
        if (_type != DatumType::Table)
        {
            throw std::invalid_argument("Invalid assignment type for Datum");
        }
        if (mSize == mCapacity)
        {
            reserve(mCapacity == 0 ? 1 : mCapacity * 2);
        }
        new(&mData.Table[mSize])Scope*(value);
        ++mSize;
    }

    void Datum::PushBack(RTTI* value)
    {
        if (_type == DatumType::Unknown)
        {
            _type = DatumType::Pointer;
        }
        if (_type != DatumType::Pointer)
        {
            throw std::invalid_argument("Invalid assignment type for Datum");
        }
        if (mSize == mCapacity)
        {
            reserve(mCapacity == 0 ? 1 : mCapacity * 2);
        }
        new(&mData.Pointer[mSize])RTTI*(value);
        ++mSize;
    }

    // Move constructor
    Datum::Datum(Datum&& other) noexcept
        : _type(other._type), mSize(std::move(other.mSize)), mCapacity(std::move(other.mCapacity))
    {
            switch (_type)
            {
            case DatumType::Integer:
                // Move ownership of the array of integers
                mData.Integer = other.mData.Integer;
                other.mData.Integer = nullptr;
                break;
            case DatumType::Float:
                // Move ownership of the array of floats
                mData.Float = other.mData.Float;
                other.mData.Float = nullptr;
                break;
            case DatumType::Vector:
                // Move ownership of the array of vectors
                mData.Vector = other.mData.Vector;
                other.mData.Vector = nullptr;
                break;
            case DatumType::Matrix:
                // Move ownership of the array of matrices
                mData.Matrix = other.mData.Matrix;
                other.mData.Matrix = nullptr;
                break;
            case DatumType::String:
                // Move ownership of the array of strings
                mData.String = other.mData.String;
                other.mData.String = nullptr;
                break;
            case DatumType::Table:
                // Move ownership of the array of strings
                mData.Table = other.mData.Table;
                other.mData.Table = nullptr;
                break;
            case DatumType::Pointer:
                // Move ownership of the array of strings
                mData.Pointer = other.mData.Pointer;
                other.mData.Pointer = nullptr;
                break;
            default:
                break;
            }
            // Reset the source Datum
            other._type = DatumType::Unknown;
            other.mSize = 0;
            other.mCapacity = 0;
    }

    // Move Assignment operator
    Datum& Datum::operator=(Datum&& other) noexcept {
        if (this != &other) {
            // Clear existing data
            Clear();
            // Move data and properties
            _type = other._type;
            mSize = std::move(other.mSize);
            mCapacity = std::move(other.mCapacity);
            isExternal = std::move(other.isExternal);
            switch (_type)
            {
            case DatumType::Integer:
                // Move ownership of the array of integers
                mData.Integer = other.mData.Integer;
                other.mData.Integer = nullptr;
                break;
            case DatumType::Float:
                // Move ownership of the array of floats
                mData.Float = other.mData.Float;
                other.mData.Float = nullptr;
                break;
            case DatumType::Vector:
                // Move ownership of the array of vectors
                mData.Vector = other.mData.Vector;
                other.mData.Vector = nullptr;
                break;
            case DatumType::Matrix:
                // Move ownership of the array of matrices
                mData.Matrix = other.mData.Matrix;
                other.mData.Matrix = nullptr;
                break;
            case DatumType::String:
                // Move ownership of the array of strings
                mData.String = other.mData.String;
                other.mData.String = nullptr;
                break;
            case DatumType::Table:
                // Move ownership of the array of strings
                mData.Table = other.mData.Table;
                other.mData.Table = nullptr;
                break;
            case DatumType::Pointer:
                // Move ownership of the array of strings
                mData.Pointer = other.mData.Pointer;
                other.mData.Pointer = nullptr;
                break;
            default:
                break;
            }
            // Reset other
            other.mSize = 0;
            other.mCapacity = 0;
            other._type = DatumType::Unknown;
            other.isExternal = false;
        }
        return *this;
    }


    //Relational Equal-to implementation
    bool Datum::operator==(const Datum& other) const
    {
        // Compare types
        if (_type != other._type) {
            return false;
        }

        // Compare sizes
        if (mSize != other.mSize) {
            return false;
        }

        // Compare individual elements based on type
        for (size_t i = 0; i < mSize; ++i) {
            switch (_type) {
            case DatumType::Integer:
                if (mData.Integer[i] != other.mData.Integer[i]) {
                    return false;
                }
                break;
            case DatumType::Float:
                if (mData.Float[i] != other.mData.Float[i]) {
                    return false;
                }
                break;
            case DatumType::Vector:
                if (mData.Vector[i] != other.mData.Vector[i]) {
                    return false;
                }
                break;
            case DatumType::Matrix:
                if (mData.Matrix[i] != other.mData.Matrix[i]) {
                    return false;
                }
                break;
            case DatumType::String:
                if (mData.String[i] != other.mData.String[i]) {
                    return false;
                }
                break;
            case DatumType::Table:
                if (mData.Table[i] != other.mData.Table[i]) {
                    return false;
                }
                break;
            case DatumType::Pointer:
                if (mData.Pointer[i] != other.mData.Pointer[i]) {
                    return false;
                }
                break;
            default:
                // Unsupported type
                //return false;
                break;
            }
        }

        // All checks passed, the instances are equal
        return true;
    }

    //Not-Equal to
    bool Datum::operator!=(const Datum& other) const
    {
        return !(*this == other);
    }

    //Access Type of Datum
    Datum::DatumType Datum::Type() const
    {
        return _type;
    }

    void Datum::SetType(Datum::DatumType type)
    {
        if (_type == DatumType::Unknown)
        {
            _type = type;
        }
        else
        {
            return;
        }
    }


    //Copy Assignment
    Datum& Datum::operator=(const Datum& other)
    {
 
        if (this != &other)
        {
            // Clear old data
            Clear();

            // Copy type
            _type = other._type;

            // Update size and capacity
            mSize = other.mSize;
            mCapacity = other.mSize;
            


                // Copy data based on type
                switch (_type)
                {
                case DatumType::Integer:
                    mData.Integer = new int[mSize];
                    for (size_t i = 0; i < mSize; ++i)
                    {
                        new(&mData.Integer[i])int(other.mData.Integer[i]);
                    }
                    break;
                case DatumType::Float:
                    mData.Float = new float[mSize];
                    for (size_t i = 0; i < mSize; ++i)
                    {
                        new(&mData.Float[i])float(other.mData.Float[i]);
                    }
                    break;
                case DatumType::Vector:
                    mData.Vector = new glm::vec4[mSize];
                    for (size_t i = 0; i < mSize; ++i)
                    {
                        new(&mData.Vector[i])glm::vec4(other.mData.Vector[i]);
                    }
                    break;
                case DatumType::Matrix:
                    mData.Matrix = new glm::mat4[mSize];
                    for (size_t i = 0; i < mSize; ++i)
                    {
                        new(&mData.Matrix[i])glm::mat4(other.mData.Matrix[i]);
                    }
                    break;
                case DatumType::String:
                    mData.String = new std::string[mSize];
                    for (size_t i = 0; i < mSize; ++i)
                    {
                        mData.String[i] = std::string(other.mData.String[i]);
                    }
                    break;
                case DatumType::Table:
                    mData.Table = new Scope * [mSize];
                    for (size_t i = 0; i < mSize; ++i)
                    {
                        new(mData.Table[i])Scope* (other.mData.Table[i]);
                    }
                    break;
                case DatumType::Pointer:
                    mData.Pointer = new RTTI * [mSize];
                    for (size_t i = 0; i < mSize; ++i)
                    {
                        new(mData.Pointer[i])RTTI* (other.mData.Pointer[i]);
                    }
                    break;
                default:
                    break;
                }
            }
        return *this;
    }


    //Move Semantics for scalar assignment
    Datum& Datum::operator=(int&& value)
    {
        if (_type == DatumType::Unknown)
        {
            _type = DatumType::Integer;
        }
        else if (_type != DatumType::Integer)
        {
            throw std::invalid_argument("Invalid assignment type for Datum");
        }

        Clear();
        PushBack(std::move(value));
        return *this;
    }
    Datum& Datum::operator=(float&& value)
    {
        if (_type == DatumType::Unknown)
        {
            _type = DatumType::Float;
        }
        else if (_type != DatumType::Float)
        {
            throw std::invalid_argument("Invalid assignment type for Datum");
        }

        Clear();
        PushBack(std::move(value));
        return *this;
    }
    Datum& Datum::operator=(glm::vec4&& value)
    {
        if (_type == DatumType::Unknown)
        {
            _type = DatumType::Vector;
        }
        else if (_type != DatumType::Vector)
        {
            throw std::invalid_argument("Invalid assignment type for Datum");
        }

        Clear();
        PushBack(std::move(value));
        return *this;
    }
    Datum& Datum::operator=(glm::mat4&& value)
    {
        if (_type == DatumType::Unknown)
        {
            _type = DatumType::Matrix;
        }
        else if (_type != DatumType::Matrix)
        {
            throw std::invalid_argument("Invalid assignment type for Datum");
        }

        Clear();
        PushBack(std::move(value));
        return *this;
    }
    Datum& Datum::operator=(std::string&& value)
    {
        if (_type == DatumType::Unknown)
        {
            _type = DatumType::String;
        }
        else if (_type != DatumType::String)
        {
            throw std::invalid_argument("Invalid assignment type for Datum");
        }

        Clear();
        PushBack(std::move(value));
        return *this;
    }

    //operator[] for scope
    Scope& Datum::operator[]( std::uint32_t index ) { return *GetScope(index) ; } 

    //Getter for Size
    size_t Datum::Size() const
    {
        return mSize;
    }
    size_t Datum::Capacity() const
    {
        return mCapacity;
    }

   //PopBack
    void Datum::PopBack()
    {
        if (!isExternal)
        {
            if (mSize > 0)
            {

                switch (_type)
                {
                    /*case DatumType::Integer:
                        delete mData[mSize - 1].Integer;
                        break;
                    case DatumType::Float:
                        delete mData[mSize - 1].Float;
                        break;
                    case DatumType::Vector:
                        delete mData[mSize - 1].Vector;
                        break;
                    case DatumType::Matrix:
                        delete mData[mSize - 1].Matrix;
                        break;*/
                case DatumType::String:
                    mData.String[mSize - 1].~basic_string();
                    break;
                case DatumType::Pointer:
                    delete mData.Pointer[mSize - 1];
                    break;
                case DatumType::Table:
                    delete mData.Table[mSize - 1];
                    break;
                default:
                    break;
                }
                --mSize;
            }
            else
                throw::out_of_range("No elements to Pop in Datum");
        }
    }

    //Resize
    void Datum::Resize(size_t newSize)
    {
        if (newSize < mSize)
        {
            // New size is smaller than current size, just update the size
            mSize = newSize;
        }
        else if (newSize > mSize)
        {
            if (newSize > mCapacity)
            {
                // New size is larger than the current capacity, need to reallocate memory
                reserve(newSize);
            }

            // Initialize new elements if necessary
            for (size_t i = mSize; i < newSize; ++i)
            {
                switch (_type)
                {
                case DatumType::String:
                    new (&mData.String[i]) std::string();
                    break;
                case DatumType::Integer:
                    new (&mData.Integer[i]) int();
                    break;
                case DatumType::Float:
                    new (&mData.Float[i]) float();
                    break;
                case DatumType::Vector:
                    new (&mData.Vector[i]) glm::vec4();
                    break;
                case DatumType::Matrix:
                    new (&mData.Matrix[i]) glm::mat4();
                    break;
                default:
                    break;
                }
            }

            // Update size
            mSize = newSize;
        }
        else
        {
            // newSize == mSize, do nothing
        }
    }

    //Get Value via Reference
    int& Datum::GetInt(std::size_t index) const
    {
        if (index >= mSize || _type != DatumType::Integer)
        {
            throw std::out_of_range("Invalid Index or Incorrect Type");
        }

        return mData.Integer[index];
    }

    float& Datum::GetFloat(std::size_t index) const
    {
        if (index >= mSize || _type != DatumType::Float)
        {
            throw std::out_of_range("Invalid Index or Incorrect Type");
        }

        return mData.Float[index];
    }

    const glm::vec4& Datum::GetVector(std::size_t index) const
    {
        if (index >= mSize || _type != DatumType::Vector)
        {
            throw std::out_of_range("Invalid Index or Incorrect Type");
        }

        return mData.Vector[index];
    }

    const glm::mat4& Datum::GetMatrix(std::size_t index) const
    {
        if (index >= mSize || _type != DatumType::Matrix)
        {
            throw std::out_of_range("Invalid Index or Incorrect Type");
        }

        return mData.Matrix[index];
    }

    const std::string& Datum::GetString(std::size_t index) const
    {
        if (index >= mSize || _type != DatumType::String)
        {
            throw std::out_of_range("Invalid Index or Incorrect Type");
        }

        return mData.String[index];
    }

    Scope* Datum::GetScope(size_t index) const
    {
        if (index >= mSize || _type == Datum::DatumType::Table)
        {
            return mData.Table[index];
        }
        else
        {
            throw std::invalid_argument("Datum at the specified index is not of type Table");
        }
    }

    //Assign Value Via reference
    void Datum::Assign(const int& value, const size_t idx)
    {
        if (idx >= mSize)
        {
            throw std::out_of_range("Invalid Index");
        }

        if (_type == DatumType::Integer)
        {
            // Assign by value
            mData.Integer[idx] = value;
        }
        else
        {
            throw std::invalid_argument("Invalid Type");
        }
    }

    void Datum::Assign(const float& value, const size_t idx)
    {
        if (idx >= mSize)
        {
            throw std::out_of_range("Invalid Index");
        }

        if (_type == DatumType::Float)
        {
            // Assign by value
            mData.Float[idx] = value;
        }
        else
        {
            throw std::invalid_argument("Invalid Type");
        }
    }

    void Datum::Assign(const glm::vec4& value, const size_t idx)
    {
        if (idx >= mSize)
        {
            throw std::out_of_range("Invalid Index");
        }

        if (_type == DatumType::Vector)
        {
            // Assign by value
            mData.Vector[idx] = value;
        }
        else
        {
            throw std::invalid_argument("Invalid Type");
        }
    }

    void Datum::Assign(const glm::mat4& value, const size_t idx)
    {
        if (idx >= mSize)
        {
            throw std::out_of_range("Invalid Index");
        }

        if (_type == DatumType::Matrix)
        {
            // Assign by value
            mData.Matrix[idx] = value;
        }
        else
        {
            throw std::invalid_argument("Invalid Type");
        }
    }

    void Datum::Assign(const std::string& value, const size_t idx)
    {
        if (idx >= mSize)
        {
            throw std::out_of_range("Invalid Index");
        }

        if (_type == DatumType::String)
        {
            // Assign by value
            mData.String[idx] = value;
        }
        else
        {
            throw std::invalid_argument("Invalid Type");
        }
    }

    //Get Value as String Reference
    std::string Datum::GetIntAsString(std::size_t index) const {
        if (index >= mSize || _type != DatumType::Integer) {
            throw std::out_of_range("Invalid Index or Incorrect Type");
        }

        return std::to_string(mData.Integer[index]);
    }
    std::string Datum::GetFloatAsString(std::size_t index) const {
        if (index >= mSize || _type != DatumType::Float) {
            throw std::out_of_range("Invalid Index or Incorrect Type");
        }

        return std::to_string(mData.Float[index]);
    }
    std::string Datum::GetMatrixAsString(std::size_t index) const {
        if (index >= mSize || _type != DatumType::Matrix) {
            throw std::out_of_range("Invalid Index or Incorrect Type");
        }

        // Assuming glm::mat4 can be converted to a string representation
        return mattostring(mData.Matrix[index]);
    }
    std::string Datum::GetVectorAsString(std::size_t index) const {
        if (index >= mSize || _type != DatumType::Vector) {
            throw std::out_of_range("Invalid Index or Incorrect Type");
        }

        // Assuming glm::vec4 can be converted to a string representation
        return vectostring(mData.Vector[index]);
    }


    //Set Storage
    void Datum::SetStorage(int* externalArray, size_t size) {
        SetType(DatumType::Integer);
        mData.Integer = externalArray;
        mSize = size;
        mCapacity = size;
        isExternal = true;
    }

    void Datum::SetStorage(float* externalArray, size_t size) {
        SetType(DatumType::Float);
        mData.Float = externalArray;
        mSize = size;
        mCapacity = size;
        isExternal = true;
    }

    void Datum::SetStorage(glm::vec4* externalArray, size_t size) {
        SetType(DatumType::Vector);
        mData.Vector = externalArray;
        mSize = size;
        mCapacity = size;
        isExternal = true;
    }

    void Datum::SetStorage(glm::mat4* externalArray, size_t size) 
    {
        SetType(DatumType::Matrix);
        mData.Matrix = externalArray;
        mSize = size;
        mCapacity = size;
        isExternal = true;
    }
    void Datum::SetStorage(std::string* externalArray, size_t size)
    {
        SetType(DatumType::String);
        mData.String = externalArray;
        mSize = size;
        mCapacity = size;
        isExternal = true;
    }
    void Datum::SetStorage(Scope** externalArray, size_t size)
    {
        SetType(DatumType::Table);
        mData.Table = externalArray;
        mSize = size;
        mCapacity = size;
        isExternal = true;
    }

    void Datum::SetStorage(RTTI** externalArray, size_t size)
    {
        SetType(DatumType::Pointer);
        mData.Pointer = externalArray;
        mSize = size;
        mCapacity = size;
        isExternal = true;
    }

    //Helper functions implementation
    std::string Datum::vectostring(const glm::vec4& vector) const {
        return "(" + std::to_string(vector.x) + ", " + std::to_string(vector.y) + ", " +
            std::to_string(vector.z) + ", " + std::to_string(vector.w) + ")";
    }
    std::string Datum::mattostring(const glm::mat4& matrix) const
    {
        std::string result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result += std::to_string(matrix[i][j]);
                if (j < 3) {
                    result += ", ";
                }
            }
            if (i < 3) {
                result += "; ";
            }
        }
        return result;
    }

    //removeAt
    void Datum::RemoveAt(size_t index)
    {
        if (isExternal)
        {
            throw std::runtime_error("Can't delete external memory");
        }

        if (_type == DatumType::Unknown)
        {
            throw std::runtime_error("Datum is Empty");
        }

        if (index >= mSize)
        {
            throw std::out_of_range("Index is out of range");
        }

        switch (_type)
        {
        case DatumType::Integer:
            // Move elements after the removed one
            for (size_t i = index; i < mSize - 1; ++i)
            {
                mData.Integer[i] = mData.Integer[i + 1];
            }
            break;

        case DatumType::Float:
            // Move elements after the removed one
            for (size_t i = index; i < mSize - 1; ++i)
            {
                mData.Float[i] = mData.Float[i + 1];
            }
            break;

        case DatumType::Vector:
            // Move elements after the removed one
            for (size_t i = index; i < mSize - 1; ++i)
            {
                mData.Vector[i] = mData.Vector[i + 1];
            }
            break;

        case DatumType::Matrix:
            // Move elements after the removed one
            for (size_t i = index; i < mSize - 1; ++i)
            {
                mData.Matrix[i] = mData.Matrix[i + 1];
            }
            break;

        case DatumType::Table:
            // Move elements after the removed one
            for (size_t i = index; i < mSize - 1; ++i)
            {
                mData.Table[i] = mData.Table[i + 1];
            }
            break;

            // Add cases for other types as needed

        default:
            // Unsupported type
            break;
        }

        --mSize;
    }

}
