
#ifndef DEF_GRAPHICS_INTERNAL_SELECTLISTENER
#define DEF_GRAPHICS_INTERNAL_SELECTLISTENER

#include <guichan.hpp>
#include <boost/function.hpp>

namespace graphics
{
	namespace internal
	{
		class ListModel;

		class SelectListener : public gcn::SelectionListener
		{
			public:
				SelectListener(bool dd = false);
				SelectListener(const boost::function<void (std::string)>& callback, bool dd = false);

				void set(const boost::function<void (std::string)>& callback);
				void clear();
				bool isSet();

				void setModel(ListModel* mod);
				void clearModel();
				bool isModel() const;

				virtual void valueChanged(const gcn::SelectionEvent& ev);

			private:
				boost::function<void (std::string)> m_callback;
				ListModel* m_model;
				const bool m_dd; // DropDown
		};//class SelectListener
	};//namespace internal
};//namespace graphics

#endif//DEF_GRAPHICS_INTERNAL_SELECTLISTENER

