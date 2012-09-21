
#include "icon.hpp"

#include <SDL/SDL.h>
#include <guichan/sdl.hpp>

namespace graphics
{
	Icon::Icon()
		: gcn::Icon()
	{}

	Icon::Icon(SDL_Surface* surf)
		: gcn::Icon()
	{
		setImage(surf);
	}

	Icon::Icon(const boost::filesystem::path& path)
		: gcn::Icon( path.string() )
	{}

	void Icon::setImage(SDL_Surface* surf)
	{
		gcn::Image* img = new gcn::SDLImage(surf, false);
		gcn::Icon::setImage(img);
		mInternalImage = true;
	}

};//namespace graphics

