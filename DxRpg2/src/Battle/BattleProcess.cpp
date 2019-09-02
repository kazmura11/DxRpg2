#include "Battle/BattleProcess.h"
#include "StdCommon.h"
#include "Util/ResourceLoader.h"
#include "Util/GameController.h"
#include "Battle/AbstractBattleCharacter.h"
#include "Battle/BattleCharacter.h"
#include "Battle/BattleMonster.h"
#include "Battle/BattleAnimation.h"
#include "Battle/BattleGraph.h"
#include "Battle/Board.h"

namespace Battle
{
	BattleProcess::BattleProcess(BattleCharacter *chr, BattleMonster *mon)
		:isWin_(false), isGameover_(false),
		 isWinMoveNext_(false), isGameoverMoveNext_(false),
		 gc_(Util::GameController::getInstance()),
		 rl_(Util::ResourceLoader::getInstance()),
		ba_(std::make_unique<BattleAnimation>()),
		bg_(std::make_unique<BattleGraph>()),
		board_(std::make_unique<Battle::Board>())
	{
		chr_ = chr;
		mon_ = mon;
	}

	BattleProcess::~BattleProcess()
	{
	}

	void BattleProcess::main()
	{
		calculateAtb();  // バトル用のカウンタ(=ATB)を上げる
 
		if (!isGameover_ && !isWin_)
		{
			// 味方
			if (!chr_->getMotionFlag() && chr_->getAtbCnt() == AtbCntMax)
			{
				selectMenu();
			}
			// 敵
			if (!mon_->getMotionFlag() && mon_->getAtbCnt() == AtbCntMax)
			{
				mon_->setMotionFlag(true);
			}
		}
		// 以下の処理は戦闘終了後(Win or Gameover)でも一部行う
		// 攻撃のモーション
		if (chr_->getMotionFlag())
		{
			doAction(chr_, mon_, Character);
		}
		if (mon_->getMotionFlag())
		{
			doAction(mon_, chr_, Enemy);
		}

		decideGameover();
		decideWin();

		graphBattle();
		
		decreaseHpAction(mon_);
		decreaseHpAction(chr_);
		displayMeter();

		ba_->graphAnimation(chr_, mon_);

		board_->graph();

		// モーションしてなくてATBカウンタがMAXなら選択項目を描画
		if (!chr_->getMotionFlag() && chr_->getAtbCnt() == AtbCntMax)
		{
			bg_->graphMenu(chr_);
		}

		if (isGameover_)
		{
			// ゲームオーバー処理
			graphGameover();
		}
		else if (isWin_)
		{
			// 勝ったときの処理
			graphWin();
		}
	}

	bool BattleProcess::getWinMoveNext() const
	{
		return isWinMoveNext_;
	}

	bool BattleProcess::getGameoverMoveNext() const
	{
		return isGameoverMoveNext_;
	}

	void BattleProcess::calculateAtb()
	{
		if (mon_->getAtbCntFlag() && !isGameover_ && !isWin_)
		{
			mon_->increaseAtbCnt();
		}
		if (chr_->getAtbCntFlag() && !isGameover_ && !isWin_)
		{
			chr_->increaseAtbCnt();
		}
	}

	void BattleProcess::selectMenu()
	{
		if (gc_.upPressedNow())
		{
			chr_->pressedKeyUp();
		}
		else if (gc_.downPressedNow())
		{
			chr_->pressedKeyDown();
		}
		if (gc_.xPressedNow())
		{
			chr_->pressedKeyX();
			if (chr_->getMenuSelect() < 10000)   // 選択がすべて完了していなければ
			{
				// NOP
			}
			else
			{
				chr_->resetSelectState();
				chr_->setMotionFlag(true);
			}
		}
		else if (gc_.zPressedNow())
		{
			chr_->pressedKeyZ();
		}
	}

