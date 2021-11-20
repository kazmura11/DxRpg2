#pragma once
#include <memory>
#include "Sequence/IChild.h"
#include "SharedCharacterStatus.h"  // for Pointer to incomplete class type

namespace Sequence
{
	namespace Game
	{
		struct IChild;
		typedef Sequence::Parent GrandParent;

		class Parent : public Sequence::IChild
		{
		public:
			enum class NextSequence
			{
				NextMapMain,
				NextBattle,
				NextRestart,
				NextNone,
			};
			Parent();
			virtual ~Parent() override;
			void update(GrandParent* grandParent);
			void moveTo(NextSequence nextSequence);
			void setMapStage(int mapStage);

		private:
			NextSequence nextSequence_;
			std::unique_ptr<Game::IChild> child_;
			std::shared_ptr<::SharedCharacterStatus> sharedStatus_;
			int  mapStage_;
		};
	}
}
