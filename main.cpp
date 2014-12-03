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
        tmdbpp::Api &api(tmdbpp::Api::instance());

        tmdbpp::Movies m = api.search().movie("indiana jones","de",1);
        std::cerr << m.page() << "/" << m.total_pages() << "@" << m.total_results() << std::endl;

        for(const auto n : m.list()) {
            std::cerr << "'" << n.title() << "'" << std::endl;
            tmdbpp::Movie m = api.get().movie(n.id());
            for(const auto g : m.genres()) {
                std::cerr << " >> '" << g.name() << "'" << std::endl;
            }
        }

        tmdbpp::Companies c = api.search().company("fox");
        std::cerr << c.page() << "/" << c.total_pages() << "@" << c.total_results() << std::endl;

        for(const auto n : c.list()) {
            std::cerr << "'" << n.name() << "'" << std::endl;
        }
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
