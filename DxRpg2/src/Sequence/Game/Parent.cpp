#include "Sequence/Game/Parent.h"
#include "Sequence/Game/Map/Map.h"
#include "Sequence/Game/Battle/Battle.h"
#include "Map/MapMainCharacter.h"
#include "Map/MapCityCharacter.h"
#include "Sequence/Parent.h"

namespace Sequence
{
	namespace Game
	{
		Parent::Parent()
			: nextSequence_(NextNone), mapStage_(0),
			child_(std::make_unique<Map::Map>(this, mapStage_)),
			sharedStatus_(std::make_unique<::SharedCharacterStatus>()),
			mapMainChar_(std::make_unique<::Map::MapMainCharacter>())
		{
			initializeParameter();
			for (int i = 0; i < CompCharMax; i++)
			{
				mapCityChar_[i] = new ::Map::MapCityCharacter();
			}
		}

		Parent::~Parent()
		{
			for (int i = 0; i < CompCharMax; i++)
			{
				if (mapCityChar_[i] != NULL)
				{
					delete mapCityChar_[i];
					mapCityChar_[i] = NULL;
				}
			}
		}

		void Parent::update(GrandParent* grandParent)
		{
			child_->update(this);
			switch (nextSequence_)
			{
			case NextMapMain:
				child_.reset();
				child_ = std::make_unique<Map::Map>(this, mapStage_);
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

		void Parent::initializeParameter()
		{
		#if _DEBUG
			sharedStatus_->hp_ = 1352;
		#else
			sharedStatus_->hp_ = 1352;
		#endif
			sharedStatus_->hpMax_ = 1352;
			sharedStatus_->mp_ = 97;
			sharedStatus_->mpMax_ = 102;
			sharedStatus_->power_ = 20;
			sharedStatus_->defense_ = 20;
			sharedStatus_->strength_ = 20;
			sharedStatus_->magic_ =  20;
			sharedStatus_->weapon_ = 20;
			sharedStatus_->equip_ = 20;
			sharedStatus_->level_ = 10;
		}

		void Parent::setMapStage(int mapStage)
		{
			mapStage_ = mapStage;
		}

	}
}
