#pragma once
#include "AbstractBattleCharacter.h"

namespace Battle
{
	const int BattleCharPosX1 = 500;
	const int BattleCharPosY1 = 200;
	class BattleCharacter : public AbstractBattleCharacter
	{
	public:
		BattleCharacter(SharedCharacterStatus *sharedStatus);
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
