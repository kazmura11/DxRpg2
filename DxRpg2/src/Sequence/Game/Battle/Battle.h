#pragma once
#include <memory>
#include "StdCommon.h"
#include "Sequence/Game/IChild.h"
#include "SharedCharacterStatus.h"  // 不完全クラス型へのポインターが参照できないそうなので

namespace Util
{
	class GameController;
	class ResourceLoader;
}
namespace Battle
{
	class BattleProcess;
	class BattleCharacter;
	class BattleMonster;
}

namespace Sequence
{
	namespace Game
	{
		namespace Battle
		{
			enum class NextSequence
			{
				NextMap,
				NextBattleShatter,
				NextBattleEnter,
				NextBattleMain,
				NextBattleDebug,
				NextRestart,
			};

			class Battle : public Sequence::Game::IChild
			{
			public:
				Battle(SharedCharacterStatus *sharedStatus);
				~Battle();
				void update(Sequence::Game::Parent *parent);

			private:
				void startBattleShatter();
				void startBattleEnter();

				NextSequence nextSequence_;
				Util::GameController &gc_;
				Util::ResourceLoader &rl_;
				std::unique_ptr<::Battle::BattleProcess> bp_;

				double startBattleX_[ImgStartBattleDivNum];
				double startBattleY_[ImgStartBattleDivNum];
				double randX_[ImgStartBattleDivNum];
				double randY_[ImgStartBattleDivNum];

				std::unique_ptr<::Battle::BattleCharacter> chr_;
				std::unique_ptr<::Battle::BattleMonster> mon_;
			};

		}

	}
}
