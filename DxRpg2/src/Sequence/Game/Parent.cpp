#include "Sequence/Game/Parent.h"
#include "Sequence/Game/Map/Map.h"
#include "Sequence/Game/Battle/Battle.h"
#include "Sequence/Parent.h"

namespace Sequence
{
	namespace Game
	{
		Parent::Parent()
			: nextSequence_(NextSequence::NextNone), mapStage_(0),
			sharedStatus_(std::make_shared<::SharedCharacterStatus>())
		{
			sharedStatus_->initialize();
			child_ = std::make_unique<Map::Map>(sharedStatus_, mapStage_);
		}

		Parent::~Parent()
		{
		}

		void Parent::update(GrandParent* grandParent)
		{
			child_->update(this);  // change game sequence here!!
			switch (nextSequence_)
			{
			case NextSequence::NextMapMain:
				child_.reset();
				child_ = std::make_unique<Map::Map>(sharedStatus_, mapStage_);
				break;
			case NextSequence::NextBattle:
				child_.reset();
				child_ = std::make_unique<Battle::Battle>(sharedStatus_);
				break;
			case NextSequence::NextRestart:
				grandParent->moveTo(GrandParent::NextSequence::NextMap);
				break;
			case NextSequence::NextNone:
				break;
			default:
				exit(EXIT_FAILURE);
				break;
			}
			nextSequence_ = NextSequence::NextNone;
		}

		void Parent::moveTo(NextSequence nextSequence)
		{
			nextSequence_ = nextSequence;
		}

		void Parent::setMapStage(int mapStage)
		{
			mapStage_ = mapStage;
		}

	}
}