	void BattleProcess::doAction(AbstractBattleCharacter *self,
		AbstractBattleCharacter *opponent,
		CharacterEnemyKind k)
	{
		if (isGameover_ || isWin_)
		{
			self->setActionCnt(0);
			return;
		}
		// 前にずれるアクション
		if (20 < self->getActionCnt() && self->getActionCnt() <= 40)
		{
			//　味方
			if (k == Character)
			{
				self->decreaseX();
			}
			// 敵
			else
			{
				self->increaseX();
			}
		}
		// 攻撃モーションを開始
		if (self->getActionCnt() == 50)
		{
			// 味方
			if (k == Character)
			{
				ba_->startAnimation(self->getMenuSelect(), 0, 100);
			}
			// 敵
			else
			{
				// 10000は通常攻撃
				ba_->startAnimation(10000, 100, 0);
			}
		}

		// 今の瞬間ダメージアクションをするフラグが立っていたら START
		int damage = 0;
		if (self->getActionFlag())	// BattleAnimationクラスでONになる
		{
			// 敵の場合
			if (k == Enemy)
			{
				// 通常攻撃選択
				self->setMenuSelect(10000);
			}

			switch (self->getMenuSelect())
			{
				case 10000:
				case 10100:
				case 20100:
				case 20200:
				case 20300:
				case 20400:
					damage = attackAction(self, opponent, k);
					ba_->setPopDamage(damage, opponent->getCenterX() - 20, opponent->getCenterY() - 20, 0);
					break;

				case 20000:
					damage = cureAction(self);
					ba_->setPopDamage(damage, self->getCenterX() - 20, self->getCenterY() - 20, 1);
					break;

				case 30000:
					// 未実装
					break;

				default:
					break;
			}
			self->setActionFlag(false);
			self->increaseDamageActionCnt();
		}

		// 今の瞬間ダメージアクションをするフラグが立っていたら END

		// 後ろに下がる
		if (0 < self->getDamageActionCnt() && self->getDamageActionCnt() <= 20)
		{
			// 後ろに動く
			if (k == Character)
			{
				self->increaseX();
			}
			else
			{
				self->decreaseX();
			}
		}

		self->increaseActionCnt();	// ここは必ず毎回実行
		if (self->getDamageActionCnt() > 0)
		{
			self->increaseDamageActionCnt();
		}
		if (self->getDamageActionCnt() == 30)
		{
			// 各項目を初期化
			self->setActionCnt(0);
			self->setDamageActionCnt(0);

			self->setMenuSelect(1);
			self->setMenuSelectY(0);
			self->setMotionFlag(false);
			self->setAtbCntFlag(true);
			self->setAtbCnt(0);
		}

		// ゲームオーバーか勝利
		if (isGameover_ || isWin_)
		{
			self->setActionCnt(0);
			self->setDamageActionCnt(0);
		}
	}

	int BattleProcess::attackAction(
		AbstractBattleCharacter *self,
		AbstractBattleCharacter *opponent,
		CharacterEnemyKind k)
	{
		int damage = decideDamage(self, ((k == Character) ? 0 : 1), self->getMenuSelect());
		if (self->getAttackMagic().flag)
		{
			damage *= 2;
		}

		opponent->setDamage(damage);  // ダメージをセット

		// 相手のダメージポイントをセット
		opponent->setDamagePoint();

		// 魔法なら
		if (20000 <= self->getMenuSelect() && self->getMenuSelect() < 30000)
		{
			self->useMp();
		}
		self->changeAttackMagic();
		return damage;
	}


	int BattleProcess::cureAction(AbstractBattleCharacter *c)
	{
		int damage = decideDamage(c, 0, c->getMenuSelect());
		c->decreaseMp(9);
		c->increaseHp(damage);

		// 回復しすぎたぶん戻す
		if (c->getHp() > c->getHpMax())
		{
			c->setHp(c->getHpMax());
		}
		c->increaseHp(0);   // 回復量を戻す
		c->changeCureMagic();
		return damage;
	}

