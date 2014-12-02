#include <string>
#include <sstream>
#include <iostream>
#include <sstream>
#include <set>

#include <tmdbpp/api.h>
#include <tmdbpp/search.h>

int main(int, char **)
{
    try
    {

        tmdbpp::Movies m = tmdbpp::Api::instance().search().movies("indiana jones","de",2);
        std::cerr << m.page() << "/" << m.total_pages() << "@" << m.total_results() << std::endl;
        std::cerr << "'" << m.list().begin()->title() << "'" << std::endl;

        std::cerr << " --- " << std::endl
                  << tmdbpp::Api::instance().get().movie(m.list().begin()->id()) << std::endl
                  << " --- " << std::endl;


        return 0;
    }
    catch( curlpp::RuntimeError &e )
    {
        std::cout << e.what() << std::endl;
    }
    catch( curlpp::LogicError &e )
    {
        std::cout << e.what() << std::endl;
    }
    catch( std::exception & e) {
        std::cout << e.what() << std::endl;
    }

  return 0;
}
