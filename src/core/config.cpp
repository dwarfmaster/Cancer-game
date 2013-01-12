
#include "config.hpp"
#include "../i18n.hpp"

#include <SDLP_tools.hpp>
#include <string>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>
#include <boost/any.hpp>

namespace opt = boost::program_options;
typedef boost::filesystem::path path_t;

// Validate functions for boost.program_options
void validate(boost::any& v, const std::vector<std::string>& values, SDL_Rect*, int)
{
	static boost::regex reg("(\\d+)[x*](\\d+)");

	using namespace boost::program_options;
	validators::check_first_occurrence(v);
	const std::string& s = validators::get_single_string(values);

	boost::smatch match;
	if( boost::regex_match(s, match, reg) )
	{
		SDL_Rect rect;
		rect.x = rect.y = 0;
		rect.w = boost::lexical_cast<Uint16>(match[1]);
		rect.h = boost::lexical_cast<Uint16>(match[2]);
		v = boost::any(rect);
	}
	else
		throw validation_error(validation_error::invalid_option_value);
}

namespace core
{
	Config* cfg;

	const char* default_path_sounds = "audio.d";
	const char* default_path_config = "conf.cfg";
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

	Config::~Config()
	{}

	void Config::load(int argc, char *argv[])
	{
		boost::program_options::variables_map vm;

		// Lecture de la ligne de commande
		opt::parsed_options parsed = opt::command_line_parser(argc, argv)
			.options(m_opts)
			.run();
		opt::store(parsed, vm);
		opt::notify(vm);

		// Lecture du fichier de conf
		if( vm.count("config") )
			m_config = vm["config"].as<path_t>();
		else
			m_config = getPath(default_path_config);

		boost::filesystem::ifstream file(m_config);
		if(file)
		{
			opt::store( opt::parse_config_file(file, m_opts), vm );
			opt::notify(vm);
		}

		processOpts(vm);
	}

	sdl::AABB Config::size() const
	{
		return m_size;
	}

	bool Config::fullscreen() const
	{
		return m_fullscreen;
	}

	path_t Config::sounds() const
	{
		return m_dsounds;
	}

	path_t Config::gtheme() const
	{
		return m_gtheme;
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
			("size,s", opt::value<SDL_Rect>()->composing(), _i("Set the size of the window, like '800x600'."))
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

	void Config::processOpts(const boost::program_options::variables_map& vm)
	{
		if( vm.count("help") )
			std::cout << m_desc << std::endl;

		if( vm.count("volume") )
			m_sounds = m_music = vm["volume"].as<unsigned short int>();
		else
		{
			if( vm.count("audio.sounds") )
				m_sounds = vm["audio.sounds"].as<unsigned short int>();
			else
				m_sounds = 127;
			if( m_sounds >= 255 )
				m_sounds = 255;

			if( vm.count("audio.music") )
				m_music = vm["audio.music"].as<unsigned short int>();
			else
				m_music = 127;
			if( m_music >= 255 )
				m_music = 255;
		}

		if( vm.count("fullscreen") )
			m_fullscreen = true;
		else
			m_fullscreen = false;

		if( vm.count("size") )
			m_size = vm["size"].as<SDL_Rect>();
		else if( fullscreen() )
			m_size = maxSize();
		else
			m_size = sdl::makeRect(0, 0, 800, 600);

		if( vm.count("sounds") )
			m_dsounds = vm["sounds"].as<path_t>();
		else
			m_dsounds =  getPath(default_path_sounds);

		if( vm.count("gtheme") )
			m_gtheme = vm["gtheme"].as<path_t>();
		else
			m_gtheme =  getPath(default_path_gtheme);
	}

	unsigned short int Config::volume(bool sounds) const
	{
		if( sounds )
			return m_sounds;
		else
			return m_music;
	}

};//namespace core

