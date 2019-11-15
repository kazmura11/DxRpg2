#pragma once
#include <memory>
#include "Sequence/IChild.h"
//#include "Map/MapMainCharacter.h"
//#include "Map/MapCityCharacter.h"  // 配列があるからこれないとダメ
#include "SharedCharacterStatus.h"  // 不完全クラス型へのポインターが参照できないそうなので

namespace Sequence
{
	namespace Game
	{
		struct IChild;
		typedef Sequence::Parent GrandParent;

		class Parent : public Sequence::IChild
		{
		public:
			enum NextSequence{
				NextMapMain,
				NextBattle,
				NextRestart,
				NextNone,
			};
			Parent();
			~Parent();
			void update(GrandParent* grandParent);
			void moveTo(NextSequence nextSequence);
			void setMapStage(int mapStage);

		private:
			NextSequence nextSequence_;
			std::unique_ptr<Game::IChild> child_;
			std::unique_ptr<::SharedCharacterStatus> sharedStatus_;
			int  mapStage_;
		};
	}
}
