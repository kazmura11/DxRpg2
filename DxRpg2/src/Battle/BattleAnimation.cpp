#include "StdCommon.h"
#include "Battle/BattleAnimation.h"
#include "Battle/BattleCharacter.h"
#include "Battle/BattleMonster.h"
#include "Util/ResourceLoader.h"

namespace Battle
{

	// C++のコンストラクタで参照渡しをすると「参照が初期化されずに宣言されています」と出る
	// https://www.hamayanhamayan.com/entry/2017/11/27/200917
	BattleAnimation::BattleAnimation():
		rl_(Util::ResourceLoader::getInstance())
	{
	}

	BattleAnimation::~BattleAnimation()
	{
	}

	// 攻撃アニメーションの開始
	void BattleAnimation::startAnimation(int selected, int target, int actor)
	{
		// 使っていないアニメーションを探す
		int i = 0;
		for (i = 0; i < AnimationMax - 1; i++)   // 1個減らす
		{
			// フラグが立っていない
			if (!animation_[i].flag)
			{
				break;
			}
		}
		animation_[i].flag = true;
		animation_[i].target = target;   // 現状 100:味方,  0:敵
		animation_[i].kind = selected;   // menuSelect()の値
		animation_[i].cnt = 0;
		animation_[i].actor = actor;     // 現状 100:味方,  0:敵

	}

	void BattleAnimation::drawAnimation(BattleCharacter *chr, BattleMonster *mon)
	{
		for (int i = 0; i < AnimationMax; i++)
		{
			if (animation_[i].flag)
			{
				int k = animation_[i].kind / 10000;
				switch (k)  // アニメーションの種類によって処理を分岐
				{
					case 1:  // 攻撃
						// ターゲットが敵
						if (animation_[i].target == 0)	// ターゲットが敵no.0だったら
						{
							animation0(i, chr, mon->getX() + 5, mon->getY() + 5);
						}
						// ターゲットが味方
						if (animation_[i].target == 100)  // ターゲットが味方no.100なら
						{
							animation0(i, mon, chr->getX() - 45, chr->getY() - 30);
						}
						break;
					case 2:  // 魔法
						// 回復魔法
						if (animation_[i].kind == 20000)
						{
							// ターゲットが敵
							if (animation_[i].actor == 0)
							{
								animation1(mon, i, 0, 0);
							}
							// ターゲットが味方
							else if (animation_[i].actor == 100)
							{
								animation1(chr, i, -30, 20);
							}
						}
						// 攻撃魔法
						else
						{
							// 敵
							if (animation_[i].target == 0)	// ターゲットが敵no.0だったら
							{
								animation0(i, chr, mon->getX() + 5, mon->getY() + 5);
							}
							// 味方
							if (animation_[i].target == 100)  // ターゲットが見方no.0なら
							{
								animation0(i, mon, chr->getX() - 32, chr->getY() - 16);
							}
						}
						break;
					default:
						exit(9999);
						break;
				}
			}
		}
		drawDamage();   // Animationの後にDamage
	}

	void BattleAnimation::drawDamage()
	{
		// 途中が飛んでいてもよい breakしない。
		for (int i = 0; i < PopDamMax; i++)
		{
			if (popDamage_[i].flag)
			{
				calcDamage(i);
				popDamage_[i].cnt++;
				// たってから120(2秒)たったらフラグを落とす
				if (popDamage_[i].cnt == 120)
				{
					popDamage_[i].flag = false;
				}
			}
		}
	}

	void BattleAnimation::calcDamage(int t)
	{
		// 位置調整 揺れて数字が出る様子を表現
		const int ArraySize = 20;
		int y[ArraySize] =
		{
			-1, -3, -4, -5, -4, -3, -1, 1, 3, 7,
			11, 15, 22, 16, 13, 12, 13, 16, 22, 21
		};
		char damage[6];
		sprintf(damage, "%d", popDamage_[t].damage);
		int cy;
		// ダメージの桁数だけループ
		for (int i = 0; i < static_cast<int>(strlen(damage)); i++)
		{
			if (popDamage_[t].cnt - 4 * i < 0)
			{
				return;
			}
			if (popDamage_[t].cnt - 4 * i < ArraySize)
			{
				cy = y[popDamage_[t].cnt - 4 * i];
			}
			else
			{
				cy = 22;
			}
			// ここも敵と味方で微妙に位置が違う変えるべき
			// ポップダメージ描画
			DxLib::DrawGraph(popDamage_[t].x + 15 * i,
				popDamage_[t].y + cy - popDamage_[t].type * 14,
				rl_.getHdlImgNumber(popDamage_[t].type, damage[i] - '0'),
				TRUE);
		}
	}

	void  BattleAnimation::animation0(int idx, AbstractBattleCharacter *c, int x, int y)
	{
		int attackKind = animation_[idx].kind / 10000;

		// 動作アニメーション描画
		if (animation_[idx].cnt < 64)
		{
			if (attackKind == 1)  // 物理攻撃
			{
				int kind = animation_[idx].cnt / 13;  // 64を12で割ると0-4にばらけるのでこうしておく
				DxLib::DrawGraph(x, y, rl_.getSlashImage(kind), TRUE);
			}
			else if (attackKind == 2)  // 攻撃魔法
			{
				int subKind = animation_[idx].kind % 10000 / 100;
				int kind = animation_[idx].cnt / 8;  // 64を8で割ると0-7にばらけるのでこうしておく
				switch (subKind) {
				case 1:  // wind
					DxLib::DrawGraph(x, y, rl_.getWindImage(kind), TRUE);
					break;
				case 2:  // fire
					DxLib::DrawGraph(x, y, rl_.getFireImage(kind), TRUE);
					break;
				case 3:  // ice
					DxLib::DrawGraph(x, y, rl_.getIceImage(kind), TRUE);
					break;
				case 4:  // thunder
					DxLib::DrawGraph(x, y, rl_.getThunderImage(kind), TRUE);
					break;
				}
			}
		}
		animation_[idx].cnt++;

		if (animation_[idx].cnt == 64)  // 調整 -> 直後に
		{
			animation_[idx].flag = false;
			c->setActionFlag(true);  // 重要
			animation_[idx].cnt = 0;
		}

	}

	// キュア
	void  BattleAnimation::animation1(AbstractBattleCharacter *c, int idx, int x, int y)
	{
		if (animation_[idx].cnt < 64)
		{
			DxLib::DrawGraph(c->getX() + x,
				c->getY() + y - animation_[idx].cnt,
				rl_.getCureImage(animation_[idx].cnt/ 8),
				TRUE);
		}
		animation_[idx].cnt++;

		if (animation_[idx].cnt == 64)
		{
			animation_[idx].flag = false;
			c->setActionFlag(true);
			animation_[idx].cnt = 0;
		}
	}

	void BattleAnimation::setPopDamage(int damage, int x, int y, int type)
	{
		int i = 0;
		for (i = 0; i < PopDamMax - 1; i++)   // 1個減らす
		{
			if (!popDamage_[i].flag) { break; }
		}
		popDamage_[i].flag = true;
		popDamage_[i].cnt = 0;
		popDamage_[i].damage = damage;
		popDamage_[i].x = x;
		popDamage_[i].y = y;
		popDamage_[i].type = type;  // 1: cure, 0: others
	}

}