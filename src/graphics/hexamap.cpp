
#include "hexamap.hpp"

#include <SDL/SDL.h>
#include <SDLP_tools.hpp>
#include "tile.hpp"

#include <tinyxml.h>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <cmath>
#define PI 3.14159265

#include "../core/exception.hpp"
#include <sstream>
#include "../i18n.hpp"

namespace fs = boost::filesystem;

namespace graphics
{
	HexaMap::HexaMap(unsigned int tileSize)
		: m_size(0,0), m_ori(0,0), m_tileSize(tileSize), m_s(m_tileSize / std::cos(30.0*PI/180.0) / 2)
	{
	}

	HexaMap::HexaMap(const fs::path& src, const load_tile_f& loader, unsigned int tileSize)
		: m_size(0,0), m_ori(0,0), m_tileSize(tileSize), m_s(m_tileSize / std::cos(30.0*PI/180.0) / 2)
	{
		load(src, loader);
	}

	HexaMap::~HexaMap()
	{
		clear();
	}

	void HexaMap::load(const boost::filesystem::path& src, const load_tile_f& loader)
	{
		clear();

		// On vérifie le loader
		if( loader.empty() )
			throw core::Exception( _i("There is no loader set for the tilemap") );

		// On ouvre le document
		TiXmlDocument file(src.string());
		if( !file.LoadFile() )
		{
			std::ostringstream oss;
			oss << _i("Error while opening the file \"") << src.string() << _i("\" : \"") << file.ErrorDesc() << _i("\"");
			throw core::Exception( oss.str() );
		}

		// On récupère le noeud map
		TiXmlElement *elem = file.FirstChildElement("map");
		if( elem == NULL )
		{
			std::ostringstream oss;
			oss << _i("The file \"") << src.string() << _i("\" has no map node");
			throw core::Exception( oss.str() );
		}

		// On récupère la taille
		if( elem->Attribute("size") == NULL )
		{
			std::ostringstream oss;
			oss << _i("The file \"") << src.string() << _i("\" has an invalid map node : no size attribute");
			throw core::Exception( oss.str() );
		}
		size_t size = sdl::atoi( elem->Attribute("size") );

		// On récupère les colonnes
		std::vector<row_t> rows;
		elem = elem->FirstChildElement("row");
		while( elem != NULL )
		{
			rows.push_back( parseRow(elem, loader, size) );
			elem = elem->NextSiblingElement("row");
		}

		// On le stocke dans la classe
		m_size.x = size;
		m_size.y = rows.size();
		m_map.resize( boost::extents[m_size.x][m_size.y] );

		for(size_t i = 0; i < m_size.x; ++i)
		{
			for(size_t j = 0; j < m_size.y; ++j)
				m_map[i][j] = rows[j][i];
		}
	}

	void HexaMap::save(const fs::path& dest) const
	{
		fs::ofstream ofs(dest);
		if( !ofs )
		{
			std::ostringstream oss;
			oss << _i("Error while opening the file : \"") << dest.string() << _i("\"");
			throw core::Exception( oss.str() );
		}

		ofs << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" << std::endl;
		ofs << "<map size=\"" << m_size.x << "\">" << std::endl;

		for(size_t i = 0; i < m_size.x; ++i)
		{
			ofs << "\t<row>" << std::endl;
			for(size_t j = 0; j < m_size.y; ++j)
			{
				ofs << "<cell type=\"";
				if( m_map[i][j] == NULL )
					ofs << "nil\" />";
				else
					ofs << "content\">" << m_map[i][j]->save() << "</cell>";
				ofs << std::endl;
			}
			ofs << "\t</row>" << std::endl;
		}

		ofs << "</map>" << std::endl;
	}

	void HexaMap::clear()
	{
		for(size_t i = 0; i < m_size.x; ++i)
		{
			for(size_t j = 0; j < m_size.y; ++j)
			{
				if( m_map[i][j] != NULL )
					delete m_map[i][j];
			}
		}

		m_map.resize( boost::extents[0][0] );
		m_size.set(0,0);
		m_ori.set(0,0);
	}

