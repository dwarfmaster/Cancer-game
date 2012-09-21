
#include "dropDown.hpp"

#include "listModel.hpp"
#include "selectListener.hpp"

namespace graphics
{
	DropDown::DropDown()
		: gcn::DropDown()
	{
		m_model = new internal::ListModel;
		setListModel(m_model);

		m_listener = new internal::SelectListener(true);
		m_listener->setModel(m_model);
		addSelectionListener(m_listener);
	}

	DropDown::DropDown(const std::vector<std::string>& values)
		: gcn::DropDown()
	{
		m_model = new internal::ListModel(values);
		setListModel(m_model);

		m_listener = new internal::SelectListener(true);
		m_listener->setModel(m_model);
		addSelectionListener(m_listener);
	}

	DropDown::~DropDown()
	{
		delete m_model;
		delete m_listener;
	}

	void DropDown::setCB(const cb_t& callback)
	{
		m_listener->set(callback);
	}

	void DropDown::clearCB()
	{
		m_listener->clear();
	}

	bool DropDown::isCB() const
	{
		return m_listener->isSet();
	}

	std::vector<std::string>* DropDown::operator->()
	{
		return &m_model->values;
	}

	const std::vector<std::string>* DropDown::operator->() const
	{
		return &m_model->values;
	}

	std::vector<std::string>& DropDown::operator*()
	{
		return m_model->values;
	}

	const std::vector<std::string>& DropDown::operator*() const
	{
		return m_model->values;
	}

};//namespace graphics

