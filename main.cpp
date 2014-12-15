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


        tmdbpp::TvSeriesCollection c;

        int p=1;
        int n=1;
        do {
            c= api.search().tv().airing_today("de",p++);

            for(const auto a : c.list()) {
                std::cerr << "#" << n++ << " [" <<  a.name() << "] " << a.popularity() << " "
                          << a.vote_average() << "/" << a.vote_count() << std::endl
                          << std::endl;

                std::cerr << api.get().tv(a.id(),"de").overview() << std::endl
                          << " ----- "
                          << std::endl;


            }
        } while(!c.list().empty());

        return 0;
#if 0
        for(const auto n : m.list()) {
            std::cerr << "'" << n.title() << "'" << std::endl;
            tmdbpp::Movie m = api.get().movie(n.id());
            for(const auto g : m.genres()) {
                std::cerr << " >> '" << g.name() << "'" << std::endl;
            }
        }

        tmdbpp::Companies c = api.search().company("fox");
        std::cerr << "+" << c.page() << "/" << c.total_pages() << "@" << c.total_results() << std::endl;

        for(const auto n : c.list()) {
            std::cerr << "**'" << n.name() << "'**" << std::endl;
            std::cerr << " -- " << std::endl << api.get().company(n.id()).homepage() << std::endl
                      << " -- " << std::endl;
        }

        {
            tmdbpp::Collections c = api.search().collection("fox");
            std::cerr << c.page() << "/" << c.total_pages() << "@" << c.total_results() << std::endl;

            for(const auto n : c.list()) {
                std::cerr << "'" << n.name() << "'" << std::endl;
                auto c = api.get().collection(n.id());
                std::cerr << c << std::endl;
                for(auto p : c.parts()) {
                    std::cerr << p.title() << "|" << p.release_date() << std::endl;
                }
            }
        }

#endif

        {
            int p=1;
            tmdbpp::Keywords kws;
            do {
                kws=api.search().keyword("horror",p);
                for(const auto n : kws.list()) {
                    tmdbpp::Keyword k = api.get().keyword(n.id());
                    std::cerr << " / / " << k.id() << "::" << k.name() << std::endl << " / / "
                              << std::endl;
                }
            } while(p++<kws.total_pages());
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
