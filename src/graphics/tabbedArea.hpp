
#ifndef DEF_GRAPHICS_TABBEDAREA
#define DEF_GRAPHICS_TABBEDAREA

#include <guichan.hpp>
#include <map>
#include <string>
#include <vector>
#include <boost/function.hpp>

namespace graphics
{
	class TabbedArea : public gcn::TabbedArea
	{
		public:
			typedef boost::function<void (std::string, Widget*)> cb_t;

			TabbedArea();
			virtual ~TabbedArea();

			bool isSelected(const std::string& name) const;
			size_t exists(const std::string& name) const; // Renvoie nbTabs() si n'existe pas, sinon renvoie l'index
			size_t nbTabs() const;
			std::vector<std::string> listOfTabs() const;

			bool select(const std::string& name);
			bool add(const std::string& name, Widget* widget);
			bool remove(const std::string& name);
			bool rename(const std::string& idx, const std::string& nname);

			void setCB(const cb_t& cb);
			void clearCB();
			bool isCB() const;

			virtual void keyPressed(gcn::KeyEvent& ev);
			virtual void mousePressed(gcn::MouseEvent& ev);

		private:
			cb_t m_callback;
			size_t m_last;

			void update();
	};//class TabbedArea
};//namespace graphics

#endif//DEF_GRAPHICS_TABBEDAREA
