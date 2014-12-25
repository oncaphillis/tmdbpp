#include <string>
#include <sstream>
#include <iostream>
#include <sstream>
#include <set>

#include <tmdbpp/api.h>
#include <tmdbpp/search.h>


void ScanTmdb(int id,std::map<int,std::shared_ptr<tmdbpp::MediaCredits> > & movies,std::set<int> & persons,
              time_t t0=0, int n=0,std::ostream & os=std::cerr) {
    if(t0==0)
        t0 = time(&t0);

    if(persons.find(id)!=persons.end()) {
        return;
    }

    time_t t1 = time(&t1);

    std::cerr << "\x1b[K" << (float) n / (float)(t1-t0) << "\r" << std::flush;

    tmdbpp::Api &api(tmdbpp::Api::instance());
    tmdbpp::Person p=api.get().person(id);

    os << "p:" << p.id() << ":" << p.name() << std::endl;

    persons.insert(id);

    tmdbpp::PersonalCredits mc = api.search().person().movie_credits(id);
    n++;
    for(auto m : mc.as_cast()) {
        std::map<int,std::shared_ptr<tmdbpp::MediaCredits> >::const_iterator it;

        tmdbpp::MediaCredits *mcp;

        if((it=movies.find(m.media_id()))!=movies.end()) {
            mcp = it->second.get();
        } else {
            mcp = (movies[m.media_id()] =
            std::shared_ptr<tmdbpp::MediaCredits>(
                new tmdbpp::MediaCredits(api.search().movie().credits(m.media_id())))).get();
            n++;
            os << "m:" << m.media_id() << ":" << m.title() << std::endl;
            for( auto c : mcp->as_cast() ) {
                os << "r:" << m.media_id() << ":" << c.id() << std::endl;
            }
        }

        for(auto p : mcp->as_cast()) {
            ScanTmdb(p.id(),movies,persons,t0,n,os);
        }
    }
}

int main(int, char **)
{
    try
    {

        tmdbpp::Api &api(tmdbpp::Api::instance());

        int p=1;

        std::list<tmdbpp::PersonSummary> l;

        while(true) {
            tmdbpp::Persons persons = api.search().person("kevin bacon",p++);
            if(persons.empty())
                break;

            std::copy(persons.list().begin(),persons.list().end(),
                      std::back_inserter(l));
        }

        std::map<int,std::shared_ptr<tmdbpp::MediaCredits> > movies;
        std::set<int> persons;

        std::ofstream fs("bacon");

        if(l.size()==1)
            ScanTmdb(l.front().id(),movies,persons,0,0,fs);



 #if 0
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
#endif
        return 0;
    }
    catch( std::exception &e )
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
