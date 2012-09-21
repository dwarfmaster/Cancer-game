
#include "listModel.hpp"

namespace graphics
{
	namespace internal
	{
		ListModel::ListModel()
			: gcn::ListModel()
		{}

		ListModel::ListModel(const std::vector<std::string>& nvalues)
			: gcn::ListModel(), values(nvalues)
		{}


		std::string ListModel::getElementAt(int i)
		{
			if( i >= 0 
					&& i < getNumberOfElements() )
				return values[i];
			else
				return "";
		}

		int ListModel::getNumberOfElements()
		{
			return values.size();
		}


	};//namespace internal
};//graphics

