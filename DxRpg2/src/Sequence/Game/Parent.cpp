#include "Sequence/Game/Parent.h"
#include "Sequence/Game/Map/Map.h"
#include "Sequence/Game/Battle/Battle.h"
#include "Sequence/Parent.h"

namespace Sequence
{
	namespace Game
	{
		Parent::Parent()
			: nextSequence_(NextNone), mapStage_(0),
			sharedStatus_(std::make_unique<::SharedCharacterStatus>())
		{
			sharedStatus_->initialize();
			child_ = std::make_unique<Map::Map>(sharedStatus_.get(), mapStage_);
		}

		Parent::~Parent()
		{
		}

		void Parent::update(GrandParent* grandParent)
		{
			child_->update(this);  // change game sequence here!!
			switch (nextSequence_)
			{
			case NextMapMain:
				child_.reset();
				child_ = std::make_unique<Map::Map>(sharedStatus_.get(), mapStage_);
				break;
			case NextBattle:
				child_.reset();
				child_ = std::make_unique<Battle::Battle>(sharedStatus_.get());
				break;
			case NextRestart:
				grandParent->moveTo(GrandParent::NextMap);
				break;
			case NextNone:
				break;
			default:
				exit(EXIT_FAILURE);
				break;
			}
			nextSequence_ = NextNone;
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
