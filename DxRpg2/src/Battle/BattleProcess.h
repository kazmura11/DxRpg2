#pragma once
#include <memory>

namespace Util
{
	class GameController;
	class ResourceLoader;
}
namespace Battle
{
	enum class CharacterEnemyKind
	{
		Character,
		Enemy,
	};

	class AbstractBattleCharacter;
	class BattleCharacter;
	class BattleMonster;
	class BattleAnimation;
	class BattleGraph;
	class Board;

	/**
	 * êÌì¨ÉÅÉCÉìèàóùÇÃÉNÉâÉX
	 */
	class BattleProcess
	{
	public:
		BattleProcess(BattleCharacter *chr, BattleMonster *mon);
		~BattleProcess();

		void main();
		bool getWinMoveNext() const;
		bool getGameoverMoveNext() const;

	private:
		void calculateAtb();
		void selectMenu();
		void doAction(AbstractBattleCharacter *self,
			AbstractBattleCharacter *opponent, CharacterEnemyKind k);

		int attackAction(AbstractBattleCharacter *my, AbstractBattleCharacter *you, CharacterEnemyKind k);
		int cureAction(AbstractBattleCharacter *c);

		int decideDamage(AbstractBattleCharacter *c, int to, int actKind);

		void decideGameover();
		void decideWin();
		void graphBattle();

		void displayMeter();
		void decreaseHpAction(AbstractBattleCharacter *c);

		void graphGameover();
		void graphWin();

		double genRandom(double sigma, double average);

		Util::GameController &gc_;
		Util::ResourceLoader &rl_;
		BattleMonster *mon_;
		BattleCharacter *chr_;

		std::unique_ptr<BattleAnimation> ba_;
		std::unique_ptr<BattleGraph> bg_;

		std::unique_ptr<Board> board_;

		bool isGameover_;
		bool isWin_;
		bool isGameoverMoveNext_;
		bool isWinMoveNext_;
	};
}

