
#include "list.hpp"

namespace graphics
{
	List::List()
		: gcn::ListBox()
	{
		m_model = new internal::ListModel;
		setListModel(m_model);

		m_listener = new internal::SelectListener;
		addSelectionListener(m_listener);
	}

	List::List(const std::vector<std::string>& values)
		: gcn::ListBox()
	{
		m_model = new internal::ListModel(values);
		setListModel(m_model);

		m_listener = new internal::SelectListener;
		addSelectionListener(m_listener);
	}

	List::~List()
	{
		delete m_model;
		delete m_listener;
	}

	std::vector<std::string>* List::operator->()
	{
		return &m_model->values;
	}

	const std::vector<std::string>* List::operator->() const
	{
		return &m_model->values;
	}

	std::vector<std::string>& List::operator*()
	{
		return m_model->values;
	}

	const std::vector<std::string>& List::operator*() const
	{
		return m_model->values;
	}

	void List::setCB(const cb_t& callback)
	{
		m_listener->set(callback);
	}

	void List::clearCB()
	{
		m_listener->clear();
	}

	bool List::isCB() const
	{
		return m_listener->isSet();
	}

};//namespace graphics

