#pragma once

namespace Sequence
{
	namespace Game
	{
		class Parent;
		class Child
		{
		public:
			virtual ~Child() { }
			virtual void update(Parent*) = 0;
		};
	}
}