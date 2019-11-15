#pragma once

namespace Sequence
{
	class Parent;
	struct IChild
	{
		virtual ~IChild() { }
		virtual void update(Parent *) = 0;
	};
}