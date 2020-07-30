#include "Battle/BattleMonster.h"
#include "DxLib.h"
#include "StdCommon.h"
#include "SharedCharacterStatus.h"

namespace Battle
{
	BattleMonster::BattleMonster()
	{
		// 敵の場合は毎度HP等ここで作成する
		sharedStatus_ = new ::SharedCharacterStatus();  // memory leak warning
		initializeParameter();
	}

	BattleMonster::~BattleMonster()
	{
		if (sharedStatus_ != NULL) {
			delete sharedStatus_;  // TODO: ここどうしよう
			sharedStatus_ = NULL;
		}
	}

	void BattleMonster::initializeParameter()
	{
		x_ = BattleMonPosX1;
		y_ = BattleMonPosY1;
#ifdef _DEBUG
		sharedStatus_->hp_ = 200;
#else
		sharedStatus_->hp_ = 200 + DxLib::GetRand(20);
#endif
		sharedStatus_->hpMax_ = sharedStatus_->hp_;
		sharedStatus_->mp_ = 136;
		sharedStatus_->mpMax_ = 136;
		sharedStatus_->power_ = 20;    // 攻撃力
		sharedStatus_->defense_ = 20;    // 防御力
		sharedStatus_->strength_ = 20;    // 力
		sharedStatus_->magic_ = 20;
		sharedStatus_->weapon_ = 20;    // 武器の攻撃力
		sharedStatus_->equip_ = 20;    // 装備の防御力
		sharedStatus_->level_ = 10;
	
		damagePoint_ = 0;
		cntSpeed_ = 1.0;    // カウンターが増加するスピード(double)
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
		animationFlag_ = false;    // アニメーションをするかのフラグ
		damageFlag_ = false;    // ダメージ表示するかのフラグ
	}
}