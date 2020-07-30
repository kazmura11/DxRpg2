#pragma once
#include "AbstractBattleCharacter.h"

namespace Battle
{
	const int	BattleMonPosX1 = 100;    //モンスター１の座標
	const int	BattleMonPosY1 = 150;

	class BattleMonster : public AbstractBattleCharacter
	{
	public:
		BattleMonster();
		virtual ~BattleMonster();

	private:
		void initializeParameter() override;
	};
}