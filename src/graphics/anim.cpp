#include "anim.hpp"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <tinyxml.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/operations.hpp>
#include <sstream>
#include "../core/exception.hpp"
#include "../i18n.hpp"

namespace graphics
{
	Anim::Anim()
		: m_count(NULL)
	{
		clear();
	}

	Anim::Anim(const Anim& cp)
		: m_count(NULL)
	{
		set(cp);
	}

	Anim::Anim(const boost::filesystem::path& path)
		: m_count(NULL)
	{
		load(path);
	}

	Anim::~Anim()
	{
		clear();
	}

	Anim& Anim::operator=(const Anim& cp)
	{
		set(cp);
		return *this;
	}

	void Anim::set(const Anim& cp)
	{
		m_frames = cp.m_frames;
		m_count = cp.m_count;
		if( m_count != NULL )
			++(*m_count);
		m_time = cp.m_time;

		m_act = cp.m_act;
		m_paused = cp.m_paused;
		m_lastTime = cp.m_lastTime;
		m_timeStay = cp.m_timeStay;
	}

	void Anim::load(const boost::filesystem::path& path)
	{
		clear();

		// On obtient le chemin du fichier
		boost::filesystem::path filepath = path.parent_path();
		if( filepath.empty() )
			filepath = "./";
		else
			filepath += "/";

		// On ouvre le fichier
		TiXmlDocument file(path.string());
		if(!file.LoadFile())
		{
			std::ostringstream oss;
			oss << _i("Error tinyxml #") << file.ErrorId() << _i(" while opening ") << path.string() << _i(" : \"") << file.ErrorDesc() << _i("\"");
			throw core::Exception( oss.str() );
		}

		// On cherche l'anim
		TiXmlElement* elem = file.FirstChildElement("anim");
		if( !elem )
		{
			std::ostringstream oss;
			oss << _i("Error tinyxml while getting the anim node") << std::endl;
			throw core::Exception( oss.str() );
		}

		// On obtient le temps
		if( !elem->Attribute("time") )
			m_time = 1000;
		else
			elem->QueryValueAttribute<Uint32>("time", &m_time);

		// On charge les images
		m_count = new size_t;
		*m_count = 1;

		elem = elem->FirstChildElement("frame");
		while( elem )
		{
			const char* p = elem->GetText();
			
			if( p )
			{
				boost::filesystem::path imgpath = p;
				imgpath = filepath / imgpath;

				if( boost::filesystem::exists(imgpath)
						&& boost::filesystem::is_regular(imgpath)
						&& !boost::filesystem::is_empty(imgpath) )
				{
					SDL_Surface* img = IMG_Load( imgpath.string().c_str() );
					if( !img )
					{
						clear();
						std::ostringstream oss;
						// Un chemin sera affiche apres picture
						oss << _i("Error while loading the picture ") << imgpath.string() << _i(" : ") << IMG_GetError();
						throw core::Exception( oss.str() );
					}
					else
						m_frames.push_back(img);
				}
				else
				{
					clear();
					std::ostringstream oss;
					// Après picture sera affiche un chemin
					oss << _i("Error with the picture ") << imgpath.string() << _i(" : is not valid");
					throw core::Exception( oss.str() );
				}
			}
			else
			{
				clear();
				std::ostringstream oss;
				oss << _i("Error tinyxml while getting an frame node");
				throw core::Exception( oss.str() );
			}

			elem = elem->NextSiblingElement("frame");
		}

		// On donne les bonnes valeurs aux attributs
		m_lastTime = SDL_GetTicks();
		m_timeStay = m_time;
	}

	void Anim::clear()
	{
		if( m_count != NULL )
		{
			if( *m_count <= 1 )
			{
				for(size_t i = 0; i < m_frames.size(); ++i)
					SDL_FreeSurface(m_frames[i]);
				m_frames.clear();
				delete m_count;
				m_count = NULL;
			}
			else if( *m_count > 0 )
				--(*m_count);
		}

		m_time = 0;
		m_act = 0;
		m_paused = true;
		m_lastTime = 0;
		m_timeStay = 0;
	}

	void Anim::play()
	{
		m_paused = false;
		m_lastTime = SDL_GetTicks();
	}

	void Anim::pause()
	{
		m_paused = true;
	}

	void Anim::toggle()
	{
		if( m_paused )
			play();
		else
			pause();
	}

	SDL_Surface* Anim::update()
	{
		if( m_paused )
			return m_frames.at(m_act);

		forward( SDL_GetTicks() - m_lastTime );
		m_lastTime = SDL_GetTicks();

		return m_frames.at(m_act);
	}

	SDL_Surface* Anim::getActSurf()
	{
		return m_frames.at(m_act);
	}

	void Anim::forward(Uint32 time)
	{
		while( time >= m_timeStay )
		{
			++m_act;
			if( m_act >= m_frames.size() )
				m_act=0;

			time -= m_timeStay;
			m_timeStay = m_time;
		}
		m_timeStay -= time;
	}

	void Anim::backward(Uint32 time)
	{
		Uint32 timeSpent = m_time - m_timeStay;

		while( time >= timeSpent )
		{
			if( m_act == 0 )
				m_act = m_frames.size() - 1;
			else
				--m_act;

			time -= timeSpent;
			timeSpent = m_time;
		}
		m_timeStay = time;
	}

	void Anim::next()
	{
		++m_act;
		if( m_act >= m_frames.size() )
			m_act = 0;
		
		m_timeStay = m_time;
		m_lastTime = SDL_GetTicks();
	}

	void Anim::prev()
	{
		if( m_act == 0 )
			m_act = m_frames.size() - 1;
		else
			--m_act;

		m_timeStay = m_time;
		m_lastTime = SDL_GetTicks();
	}

	bool Anim::isLoaded() const
	{
		return !m_frames.empty();
	}

};//namespace graphics
