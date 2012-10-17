
#ifndef DEF_BODY
#define DEF_BODY

#include <SDLP_position.hpp>
#include <boost/array.hpp>
#include <vector>

struct SDL_Surface;
namespace boost
{
	namespace filesystem
	{
		class path;
	};
};

class Body
{
	public:
		Body();
		~Body();

		void run();

	private:
		// Organes
		enum Organs{brain, heart, lung, liver, kidney, digestive, ALLORGS};
		struct Organ
		{
			SDL_Surface* healthy; // Image de l'organe sain
			SDL_Surface* sick; // Image de l'organe atteint
			SDL_Surface* control; // Image de l'organe contrôlé

			enum OrganState{HEATHLY, SICK, CONTROL};
			OrganState state; // TODO récupérer
		};
		boost::array<Organ, ALLORGS> m_orgs;
		Organs m_selected; // Si m_selected == ALLORGS : aucun sélectionné
		SDL_Surface* loadPicture(const boost::filesystem::path& path, double fact);
		Organ loadOrgan(const boost::filesystem::path& dir, double fact); // fact est le redimmensionnement, lance une exception en cas d'erreur

		// Vaisseau sanguins
		struct Vessel
		{
			SDL_Surface* picture;
			Organs src;
			Organs dest;
			unsigned int size;
		};
		std::vector<Vessel> m_vessels;
		void loadVessels(const boost::filesystem::path& dir, double fact);
		Vessel loadVessel(const boost::filesystem::path& path, double fact);
		Organs strToOrgan(const std::string& s); // retourne ALLORGS en cas d'erreur

		// Corp
		SDL_Surface* m_corpse;
		sdl::Pointui m_ori; // Position où blitter le corp
		void load(); // Charge toutes les images, ainsi que les sauvegardes et la GUI, lance une exception en cas d'erreur

		void clic(const sdl::Pointsi& pos);
		void draw();
		void setGui();

		SDL_Surface* ecran;
};//class Body

#endif//DEF_BODY

