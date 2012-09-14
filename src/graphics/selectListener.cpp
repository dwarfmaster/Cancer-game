
#include "selectListener.hpp"

namespace graphics
{
	namespace internal
	{
		SelectListener::SelectListener()
			: gcn::SelectionListener()
		{}

		SelectListener::SelectListener(const boost::function<void (std::string)>& callback)
			: gcn::SelectionListener(), m_callback(callback)
		{}

		void SelectListener::set(const boost::function<void (std::string)>& callback)
		{
			m_callback = callback;
		}

		void SelectListener::clear()
		{
			m_callback.clear();
		}

		bool SelectListener::isSet()
		{
			return !m_callback.empty();
		}

		void SelectListener::valueChanged(const gcn::SelectionEvent& ev)
		{
			if( m_callback )
			{
				gcn::ListBox* source = dynamic_cast<gcn::ListBox*>( ev.getSource() );
				int s = source->getSelected();
				std::string select = source->getListModel()->getElementAt(s);

				m_callback(select);
			}
		}

	};//namespace internal
};//namespace graphics


