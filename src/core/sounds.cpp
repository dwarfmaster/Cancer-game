
#include "sounds.hpp"

#include "../i18n.hpp"
#include "exception.hpp"
#include <SDL/SDL_mixer.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <sstream>
#include <SDLP_tools.hpp>

namespace fs = boost::filesystem;

namespace core
{
	Sounds::Sounds(const fs::path& sounds)
		: m_paused(true), m_mute(false), m_volume(255/2)
	{
		if( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0 )
		{
			std::ostringstream oss;
			oss << _i("Error while loading audio library : \"") << Mix_GetError() << "\"";
			throw core::Exception( oss.str() );
		}

		for(size_t i = 0; i < m_sounds.size(); ++i)
			m_sounds[i].sound = NULL;
		for(size_t i = 0; i < m_musics.size(); ++i)
			m_musics[i] = NULL;

		load(sounds);
	}

	Sounds::~Sounds()
	{
		clear(); // Libère les ressources
		Mix_CloseAudio();
	}

	void Sounds::playMusic(const Music& mus)
	{
		Mix_PlayMusic( m_musics[mus], -1 );
		m_paused = false;
	}

	void Sounds::playSound(const Sound& sound)
	{
		Mix_PlayChannel( m_sounds[sound].channel, m_sounds[sound].sound, 0 );
	}

	void Sounds::pause()
	{
		Mix_PauseMusic();
		Mix_Pause(-1);
		m_paused = true;
	}

	void Sounds::unpause()
	{
		Mix_ResumeMusic();
		Mix_Resume(-1);
		m_paused = false;
	}

	void Sounds::togglepause()
	{
		if( m_paused )
			unpause();
		else
			pause();
	}

	void Sounds::setVolume(unsigned char nvol)
	{
		m_volume = nvol;

		if( !m_mute )
		{
			int vol = mixVol(m_volume);
			Mix_VolumeMusic(vol);

			for(size_t i = 0; i < m_nchannels; ++i)
				Mix_Volume(i, vol);
		}
	}

	void Sounds::mute()
	{
		Mix_VolumeMusic(0);
		for(size_t i = 0; i < m_nchannels; ++i)
			Mix_Volume(i, 0);

		m_mute = true;
	}

	void Sounds::unmute()
	{
		int vol = mixVol(m_volume);
		Mix_VolumeMusic(vol);
		for(size_t i = 0; i < m_nchannels; ++i)
			Mix_Volume(i, vol);

		m_mute = false;
	}

	void Sounds::toggleMute()
	{
		if( m_mute )
			unmute();
		else
			mute();
	}

	bool Sounds::isPaused() const
	{
		return m_paused;
	}

	bool Sounds::isMute() const
	{
		return m_mute;
	}

	unsigned char Sounds::volume() const
	{
		return m_volume;
	}

	struct tmpSound // Structure utilisée uniquement par la fonction ci-dessous
	{
		fs::path path;
		size_t channel;
	};

