
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

namespace graphics
{
	class DeleterContainer;
	class Button;
};
#include "graphics/label.hpp"
#include <guichan.hpp>

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

		std::string organToStr(const Organs& org);
		Organs strToOrgan(const std::string& s); // retourne ALLORGS en cas d'erreur
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
		Vessel* m_veselected;
		std::vector<Vessel> m_vessels;
		void loadVessels(const boost::filesystem::path& dir, double fact);
		Vessel loadVessel(const boost::filesystem::path& path, double fact);

		// Corp
		SDL_Surface* m_corpse;
		sdl::Pointui m_ori; // Position où blitter le corp
		void load(); // Charge toutes les images, ainsi que les sauvegardes et la GUI, lance une exception en cas d'erreur

		// Gui
		gcn::Container* m_top;
		graphics::DeleterContainer* m_gvessels;
		graphics::Label* m_gvessrc;
		graphics::Label* m_gvessrct;
		graphics::Label* m_gvesdest;
		graphics::Label* m_gvesdestt;
		graphics::Button* m_gvesthrow;
		graphics::DeleterContainer* m_gorgans;
		graphics::Label* m_gorgtitle;
		graphics::Label* m_gorgname;
		graphics::Label* m_gorgst;
		graphics::Label* m_gorgstate;
		graphics::Button* m_gorgenter;
		void initGui();

		// Fonctions
		void clic(const sdl::Pointsi& pos);
		void selectOrgan(const Organs& org);
		void selectVessel(Vessel* s);
		void enter(); // On entre dans un organe
		void vesthrow(); // On envoie des agents mutagènes à travers un vaisseau sanguin
		void draw();
		void setGui();

		SDL_Surface* ecran;
};//class Body

#endif//DEF_BODY

