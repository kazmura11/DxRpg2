#pragma once
#include <memory>
#include "IChild.h"
#include "Sequence/Game/Parent.h"

namespace Sequence
{
	class Parent
	{
	public:
		enum class NextSequence
		{
			NextMap,
			NextNone,
		};

		void update()
		{
			child_->update(this);
			switch (nextSequence_)
			{
			case NextSequence::NextMap:
				child_.reset();
				child_ = std::make_unique<Game::Parent>();
				break;
			case NextSequence::NextNone:
				break;
			default:
				exit(EXIT_FAILURE);
				break;
			}
			nextSequence_ = NextSequence::NextNone;
		}

		void moveTo(NextSequence next)
		{
			nextSequence_ = next;
		}

		static Parent* getInstance()
		{
			static Parent parent;
			return &parent;
		}

	private:
		Parent()
			: nextSequence_(NextSequence::NextNone),
			child_(std::make_unique<Game::Parent>())
		{
		}

		~Parent()
		{
		}

		Parent(const Parent&): nextSequence_(NextSequence::NextNone) { }
		Parent& operator=(const Parent&) { }
		NextSequence nextSequence_;
		std::unique_ptr<IChild> child_;
	};
}
