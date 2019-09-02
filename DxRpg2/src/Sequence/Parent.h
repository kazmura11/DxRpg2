#pragma once
#include <memory>
#include "Child.h"
#include "Sequence/Game/Parent.h"

namespace Sequence
{
	class Parent
	{
	public:
		enum NextSequence
		{
			NextMap,
			NextNone,
		};

		void update()
		{
			child_->update(this);
			switch (nextSequence_)
			{
			case NextMap:
				child_.reset();
				child_ = std::make_unique<Game::Parent>();
				break;
			case NextNone:
				break;
			default:
				exit(EXIT_FAILURE);
				break;
			}
			nextSequence_ = NextNone;
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
			: nextSequence_(NextNone),
			child_(std::make_unique<Game::Parent>())
		{
		}

		~Parent()
		{
		}

		Parent(const Parent&) { }
		Parent& operator=(const Parent&) { }
		NextSequence nextSequence_;
		std::unique_ptr<Child> child_;
	};
}
