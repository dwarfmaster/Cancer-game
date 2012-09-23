#include "core/exception.hpp"
#include "core/config.hpp"
#include "i18n.hpp"
#include <guichan.hpp>
#include <exception>

int main(int argc, char *argv[])
{
	try{
		// On charge la configuration
		core::cfg = new core::Config(argc, argv);

		// TODO

		delete core::cfg;
	}
	catch(const core::Exception& e)
	{
		std::cerr << _i("A game error appened : ") << e.what() << std::endl;
		return 1;
	}
	catch(const gcn::Exception& e)
	{
		std::cerr << _i("A guichan error appened : \"") << e.getMessage() 
			// Est suivit d'un nom de fonction
			<< _i("\" in ") << e.getFunction() << std::endl;
		return 1;
	}
	catch(const std::exception& e)
	{
		std::cerr << _i("An internal error appened : ") << e.what() << std::endl;
		return 1;
	}
	catch(...)
	{
		std::cerr << _i("An unknown error appened.") << std::endl;
		return 1;
	}

	return 0;
}

