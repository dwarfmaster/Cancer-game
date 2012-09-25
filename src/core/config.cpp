
#include "config.hpp"
#include "../i18n.hpp"

#include <SDLP_tools.hpp>
#include <string>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>

namespace opt = boost::program_options;
typedef boost::filesystem::path path_t;

namespace core
{
	Config* cfg;

	const char* default_path_sounds = "audio.d";
	const char* default_path_config = "conf";
	const char* default_path_gtheme = "default.gtheme";
	const char* rc_dir = RCDIR;
	const char* final_name = FNAME;

	Config::Config()
		: m_desc(_i("Allowed options"))
	{
		setOpts();
	}

	Config::Config(int argc, char *argv[])
		: m_desc(_i("Allowed options"))
	{
		setOpts();
		load(argc, argv);
	}

	void Config::load(int argc, char *argv[])
	{
		// Lecture de la ligne de commande
		opt::parsed_options parsed = opt::command_line_parser(argc, argv)
			.options(m_opts)
			.run();
		opt::store(parsed, m_vm);
		opt::notify(m_vm);

		// Lecture du fichier de conf
		if( m_vm.count("config") )
			m_config = m_vm["config"].as<path_t>();
		else
			m_config = getPath(default_path_config);

		boost::filesystem::ifstream file(m_config);
		if(file)
		{
			opt::store( opt::parse_config_file(file, m_opts), m_vm );
			opt::notify(m_vm);
		}

		processOpts();
	}

	sdl::AABB Config::size() const
	{
		if( m_vm.count("size") )
			return parseSize( m_vm["size"].as<std::string>() );
		else if( fullscreen() )
			return maxSize();
		else
			return sdl::makeRect(0, 0, 800, 600);
	}

	bool Config::fullscreen() const
	{
		return m_vm.count("fullscreen");
	}

	path_t Config::sounds() const
	{
		if( m_vm.count("sounds") )
			return m_vm["sounds"].as<path_t>();
		else
			return getPath(default_path_sounds);
	}

	path_t Config::gtheme() const
	{
		if( m_vm.count("gtheme") )
			return m_vm["gtheme"].as<path_t>();
		else
			return getPath(default_path_gtheme);
	}

	void Config::setOpts()
	{
		std::string configDesc( _i("Precise a configuration file (default = ") );
		configDesc += getPath(default_path_config).string();
		configDesc += _i(").");

		// Options
		opt::options_description general( _i("General") );
		general.add_options()
			("config,c", opt::value<path_t>(), configDesc.c_str() )
			("help,h", _i("Display an help message."))
			;

		// Options
		opt::options_description sound( _i("Audio") );
		sound.add_options()
			("sounds,S", opt::value<path_t>(), _i("Set the path to the a sounds theme (a directory)."))
			("volume,v", opt::value<unsigned short int>(), _i("Set the volume of the application."))
			;

		// Options
		opt::options_description graphics( _i("Graphics") );
		graphics.add_options()
			("size,s", opt::value<std::string>()->composing(), _i("Set the size of the window, like '800x600'."))
			("fullscreen,f", _i("Launch the program in fullscreen mode."))
			("gtheme,g", opt::value<path_t>(), _i("Set the path to the graphics theme."))
			;

		opt::options_description hidden;
		hidden.add_options()
			("audio.music", opt::value<unsigned short int>())
			("audio.sounds", opt::value<unsigned short int>())
			;

		m_desc.add(general).add(sound).add(graphics);
		m_opts.add(general).add(sound).add(graphics).add(hidden);

		const char* home = std::getenv("HOME");
		if( home != NULL )
		{
			std::string add(".");
			add += final_name;
			m_home = path_t(home) / add;
		}
	}

	sdl::AABB Config::maxSize() const
	{
		if( !SDL_WasInit(SDL_INIT_VIDEO) )
			return sdl::makeRect(0, 0, 1600, 900);

		SDL_Rect** modes = SDL_ListModes(NULL, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);

		if( modes == (SDL_Rect**)0 )
			return sdl::makeRect(0, 0, 0, 0);
		else if( modes == (SDL_Rect**)-1 )
			return sdl::makeRect(0, 0, 1600, 900);
		else
			return *modes[0];
	}

	sdl::AABB Config::parseSize(const std::string& size) const
	{
		std::string tmp(""); // sert à stocker la string d'un nombre
		bool cont = true, first = true;
		SDL_Rect rect;
		rect.x = rect.y = 0;
		rect.w = 800;
		rect.h = 600;
		for(size_t k = 0; cont; ++k)
		{
			if(k >= size.size())
			{
				unsigned int nb = 0;
				nb = sdl::atoi(tmp);
				if(first)
					rect.w = nb;
				else
					rect.h = nb;
				cont = false;
			}
			else if(size[k] >= '0' 
					&& size[k] <= '9')
				tmp += size[k];
			else if(size[k] == '*' 
					|| size[k] == 'x')
			{
				unsigned int nb = 0;
				nb = sdl::atoi(tmp);
				if(first)
				{
					rect.w = nb;
					tmp = "";
					first = false;
				}
				else
				{
					rect.h = nb;
					cont = false;
				}
			}
			else // Peu probable
			{
				unsigned int nb = 0;
				std::istringstream(tmp) >> nb;
				if(first)
					rect.w = nb;
				else
					rect.h = nb;
				cont=false;
			}
		}

		if( rect.w < 800 )
			rect.w = 800;
		if( rect.h < 600 )
			rect.h = 600;

		return rect;
	}

	path_t Config::getPath(path_t end) const
	{
		if( !m_home.empty()
				&& boost::filesystem::exists(m_home) 
				&& boost::filesystem::is_directory(m_home) )
		{
			path_t path = m_home / end;
			if( boost::filesystem::exists(path) )
				return path;
			else
				return rc_dir / end;
		}
		else
			return rc_dir / end;
	}

	void Config::processOpts()
	{
		if( m_vm.count("help") )
			std::cout << m_desc << std::endl;

		if( m_vm.count("volume") )
			m_sounds = m_music = m_vm["volume"].as<unsigned short int>();
		else
		{
			if( m_vm.count("audio.sounds") )
				m_sounds = m_vm["audio.sounds"].as<unsigned short int>();
			else
				m_sounds = 127;
			if( m_sounds >= 255 )
				m_sounds = 255;

			if( m_vm.count("audio.music") )
				m_music = m_vm["audio.music"].as<unsigned short int>();
			else
				m_music = 127;
			if( m_music >= 255 )
				m_music = 255;
		}
	}

	unsigned short int Config::volume(bool sounds) const
	{
		if( sounds )
			return m_sounds;
		else
			return m_music;
	}

};//namespace core

