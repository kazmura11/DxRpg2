#include "Battle/BattleCharacter.h"
#include "StdCommon.h"
#include "SharedCharacterStatus.h"
#include <cstdlib>

namespace Battle
{
	BattleCharacter::BattleCharacter(std::shared_ptr<::SharedCharacterStatus> sharedStatus)
	{
		sharedStatus_ = std::move(sharedStatus);
		initializeParameter();
	}

	BattleCharacter::~BattleCharacter()
	{
	}

	void BattleCharacter::initializeParameter()
	{
		x_ = BattleCharPosX1;
		y_ = BattleCharPosY1;

		damagePoint_ = 0;
		cntSpeed_ = 1.0;
		for (int i = 0; i < sizeof(abiFlag_); i++)
		{
			abiFlag_[i] = 0;
		}
		atbCnt_ = DxLib::GetRand(AtbCntMax - 5);  // カウンター240で行動可
		atbCntFlag_ = true;
		motionFlag_ = false;
		menuSelect_ = 1;
		menuSelectY_ = 0;
		actionCnt_ = 0;
		damageActionCnt_ = 0;
		actionFlag_ = false;

		animationFlag_ = false;
		damageFlag_ = false;
	}

	void BattleCharacter::pressedKeyUp()
	{
		menuSelect_--;
		menuSelectY_--;
		if (menuSelectY_ == -1) // 選択位置が一番上を超えたら
		{
			menuSelect_ += 5;    // 一番下へ 
			menuSelectY_ = 4;   // 一番下へ 4は選択表示項目の最大数-1
		}
	}

	void BattleCharacter::pressedKeyDown()
	{
		menuSelect_++;
		menuSelectY_++;
		if (menuSelectY_ == 5) // 選択位置が一番上を超えたら
		{
			menuSelect_ -= 5;   // 一番上へ
			menuSelectY_ = 0;   // 一番上へ
		}
	}

	void BattleCharacter::pressedKeyX()
	{
		menuSelect_ = menuSelect_ * 100;   // 決定ボタンが押されるたびに100倍に
		menuSelectY_ = 0;   // 選択位置を初期状態に
	}

	void BattleCharacter::pressedKeyZ()
	{
		menuSelect_ = 1;    // 選択状態を戻す 
		menuSelectY_ = 0;   // 選択位置を初期状態に
	}

	void BattleCharacter::resetSelectState()
	{
		switch (menuSelect_)
		{
			case 10200:
			case 10300:
			case 10400:
		menuSelect_ = 10000;
				break;
			case 30000:
				menuSelect_ = 10000;
				break;
		}
	}
}