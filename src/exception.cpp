#include "exception.hpp"

#include <SDL/SDL.h>
#include "i18n.hpp"

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
			// Normalement n'a pas besoin de traduction
			m_err += _i(" : ");
			m_err += SDL_GetError();
			// Normallement n'a pas besoin de traduction.
			m_err += _i(".");
			break;
		default:
			// Normallement n'a pas besoin de traduction.
			m_err += _i(".");
			break;
	};
}

const char* Exception::what() const _GLIBCXX_USE_NOEXCEPT
{
	return m_err.c_str();
}

