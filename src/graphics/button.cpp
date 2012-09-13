
#include "button.hpp"

namespace graphics
{
	Button::Button()
		: gcn::Button()
	{}

	Button::Button(const std::string& caption)
		: gcn::Button(caption)
	{}

	Button::Button(const std::string& caption, const boost::function<void ()>& callback)
		: gcn::Button(caption)
	{
		set(callback);
	}

	bool Button::isSet() const
	{
		return !m_callback.empty();
	}

	void Button::set(const boost::function<void ()>& callback)
	{
		m_callback = callback;
	}

	void Button::clear()
	{
		m_callback.clear();
	}

	void Button::keyPressed(gcn::KeyEvent &keyEvent)
	{
		update();
		gcn::Button::keyPressed(keyEvent);
	}

	void Button::keyReleased(gcn::KeyEvent &keyEvent)
	{
		update();
		gcn::Button::keyReleased(keyEvent);
	}

	void Button::mousePressed(gcn::MouseEvent &mouseEvent)
	{
		update();
		gcn::Button::mousePressed(mouseEvent);
	}

	void Button::mouseReleased(gcn::MouseEvent &mouseEvent)
	{
		update();
		gcn::Button::mouseReleased(mouseEvent);
	}

};//namespace graphics