	void HexaMap::scroll(const sdl::Vector2f& dec)
	{
		sdl::AABB totsize = totalSize();
		sdl::Pointsi ori = m_ori;
		ori += dec;

		if( ori.x < 0 )
			m_ori.x = 0;
		else if( ori.x + m_pictSize->w > totsize->w )
			m_ori.x = totsize->w - m_pictSize->w;
		else
			m_ori.x = ori.x;

		if( ori.y < 0 )
			m_ori.y = 0;
		else if( ori.y + m_pictSize->h > totsize->h )
			m_ori.y = totsize->h - m_pictSize->h;
		else
			m_ori.y = ori.y;
	}

	Tile* HexaMap::getTileAt(unsigned int x, unsigned int y)
	{
		if( x < m_size.x
				|| y < m_size.y )
			return m_map[x][y];
		else
			return NULL;
	}

	const Tile* HexaMap::getTileAt(unsigned int x, unsigned int y) const
	{
		if( x < m_size.x
				|| y < m_size.y )
			return m_map[x][y];
		else
			return NULL;
	}

	Tile* HexaMap::getTileAtPos(const sdl::Pointui& pos)
	{
		sdl::Pointui rpos = pos + sdl::Vector2f(m_ori);
		long int x = rpos.x / m_s / 1.5;
		long int y = (rpos.y - (x%2) * m_tileSize / 2) / m_tileSize;

		if( y < 0
				|| x < 0 )
			return NULL;
		else
			return getTileAt( (unsigned int)x, (unsigned int)y );
	}

	const Tile* HexaMap::getTileAtPos(const sdl::Pointui& pos) const
	{
		sdl::Pointui rpos = pos + sdl::Vector2f(m_ori);
		long int x = rpos.x / m_s / 1.5;
		long int y = (rpos.y - (x%2) * m_tileSize / 2) / m_tileSize;

		if( y < 0
				|| x < 0 )
			return NULL;
		else
			return getTileAt( (unsigned int)x, (unsigned int)y );
	}

	SDL_Surface* HexaMap::get(const sdl::AABB& size) const
	{
		// TODO
	}

	void HexaMap::setSize(const sdl::AABB& size)
	{
		m_pictSize = size;
	}

	SDL_Surface* HexaMap::get() const
	{
		return get(m_pictSize);
	}

	HexaMap::operator SDL_Surface*() const
	{
		return get(m_pictSize);
	}

	HexaMap::row_t HexaMap::parseRow(const TiXmlElement* row, const load_tile_f& loader, size_t size)
	{
		row_t rowlist;
		const TiXmlElement* elem = row->FirstChildElement("cell");
		while( elem != NULL )
		{
			if( elem->Attribute("type") == NULL )
				throw std::string("Invalid cell."); // Le texte ne va pas être affiché.

			std::string type = elem->Attribute("type");
			if( type == "nil" )
				rowlist.push_back(NULL);
			else if( elem->GetText() != NULL )
			{
				Tile* loaded = loader( elem->GetText() );
				if( loaded == NULL )
					throw std::string("Invalid content of cell."); // Le texte ne va pas être affiché.
				rowlist.push_back(loaded);
			}
			else
				throw std::string("Invalid cell."); // Le texte ne va pas être affiché.

			elem = elem->NextSiblingElement("cell");
		}

		if( rowlist.size() != size )
			throw std::string("Invalid size."); // Le texte ne va pas être affiché.

		return rowlist;
	}
			
	sdl::AABB HexaMap::totalSize() const
	{
		size_t w = (m_size.x - 1) * m_s * 1.5;
		w += m_tileSize;
		size_t h = m_size.y * m_tileSize + m_tileSize / 2;

		return sdl::makeRect(0, 0, w, h);
	}

};//namespace graphics

