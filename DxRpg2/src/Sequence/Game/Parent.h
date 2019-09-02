#pragma once
#include <memory>
#include "Sequence/Child.h"
#include "Map/MapMainCharacter.h"
#include "Map/MapCityCharacter.h"  // 配列があるからこれないとダメ
#include "SharedCharacterStatus.h"  // 不完全クラス型へのポインターが参照できないそうなので

namespace Sequence
{
	namespace Game
	{
		class Child;
		typedef Sequence::Parent GrandParent;

		class Parent : public Sequence::Child
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
			std::unique_ptr<::Map::MapMainCharacter> mapMainChar_;
			::Map::MapCityCharacter *mapCityChar_[CompCharMax];  // 配列のunique_ptrは簡単にはできないので諦める

		private:
			NextSequence nextSequence_;
			std::unique_ptr <Game::Child> child_;
			std::unique_ptr <::SharedCharacterStatus> sharedStatus_;
			int  mapStage_;
			void initializeParameter();
		};
	}
}
