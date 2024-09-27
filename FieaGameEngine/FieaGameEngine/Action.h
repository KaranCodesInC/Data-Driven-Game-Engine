#pragma once
#include "Attributed.h"

typedef size_t GameTime;
namespace Fiea::GameEngine
{
	class GameObject;

	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed);
	public:
		

		Action();

		Action(const Action& other) = default;

		Action(Action&& other) = default;

		Action& operator=(Action&& other) = default;

		Action& operator=(const Action& other) = default;

		virtual ~Action() = default;

		//Datum& AppendAuxiliaryAttribute(const std::string& key) override;

		static std::vector<signature> Signatures();

		std::string name = "Actions";

		void virtual Update(const GameTime&) = 0;

		std::string ToString() const override { return Attributed::ToString(); }

		//[[nodiscard]] Action* Clone() const override;

		void setParentGameObject(GameObject* parent);

	protected:
		Action(size_t id);
		GameObject* parentGameObject = nullptr;

	};
}
