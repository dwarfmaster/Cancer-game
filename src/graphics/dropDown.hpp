
#ifndef DEF_GRAPHICS_DROPDOWN
#define DEF_GRAPHICS_DROPDOWN

#include <guichan.hpp>
#include <vector>
#include <string>
#include <boost/function.hpp>

namespace graphics
{
	namespace internal
	{
		class ListModel;
		class SelectListener;
	};

	class DropDown : public gcn::DropDown
	{
		public:
			typedef boost::function<void (std::string)> cb_t;

			DropDown();
			DropDown(const std::vector<std::string>& values);
			~DropDown();

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
	};//class DropDown
};//namespace graphics

#endif//DEF_GRAPHICS_DROPDOWN

