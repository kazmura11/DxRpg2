#pragma once
#include "AbstractBattleCharacter.h"

namespace Battle
{
	constexpr int BattleCharPosX1 = 500;
	constexpr int BattleCharPosY1 = 200;
	class BattleCharacter : public AbstractBattleCharacter
	{
	public:
		BattleCharacter(std::shared_ptr<::SharedCharacterStatus> sharedStatus);
		~BattleCharacter();

		void pressedKeyUp();
		void pressedKeyDown();
		void pressedKeyX();
		void pressedKeyZ();
		void resetSelectState();

	private:
		void initializeParameter() override;
	};
}
