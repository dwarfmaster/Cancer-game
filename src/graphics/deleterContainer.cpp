
#include "deleterContainer.hpp"

namespace graphics
{
	DeleterContainer::DeleterContainer()
		: Container()
	{}

	DeleterContainer::~DeleterContainer()
	{
		WidgetList ws = mWidgets;
		clear();

		for(WidgetListIterator it = ws.begin(); it != ws.end(); ++it)
			delete *it;
	}

};//namespace graphics