	// to: 0:味方->敵、1:敵->味方
	int BattleProcess::decideDamage(AbstractBattleCharacter *c, int to, int actKind)
	{
		int power = 0;
		int strength = 0;
		int weapon = 0;
		int level = 0;
		int defense = 0;
		int equip = 0;
		switch (to)
		{
			case 0:	 // 味方の「通常攻撃」
				chr_->getOffenseParam(&power, &strength, &weapon, &level);
				mon_->getDefenseParam(&defense, &equip);
				if (actKind % 10000 == 2)	  // 武力の泉の時(下1ケタが2)
				{
					power += 100 * (actKind % 10000) / 100;
				}
				if (actKind % 10000 == 100)   // 特殊攻撃
				{
					power *= 4;
				}
				break;
			case 1:	 // 敵の「通常攻撃」
				mon_->getOffenseParam(&power, &strength, &weapon, &level);
				chr_->getDefenseParam(&defense, &equip);
				break;
			case 2:
				break;
			case 3:
				break;
		}
		if (actKind < 40000 && actKind != 20000)  // 攻撃
		{
			return static_cast<int>(level * 2 + (power + weapon - (defense + equip))
				* strength * genRandom(0.01, 0.05) + strength * genRandom(0.2, 1.0));
		}
		if (actKind == 20000)	// 回復
		{
			return static_cast<int>(c->getMagic() * 10.0 * genRandom(0.2, 1.0));
		}
		return -1;
	}

	void BattleProcess::decideGameover()
	{
		if (chr_->getHp() <= 0)
		{
			isGameover_ = true; // ゲームオーバー
		}
	}

	void BattleProcess::decideWin()
	{

		if (mon_->getHp() <= 0)
		{
			isWin_ = true; // 勝利
		}
	}

	void BattleProcess::graphBattle()
	{
		bg_->drawBackground();
		bg_->drawBottomWindow();

		bg_->drawMonsterImage(mon_);
		bg_->drawCharacterImage(chr_);
	}

	void BattleProcess::displayMeter()
	{
		bg_->graphMonsterBar(mon_);
		bg_->graphCharacterBar(chr_);
	}

	// HP現象のモーション 一瞬で減るように改造
	void BattleProcess::decreaseHpAction(AbstractBattleCharacter *c)
	{
		if (isGameover_ && isWin_)
		{
			return;
		}

		if (c->getHp() - c->getDamage() < 0)
		{
			c->setHp(0);
		}
		else
		{
			int newHp = c->getHp() - c->getDamage();
			c->setHp(newHp);
		}
		c->setDamage(0);
	}

	/**
	 * @param sigma
	 * @param average
	 * @return 散らばり度合いとして正規分布した乱数を返す
	 */
	double BattleProcess::genRandom(double sigma, double average)
	{
		static double r1, r2, s, r;
		static int valid = 0;
		double c;
		if (!valid)
		{
			r1 = DxLib::GetRand(RAND_MAX) / static_cast<double>(RAND_MAX + 1);
			r2 = DxLib::GetRand(RAND_MAX) / static_cast<double>(RAND_MAX + 1);
			s = sqrt(-2.0 * log(1.0 - r2));
		}
		valid = !valid;
		c = 2.0 * PI * r1;
		r = valid ? s * sin(c) : s + cos(c);
		return r * sigma + average;
	}

	void BattleProcess::graphGameover()
	{
		static int cnt = 0;
		bg_->graphGameover();

		cnt++;
		if (cnt == 120)
		{
			cnt = 0;
			isGameoverMoveNext_ = true;
		}
	}

	void BattleProcess::graphWin()
	{
		static int cnt = 0;

		bg_->graphWin();

		cnt++;
		if (cnt == 120)
		{
			cnt = 0;
			isWinMoveNext_ = true;
		}
	}

}
