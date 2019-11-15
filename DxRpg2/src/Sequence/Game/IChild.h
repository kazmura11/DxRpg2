#pragma once

namespace Sequence
{
	namespace Game
	{
		class Parent;
		struct IChild
		{
			virtual ~IChild() { }
			virtual void update(Parent*) = 0;
		};
	}
}