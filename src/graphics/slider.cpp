
#include "slider.hpp"

namespace graphics
{
	Slider::Slider(double end)
		: gcn::Slider(end), m_last(0.0), m_limit(1.0)
	{}

	Slider::Slider(double start, double end)
		: gcn::Slider(start, end), m_last(start), m_limit(1.0)
	{}

	void Slider::keyPressed(gcn::KeyEvent& ev)
	{
		gcn::Slider::keyPressed(ev);
		update();
	}

	void Slider::mouseDragged(gcn::MouseEvent& ev)
	{
		gcn::Slider::mouseDragged(ev);
		update();
	}

	void Slider::mousePressed(gcn::MouseEvent& ev)
	{
		gcn::Slider::mousePressed(ev);
		update();
	}

	void Slider::mouseWheelMovedDown(gcn::MouseEvent& ev)
	{
		gcn::Slider::mouseWheelMovedDown(ev);
		update();
	}

	void Slider::mouseWheelMovedUp(gcn::MouseEvent& ev)
	{
		gcn::Slider::mouseWheelMovedUp(ev);
		update();
	}

	void Slider::setLimit(double nlimit)
	{
		m_limit = nlimit;
	}

	double Slider::limit() const
	{
		return m_limit;
	}

	void Slider::set(const callback_t& cb)
	{
		m_callback = cb;
	}

	void Slider::clear()
	{
		m_callback.clear();
	}

	bool Slider::isSet() const
	{
		return !m_callback.empty();
	}

	void Slider::update()
	{
		double value = getValue();
		double diff = m_last > value ? m_last - value : value - m_last;

		if( diff >= m_limit 
				|| (value == mScaleEnd && m_last != value)
				|| (value == mScaleStart && m_last != value) )
		{
			if( m_callback )
				m_callback(value);
			m_last = value;
		}
	}
};//namespace graphics

