#pragma once

class SharedCharacterStatus
{
public:
	SharedCharacterStatus()
		:hp_(0), hpMax_(0), mp_(0), mpMax_(0),
		power_(0), defense_(0), strength_(0), magic_(0),
		weapon_(0), equip_(0), level_(0), kind_(0) { }
	~SharedCharacterStatus() { }
	// よく使うからpublicのまま、privateにする意味もない。
public:
	int hp_;
	int hpMax_;
	int mp_;
	int mpMax_;
	int power_;
	int defense_;
	int strength_;
	int magic_;
	int weapon_;
	int equip_;
	int level_;
	int kind_;	// キャラクター・モンスターの種類
};

