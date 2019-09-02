#pragma once
#include "AbstractMapCharacter.h"
#include "SharedCharacterStatus.h"  // 不完全クラス型へのポインターが参照できないそうなので

namespace Map
{
	const int	EncRand = 4;  // バトルに突入する確率
	const int	EncMinWalkCnt = 4;  // バトルに突入する最低歩数

	class MapMainCharacter : public AbstractMapCharacter
	{
	public:
		MapMainCharacter();
		~MapMainCharacter() override;
		void initMapState(int tx, int ty, int dir, int kind) override;
		bool move(int *isPassable) override;
		// 未実装項目SharedCharacterStatusを参照できるようにする

	private:
		int  prevX_;  // 前回いたX座標
		int  prevY_;  // 前回いたY座標
		int  encCnt_;  // 敵遭遇カウント
		bool encountersEnemy();
	};
}