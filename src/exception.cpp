#include "exception.hpp"

#include <SDL/SDL.h>

Exception::Exception()
{
	set("");
}

Exception::Exception(const std::string& err, const ErrType& errt)
{
	set(err, errt);
}

void Exception::set(const std::string& err, const ErrType& errt)
{
	m_err = err;

	switch(errt)
	{
		case SDL:
			m_err += " : ";
			m_err += SDL_GetError();
			m_err += '.';
			break;
		default:
			m_err += '.';
			break;
	};
}

const char* Exception::what() const _GLIBCXX_USE_NOEXCEPT
{
	return m_err.c_str();
}

