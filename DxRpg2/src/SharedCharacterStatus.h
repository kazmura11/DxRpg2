#pragma once

class SharedCharacterStatus
{
public:
	SharedCharacterStatus()
		:hp_(0), hpMax_(0), mp_(0), mpMax_(0),
		power_(0), defense_(0), strength_(0), magic_(0),
		weapon_(0), equip_(0), level_(0), kind_(0) { }
	~SharedCharacterStatus() { }
	// dare to make "public"
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
	int kind_;   // character / enemy kind

	void initialize()
	{
#if _DEBUG
		hp_ = 400;
#else
		hp_ = 1352;
#endif
		hpMax_ = 1352;
		mp_ = 97;
		mpMax_ = 102;
		power_ = 20;
		defense_ = 20;
		strength_ = 20;
		magic_ = 20;
		weapon_ = 20;
		equip_ = 20;
		level_ = 10;
	}
};

