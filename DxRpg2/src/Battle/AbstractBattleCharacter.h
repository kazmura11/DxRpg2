#pragma once
#include "SharedCharacterStatus.h"  // 不完全クラス型へのポインターが参照できないそうなので

namespace Battle
{
	const int AbilityMax = 5;
	const int DamagePointMax = 200;
	const int UseMp[] = { 9, 8, 16, 24, 32 };

	struct Status
	{
		int power;
		int defense;
		int strength;
		int magic;
		int weapon;
		int equip;
		Status()
			: power(0), defense(0), strength(0), magic(0), weapon(0), equip(0) { }
	};

	struct Magic
	{
		int flag;
		int kind;
		int cnt;
		Magic()
			: flag(0), kind(0), cnt(0) { }
		void change()
		{
			if (flag == 1)
			{
				flag = 2;
			}
		}
	};

	class AbstractBattleCharacter
	{
	public:
		AbstractBattleCharacter();
		virtual ~AbstractBattleCharacter();

		int getX() const;
		void setX(int x);
		int getY() const;
		void setY(int y);
		int getDamagePoint() const;
		void setDamagePoint();

		Status &getStatus();
		void setStatus(Status status);
		Magic &getAttackMagic();
		void changeAttackMagic();
		Magic &getCureMagic();
		void changeCureMagic();

		int getMenuSelect() const;
		int getMenuSelectY() const;
		void setMenuSelect(int menuSelect);
		void setMenuSelectY(int menuSelectY);

		void getOffenseParam(int *power, int *strength, int *weapon, int *level);
		void getDefenseParam(int *defense, int *equip);

		int getActionCnt() const;
		void setActionCnt(int actionCnt);
		void increaseActionCnt();
		int getDamageActionCnt() const;
		void setDamageActionCnt(int damageActionCnt);
		void increaseDamageActionCnt();

		void increaseX();
		void decreaseX();

		bool getActionFlag() const;
		void setActionFlag(bool actionFlag);

		int getAtbCnt() const;
		void setAtbCnt(int atbCnt);
		void increaseAtbCnt();

		bool getAtbCntFlag() const;
		void setAtbCntFlag(bool atbCntFlag);

		bool getMotionFlag() const;
		void setMotionFlag(bool motionFlag);
		bool getAnimationFlag() const;
		void setAnimationFlag(bool animationFlag);
		bool getDamageFlag() const;
		void setDamageFlag(bool damageFlag);

		int getDamage() const;
		void setDamage(int damage);

		int getCenterX() const;
		void setCenterX(int centerX);
		int getCenterY() const;
		void setCenterY(int centerY);

		void increaseHp(int hp);
		void decreaseMp(int mp);
		void useMp();

		int getHp() const;
		void setHp(int hp);
		int getHpMax() const;
		int getMp() const;
		int getMpMax() const;
		int getMagic() const;
		int getKind() const;
		void setKind(int kind);

	protected:
		virtual void initializeParameter() = 0;
		int	 x_;
		int	 y_;

		int damagePoint_;	// ダメージを受けるとたまる値
		double cntSpeed_;
		bool abiFlag_[60];
		int atbCnt_;
		bool atbCntFlag_;
		bool motionFlag_;
		int menuSelect_;
		int menuSelectY_;
		int actionCnt_;
		int damageActionCnt_;
		bool actionFlag_;
		bool animationFlag_;
		bool damageFlag_;
		int damage_;

		Status status_;
		Magic attackMagic_;
		Magic cureMagic_;

		int centerX_;
		int centerY_;

		::SharedCharacterStatus *sharedStatus_;
	};
}