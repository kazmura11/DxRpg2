#pragma once

namespace Util
{
	class GameController;
	class ResourceLoader;
}
namespace Battle
{
	const int  BattleStringLenMax = 32;

	const int  MenuNum = 3;
	const int  SubMenuNum = 5;

	const char Main[MenuNum][BattleStringLenMax] =
	{ "攻撃", "魔法", "逃げる" };

	const char Sub[MenuNum][SubMenuNum][BattleStringLenMax] =
	{
		{ "通常攻撃", "テクニカルアタック", "", "", "" },
		{ "キュア", "エアー", "ファイア", "フリーザ", "サンダー" },
		{ "逃げる", "", "", "", "" }
	};

	class BattleCharacter;
	class BattleMonster;

	/**
	 * 描画系のクラス
	 * 攻撃・魔法を除く、キャラクター、モンスターのイメージ
	 * バーの描画などを行う
	 */
	class BattleGraph
	{
	public:
		BattleGraph();
		~BattleGraph();
		void drawBackground() const;
		void drawBottomWindow() const;
		void drawMonsterImage(BattleMonster *mon) const;
		void drawCharacterImage(BattleCharacter *chr) const;
		void graphMonsterBar(BattleMonster *mon);
		void graphCharacterBar(BattleCharacter *chr);

		void graphMenu(BattleCharacter *chr);
		void graphGameover() const;
		void graphWin() const;

	private:
		Util::GameController &gc_;
		Util::ResourceLoader &rl_;

		void graphBar(int x, int y, int hp, int hpMax, int mp, int mpMax, int atbCnt, int dp);
		void decorateMeter(int x, int y, int h, int len);
		char *decideBoardString(char *message, BattleCharacter *chr);
	};
}
