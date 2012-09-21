
#include "textField.hpp"
#include "focusListener.hpp"
#include <boost/bind.hpp>

namespace graphics
{
	TextField::TextField()
		: gcn::TextField(), m_last(""), m_message(""), m_empty(true)
	{
		setupListener();
	}

	TextField::TextField(const std::string& caption)
		: gcn::TextField(caption), m_last(caption), m_message("")
	{
		m_empty = caption.empty();
		setupListener();
	}

	TextField::~TextField()
	{
		delete m_listener;
	}

	void TextField::setChangeCB(const cb_t& cb)
	{
		m_changedCB = cb;
	}

	void TextField::clearChangeCB()
	{
		m_changedCB.clear();
	}

	bool TextField::isChangeCB() const
	{
		return !m_changedCB.empty();
	}

	void TextField::setValidCB(const cb_t& cb)
	{
		m_validCB = cb;
	}

	void TextField::clearValidCB()
	{
		m_validCB.clear();
	}

	bool TextField::isValidCB() const
	{
		return !m_validCB.empty();
	}

	void TextField::setEmptyMessage(const std::string& msg)
	{
		m_message = msg;
		update();
	}

	std::string TextField::emptyMessage() const
	{
		return m_message;
	}

	void TextField::keyPressed(gcn::KeyEvent& ev)
	{
		if( m_empty )
			setText("");

		bool valid = false;
		if( ev.getKey().getValue() == gcn::Key::ENTER
				&& m_validCB )
			valid = true;

		gcn::TextField::keyPressed(ev);

		if( valid )
			m_validCB( getText() );
		update();
	}

	void TextField::update()
	{
		std::string txt = getText();

		if( txt.empty() )
		{
			m_empty = true;
			if( isFocused() )
				setText("");
			else
				setText(m_message);
		}
		else
			m_empty = false;

		if( txt != m_last )
		{
			if( m_changedCB )
				m_changedCB(txt);
			m_last = txt;
		}
	}
	
	void TextField::setupListener()
	{
		m_listener = new graphics::internal::FocusListener(
				boost::bind(&graphics::TextField::changeFocus, this, _1) );
		addFocusListener(m_listener);
	}

	void TextField::changeFocus(bool foc)
	{
		if( m_empty )
		{
			if( foc )
				setText("");
			else
				setText(m_message);
		}
	}

};//namespace graphics

