#pragma once

namespace Util
{
	class GameController;
	class ResourceLoader;
}
namespace Battle
{
	constexpr int  BattleStringLenMax = 32;

	constexpr int  MenuNum = 3;
	constexpr int  SubMenuNum = 5;

	constexpr char Main[MenuNum][BattleStringLenMax] =
	{ "攻撃", "魔法", "逃げる" };

	constexpr char Sub[MenuNum][SubMenuNum][BattleStringLenMax] =
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
		void drawMonsterBar(BattleMonster *mon);
		void drawCharacterBar(BattleCharacter *chr);

		void drawMenu(BattleCharacter *chr);
		void drawGameover() const;
		void drawWin() const;

	private:
		Util::GameController &gc_;
		Util::ResourceLoader &rl_;

		void drawBar(int x, int y, int hp, int hpMax, int mp, int mpMax, int atbCnt, int dp);
		void decorateMeter(int x, int y, int h, int len);
		char *decideBoardString(char *message, BattleCharacter *chr);
	};
}
