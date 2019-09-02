#include "AbstractBattleCharacter.h"
#include "StdCommon.h"

namespace Battle
{
	AbstractBattleCharacter::AbstractBattleCharacter()
		:x_(0), y_(0),
		damagePoint_(0), cntSpeed_(0.0),
		atbCnt_(0), atbCntFlag_(false), motionFlag_(false),
		menuSelect_(0), menuSelectY_(0), actionCnt_(false), damageActionCnt_(0),
		actionFlag_(false), animationFlag_(false), damageFlag_(false),
		damage_(0), centerX_(0), centerY_(0)

	{
		for (int i = 0; i < sizeof(abiFlag_); i++)
		{
			abiFlag_[i] = false;
		}
	}

	AbstractBattleCharacter::~AbstractBattleCharacter()
	{
	}

	int AbstractBattleCharacter::getX() const
	{
		return x_;
	}

	void AbstractBattleCharacter::setX(int x)
	{
		x_ = x;
	}

	int AbstractBattleCharacter::getY() const
	{
		return y_;
	}

	void AbstractBattleCharacter::setY(int y)
	{
		y_ = y;
	}

	int AbstractBattleCharacter::getDamagePoint() const
	{
		return damagePoint_;
	}


	// ƒ_ƒ[ƒW‚ÆÅ‘åHP‚æ‚èDamagePoint‚ðŒˆ’è
	void  AbstractBattleCharacter::setDamagePoint()
	{
		int plusDp = damage_ * 1000 / sharedStatus_->hp_;
		if (plusDp < 1)
		{
			damagePoint_++;
		}
		else
		{
			damagePoint_ += plusDp;
		}
		if (damagePoint_ > DamagePointMax)
		{
			damagePoint_ = DamagePointMax;
		}
	}

	int AbstractBattleCharacter::getAtbCnt() const
	{
		return atbCnt_;
	}

	void AbstractBattleCharacter::setAtbCnt(int atbCnt)
	{
		atbCnt_ = atbCnt;
	}

	void AbstractBattleCharacter::increaseAtbCnt()
	{
		atbCnt_++;
		if (atbCnt_ == AtbCntMax)
		{
			atbCntFlag_ = false;
		}
	}

	bool AbstractBattleCharacter::getAtbCntFlag() const
	{
		return atbCntFlag_;
	}

	void AbstractBattleCharacter::setAtbCntFlag(bool atbCntFlag)
	{
		atbCntFlag_ = atbCntFlag;
	}

	bool AbstractBattleCharacter::getMotionFlag() const
	{
		return motionFlag_;
	}

	void AbstractBattleCharacter::setMotionFlag(bool motionFlag)
	{
		motionFlag_ = motionFlag;
	}

	Status &AbstractBattleCharacter::getStatus()
	{
		return status_;
	}

	void AbstractBattleCharacter::setStatus(Status status)
	{
		status_ = status;
	}

	Magic &AbstractBattleCharacter::getAttackMagic()
	{
		return attackMagic_;
	}

	Magic &AbstractBattleCharacter::getCureMagic()
	{
		return cureMagic_;
	}

	int AbstractBattleCharacter::getMenuSelect() const
	{
		return menuSelect_;
	}

	int AbstractBattleCharacter::getMenuSelectY() const
	{
		return menuSelectY_;
	}

	void AbstractBattleCharacter::setMenuSelect(int menuSelect)
	{
		menuSelect_ = menuSelect;
	}
	void AbstractBattleCharacter::setMenuSelectY(int menuSelectY)
	{
		menuSelectY_ = menuSelectY;
	}

	void AbstractBattleCharacter::changeAttackMagic()
	{
		attackMagic_.change();
	}

	void AbstractBattleCharacter::changeCureMagic()
	{
		cureMagic_.change();
	}

	void AbstractBattleCharacter::increaseHp(int hp)
	{
		sharedStatus_->hp_ += hp;
	}

	void AbstractBattleCharacter::decreaseMp(int mp)
	{
		sharedStatus_->mp_ -= mp;
	}

