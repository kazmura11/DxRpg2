#pragma once

namespace Sequence
{
	class Parent;
	class Child
	{
	public:
		virtual ~Child() { }
		virtual void update(Parent *) = 0;
	};
}