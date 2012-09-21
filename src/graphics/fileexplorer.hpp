
#ifndef DEF_GRAPHICS_FILEEXPLORER
#define DEF_GRAPHICS_FILEEXPLORER

#include <guichan.hpp>
#include <vector>
#include <string>
#include <boost/filesystem/path.hpp>
#include <boost/function.hpp>
#include <SDLP_position.hpp>

namespace graphics
{
	class TextField;
	class List;
	class Button;
	class CheckBox;
	class Theme;

	// Cette classe ne gère pas bien les chemins relatifs : utilisez toujours des chemins absolus
	class FileExplorer : public gcn::Container
	{
		friend Theme;

		public:
			typedef boost::function<void (boost::filesystem::path)> cb_t;
			typedef std::vector<std::string> reglist_t;

			FileExplorer(bool dir = false);
			FileExplorer(const boost::filesystem::path& path, bool dir = false);
			FileExplorer(const reglist_t& regs, bool dir = false);
			~FileExplorer();

			reglist_t getRegs() const;
			void setRegs(const reglist_t& regs);
			void addReg(const std::string& reg);

			void setPath(const boost::filesystem::path& path);
			boost::filesystem::path getActPath() const;

			void setFoundCB(const cb_t& cb);
			void clearCB();
			bool isCB() const;

			void setDimension(const gcn::Rectangle& dimension);
			void draw(gcn::Graphics* g);

		private:
			reglist_t m_regs; // Expressions regulières cherchées
			boost::filesystem::path m_path;
			cb_t m_found;
			const bool m_dir; // Cherche un fichier ou un dossier
			bool m_all; // Show all

			TextField* m_field;
			List* m_dirs;
			List* m_files;
			
			Button* m_valid;
			Button* m_enter;
			Button* m_up;
			CheckBox* m_call;
			gcn::ScrollArea* m_scrldir;
			gcn::ScrollArea* m_scrlfile;

			void select();
			void enter();
			void toggleAll(bool all);
			void setup();
			void update();
			void upPos();
			void changePath(std::string npath);
			void up();

			struct Dir
			{
				std::vector<std::string> dirs;
				std::vector<std::string> files;
			};
			Dir scanDir(const boost::filesystem::path& path);
	};//class FileExporer
};//namespace graphics

#endif//DEF_GRAPHICS_FILEEXPLORER
