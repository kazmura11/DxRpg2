#pragma once
#include "AbstractMapCharacter.h"
#include "SharedCharacterStatus.h"  // 不完全クラス型へのポインターが参照できないそうなので

namespace Map
{
	// 町キャラクタのクラス
	class MapCityCharacter : public AbstractMapCharacter
	{
	public:
		MapCityCharacter() { }
		~MapCityCharacter() override { }

		void initMapState(int tx, int ty, Direction dir, int kind) override;
		bool move(PassFlag *isPassable) override;
	};
}