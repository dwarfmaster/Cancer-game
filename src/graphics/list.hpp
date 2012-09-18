
#ifndef DEF_GRAPHICS_LIST
#define DEF_GRAPHICS_LIST

#include <guichan.hpp>

#include "selectListener.hpp"
#include "listModel.hpp"
#include <boost/function.hpp>

namespace graphics
{
	// setListModel ne doit pas être utilisé directement
	class List : public gcn::ListBox
	{
		public:
			typedef boost::function<void (std::string)> cb_t;

			List();
			List(const std::vector<std::string>& values);
			~List();

			void setCB(const cb_t& callback);
			void clearCB();
			bool isCB() const;

			std::vector<std::string>* operator->();
			const std::vector<std::string>* operator->() const;
			std::vector<std::string>& operator*();
			const std::vector<std::string>& operator*() const;

		private:
			internal::ListModel* m_model;
			internal::SelectListener* m_listener;
	};//class List
};//namespace graphics

#endif//DEF_GRAPHICS_LIST
