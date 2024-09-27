#pragma once
#include "Attributed.h"
#include "signature.h"
#include "AttributedBar.h"
#include "Action.h"


namespace Fiea::GameEngine {

    class GameObject : public Attributed
    {
        RTTI_DECLARATIONS(GameObject, Attributed);
        typedef size_t GameTime;

    public:

        //Constructor
        GameObject();

       // GameObject(size_t id);

        // Destructor
        virtual ~GameObject() override = default;

        // Copy constructor
        GameObject(const GameObject& other) = default;

        // Move constructor
        GameObject(GameObject&& other) noexcept = default;

        // Copy assignment operator
        GameObject& operator=(const GameObject& other) = default;

        // Move assignment operator
        GameObject& operator=(GameObject&& other) noexcept = default;

        //clonable pattern functionality
        [[nodiscard]] GameObject* Clone() const override;

        void Update(const GameTime& time);

        void addChild(GameObject* child);

        void removeChild(size_t idx);

        GameObject* getChild(size_t idx);

        GameObject* GetRoot(GameObject* GameObj);

        Datum* Actions();

        void addAction(Action* action, const std::string& key = "");

        Action* CreateAction(const std::string& className, const std::string& instanceName = "");

        Action* getAction(const string& classmName);

        static std::vector<signature> Signatures();
        static std::vector<signature> CombinedSigs();

        std::string Name = "GameObjectParent";

        struct Transform
        {
            glm::vec4 position = glm::vec4(0.0f);
            glm::vec4 rotation = glm::vec4(0.0f);
            glm::vec4 scale = glm::vec4(1.0f);
        };

        Transform Transform;

        bool updateCall = false;
        
    protected:

        GameObject(size_t id);

    private:

        Scope* children;
        Scope* actions;

    };
}