	boost::array<tmpSound, Sounds::ALL_SOUNDS> parse(const fs::path& file) // Utilisée uniquement dans la fonction Sounds::load
	{
		fs::ifstream ifs(file);
		if( !ifs )
		{
			std::ostringstream oss;
			oss << _i("Error while opening the file ") << file.string();
			throw core::Exception( oss.str() );
		}

		std::string line;
		boost::array<tmpSound, Sounds::ALL_SOUNDS> sounds;

		size_t nbline = 0;
		while( std::getline(ifs, line) )
		{
			++nbline;

			if( nbline > 7 )
				break;

			std::istringstream iss(line);
			std::string mot;

			if( !getline(iss, mot, ' ') )
			{
				std::ostringstream oss;
				oss << _i("Error while reading the file \"") << file.string() << _i("\" at line ") << nbline;
				throw core::Exception( oss.str() );
			}

			Sounds::Sound id;
			if( mot == "ok" )
				id = Sounds::ok;
			else if( mot == "cancel" )
				id = Sounds::cancel;
			else if( mot == "select" )
				id = Sounds::select;
			else if( mot == "wincel" )
				id = Sounds::wincel;
			else if( mot == "winorg" )
				id = Sounds::winorg;
			else if( mot == "alert" )
				id = Sounds::alert;
			else if( mot == "hibern" )
				id = Sounds::hibern;
			else
			{
				std::ostringstream oss;
				oss << _i("Error while reading the file \"") << file.string() << _i("\" at line ") << nbline << _i(" : unknow sound name \"") << mot << _i("\"");
				throw core::Exception( oss.str() );
			}

			if( !getline(iss, mot, ' ') )
			{
				std::ostringstream oss;
				oss << _i("Error while reading the file \"") << file.string() << _i("\" at line ") << nbline;
				throw core::Exception( oss.str() );
			}

			sounds[id].path = mot;

			if( !getline(iss, mot, ' ') )
			{
				std::ostringstream oss;
				oss << _i("Error while reading the file \"") << file.string() << _i("\" at line ") << nbline;
				throw core::Exception( oss.str() );
			}

			sounds[id].channel = sdl::atoi(mot);
		}

		if( nbline < 7 )
		{
			std::ostringstream oss;
			oss << _i("Error in the file \"") << file.string() << _i("\" : not enought sounds set");
			throw core::Exception( oss.str() );
		}

		return sounds;
	}

	void Sounds::load(const fs::path& sounds)
	{
		fs::path musicdir = sounds / "music";
		if( !fs::exists(musicdir)
				|| !fs::is_directory(musicdir) )
			throw core::Exception( _i("The sounds theme has no music dir") );

		// Chargement des musiques
		fs::path path = musicdir / "menu.ogg";
		m_musics[menu] = Mix_LoadMUS( path.string().c_str() );
		if( !m_musics[menu] )
		{
			std::ostringstream oss;
			oss << _i("Error while loading the music ") << path.string();
			throw core::Exception( oss.str() );
		}

		path = musicdir / "game.ogg";
		m_musics[game] = Mix_LoadMUS( path.string().c_str() );
		if( !m_musics[game] )
		{
			std::ostringstream oss;
			oss << _i("Error while loading the music ") << path.string();
			clear();
			throw core::Exception( oss.str() );
		}

		// Chargement des sons
		fs::path sounddir = sounds / "sounds";
		path = sounddir / "list";
		boost::array<tmpSound, ALL_SOUNDS> soundlist = parse(path);

		m_nchannels = 0;
		for(size_t i = 0; i < soundlist.size(); ++i)
		{
			path = sounddir / soundlist[i].path;
			m_sounds[i].channel = soundlist[i].channel;

			if( m_sounds[i].channel > m_nchannels )
				m_nchannels = m_sounds[i].channel;

			m_sounds[i].sound = Mix_LoadWAV( path.string().c_str() );
			if( !m_sounds[i].sound )
			{
				std::ostringstream oss;
				oss << _i("Error while loading the sound ") << path.string();
				clear();
				throw core::Exception( oss.str() );
			}
		}

		// Allocation des channeau
		++m_nchannels;
		Mix_AllocateChannels(m_nchannels);
	}

	int Sounds::mixVol(unsigned char vol) const
	{
		float fact = float(MIX_MAX_VOLUME) / 255.0f;
		float nvalue = float(vol) * fact;

		return static_cast<int>(nvalue);
	}

	void Sounds::clear()
	{
		for(size_t i = 0; i < m_musics.size(); ++i)
		{
			if( m_musics[i] )
				Mix_FreeMusic( m_musics[i] );
		}

		for(size_t i = 0; i < m_sounds.size(); ++i)
		{
			if( m_sounds[i].sound )
				Mix_FreeChunk( m_sounds[i].sound );
		}
	}

};//namespace core

