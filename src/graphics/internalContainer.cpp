
#include "internalContainer.hpp"

namespace graphics
{
	InternalContainer::InternalContainer()
		: Container()
	{}

	InternalContainer::~InternalContainer()
	{
		WidgetList ws = mWidgets;
		clear();

		for(WidgetListIterator it = mWidgets.begin(); it != mWidgets.end(); ++it)
			delete *it;
	}

};//namespace graphics

