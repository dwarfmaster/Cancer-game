
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

		for(WidgetListIterator it = ws.begin(); it != ws.end(); ++it)
			delete *it;
	}

};//namespace graphics

