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

        std::list<tmdbpp::Genre> gl = api.get().movie().genres();

        int i=0;

        for(auto g : gl) {
            std::cerr << "@" << ++i << " " << g.id() << " " << g.name() << std::endl;
        }
        ::exit(1);

        int p=1;
        while(true) {
            tmdbpp::Persons persons = api.search().person("kevin bacon",p++);
            if(persons.empty())
                break;

            std::cerr << persons.page() << "/" << persons.total_pages() << "@" << persons.total_results() << std::endl;
            for(const auto & p : persons.list()) {
                std::cerr << p.name() << std::endl;
                auto pp = api.get().person(p.id());
                std::cerr << pp.name() << " -- " << pp.also_known_as() << " -- " << pp.birthday() << std::endl;
                auto cr = api.search().person().movie_credits(p.id());

                std::cerr << "<<" << std::endl
                          << cr.as_cast().size() << "::" << cr.as_crew().size()
                          << std::endl
                          << ">>" << std::endl;
                for(auto c : cr.as_cast()) {
                    std::cerr << c.title() << "::" << c.character() << "@" << c.media_id() << std::endl;
                    auto ccc = api.search().movie().credits(c.media_id());
                    for(auto cc : ccc.as_cast() ) {
                        std::cerr << " >> " << cc.name() << " " << cc.character() << std::endl;
                    }
                }
             }
        }
        return 0;
    }
    catch( std::exception &e )
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
