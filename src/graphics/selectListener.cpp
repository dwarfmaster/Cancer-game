
#include "selectListener.hpp"

#include "listModel.hpp"

namespace graphics
{
	namespace internal
	{
		SelectListener::SelectListener(bool dd)
			: gcn::SelectionListener(), m_model(NULL), m_dd(dd)
		{}

		SelectListener::SelectListener(const boost::function<void (std::string)>& callback, bool dd)
			: gcn::SelectionListener(), m_callback(callback), m_model(NULL), m_dd(dd)
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
				int s;

				if(m_dd)
				{
					gcn::DropDown* source = dynamic_cast<gcn::DropDown*>( ev.getSource() );
					s = source->getSelected();
				}
				else
				{
					gcn::ListBox* source = dynamic_cast<gcn::ListBox*>( ev.getSource() );
					s = source->getSelected();
				}

				gcn::ListModel* model;
				if( m_model != NULL )
					model = m_model;
				else
				{
					if(m_dd)
					{
						gcn::DropDown* source = dynamic_cast<gcn::DropDown*>( ev.getSource() );
						model = source->getListModel();
					}
					else
					{
						gcn::ListBox* source = dynamic_cast<gcn::ListBox*>( ev.getSource() );
						model = source->getListModel();
					}
				}

				std::string select = model->getElementAt(s);

				m_callback(select);
			}
		}

		void SelectListener::setModel(ListModel* mod)
		{
			m_model = mod;
		}

		void SelectListener::clearModel()
		{
			m_model = NULL;
		}

		bool SelectListener::isModel() const
		{
			return m_model != NULL;
		}

	};//namespace internal
};//namespace graphics


