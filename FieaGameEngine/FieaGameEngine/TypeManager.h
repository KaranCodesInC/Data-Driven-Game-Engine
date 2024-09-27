// TypeManager.h
#pragma once

#include "signature.h"
#include <unordered_map>
#include <string>

namespace Fiea::GameEngine
{
    class TypeManager
    {
    public:
        static TypeManager& Instance()
        {
            if (instance == nullptr)
            {
                instance = new TypeManager();
            }

            return *instance;
        }

       // ~TypeManager() = default;

        // Modify AddSignature to accept vector parameter by value
       void AddSignature(size_t id, std::vector<signature> sig)
        {
            mSignatures[id] = sig;
        }

       std::vector<signature>& GetSignature(const size_t& id)
        {
            auto it = mSignatures.find(id);
            if (it != mSignatures.end())
            {
                return it->second;
            }
            throw std::out_of_range("Type signature not found");
        }

        void cleanup()
        {
            delete instance;
            instance = nullptr;
        }

        inline static TypeManager* instance;
    private:
        TypeManager() = default;
        
        TypeManager(const TypeManager&) = delete;
        TypeManager& operator=(const TypeManager&) = delete;
        TypeManager(TypeManager&&) = delete;
        TypeManager& operator=(TypeManager&&) = delete;

        std::unordered_map<size_t, std::vector<signature>> mSignatures{};
    };
}
