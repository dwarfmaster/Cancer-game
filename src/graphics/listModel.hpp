
#ifndef DEF_GRAPHICS_INTERNAL_LISTMODEL
#define DEF_GRAPHICS_INTERNAL_LISTMODEL

#include <guichan.hpp>
#include <vector>
#include <string>

namespace graphics
{
	namespace internal
	{
		class ListModel : public gcn::ListModel
		{
			public:
				std::vector<std::string> values;

				ListModel();
				ListModel(const std::vector<std::string>& nvalues);

				virtual std::string getElementAt(int i);
				virtual int getNumberOfElements();
		};//class ListModel
	};//namespace internal
};//namespace graphics

#endif//DEF_GRAPHICS_INTERNAL_LISTMODEL