	void AbstractBattleCharacter::useMp()
	{
		sharedStatus_->mp_ -= UseMp[(menuSelect_ - 20000) / 100];
		if (sharedStatus_->mp_ < 0)
		{
			sharedStatus_->mp_ = 0;
		}
	}

	int AbstractBattleCharacter::getHp() const
	{
		return sharedStatus_->hp_;
	}
	
	void AbstractBattleCharacter::setHp(int hp)
	{
		sharedStatus_->hp_ = hp;
	}

	int AbstractBattleCharacter::getHpMax() const
	{
		return sharedStatus_->hpMax_;
	}

	int AbstractBattleCharacter::getMp() const
	{
		return sharedStatus_->mp_;
	}

	int AbstractBattleCharacter::getMpMax() const
	{
		return sharedStatus_->mpMax_;
	}

	int AbstractBattleCharacter::getMagic() const
	{
		return sharedStatus_->magic_;
	}

	int AbstractBattleCharacter::getKind() const
	{
		return sharedStatus_->kind_;
	}

	void AbstractBattleCharacter::setKind(int kind)
	{
		sharedStatus_->kind_ = kind;
	}

	void AbstractBattleCharacter::getOffenseParam(
		int *power, int *strength, int *weapon, int *level)
	{
		*power = sharedStatus_->power_ + status_.power;
		*strength = sharedStatus_->strength_ + status_.strength;
		*weapon = sharedStatus_->weapon_ + status_.weapon;
		*level = sharedStatus_->level_;
	}
	void AbstractBattleCharacter::getDefenseParam(
		int *defense, int *equip)
	{
		*defense = sharedStatus_->defense_ + status_.defense;
		*equip = sharedStatus_->equip_ + status_.equip;
	}

	int AbstractBattleCharacter::getActionCnt() const
	{
		return actionCnt_;
	}

	void AbstractBattleCharacter::setActionCnt(int actionCnt)
	{
		actionCnt_ = actionCnt;
	}

	void AbstractBattleCharacter::increaseActionCnt()
	{
		actionCnt_++;
	}

	int AbstractBattleCharacter::getDamageActionCnt() const
	{
		return damageActionCnt_;
	}

	void AbstractBattleCharacter::setDamageActionCnt(int damageActionCnt)
	{
		damageActionCnt_ = damageActionCnt;
	}

	void AbstractBattleCharacter::increaseDamageActionCnt()
	{
		damageActionCnt_++;
	}

	void AbstractBattleCharacter::increaseX()
	{
		x_++;
	}

	void AbstractBattleCharacter::decreaseX()
	{
		x_--;
	}

	bool AbstractBattleCharacter::getActionFlag() const
	{
		return actionFlag_;
	}

	void AbstractBattleCharacter::setActionFlag(bool actionFlag)
	{
		actionFlag_ = actionFlag;
	}

	bool AbstractBattleCharacter::getAnimationFlag() const
	{
		return animationFlag_;
	}

	void AbstractBattleCharacter::setAnimationFlag(bool animationFlag)
	{
		animationFlag_ = animationFlag;
	}

	bool AbstractBattleCharacter::getDamageFlag() const
	{
		return damageFlag_;
	}

	void AbstractBattleCharacter::setDamageFlag(bool damageFlag)
	{
		damageFlag_ = damageFlag;
	}

	int AbstractBattleCharacter::getDamage() const
	{
		return damage_;
	}

	void AbstractBattleCharacter::setDamage(int damage)
	{
		damage_ = damage;
	}

	int AbstractBattleCharacter::getCenterX() const
	{
		return centerX_;
	}
	void AbstractBattleCharacter::setCenterX(int centerX)
	{
		centerX_ = centerX;
	}
	int AbstractBattleCharacter::getCenterY() const
	{
		return centerY_;
	}
	void AbstractBattleCharacter::setCenterY(int centerY)
	{
		centerY_ = centerY;
	}

}