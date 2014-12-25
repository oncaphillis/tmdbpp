#include <string>
#include <sstream>
#include <iostream>
#include <sstream>
#include <set>
#include <algorithm>
#include <vector>

#include <tmdbpp/api.h>
#include <tmdbpp/search.h>

class TmdbMap {

private:

    typedef std::map<int,std::string>     person_map_t;
    typedef std::map<int,std::string>     movie_map_t;
    typedef std::map<int,std::set<int> >  movie_to_person_t;
    typedef std::map<int,std::set<int> >  person_to_movie_t;

public:
    bool load(std::istream & is) {
        person_map_t persons;
        movie_map_t  movies;
        movie_to_person_t movie_to_person;
        person_to_movie_t person_to_movie;

        std::set<int> orphans;

        int i=0;
        std::string line;

        while(std::getline(is,line)) {
            std::cerr << "\x1b[K" << i++ << "\r" << std::flush;

            std::stringstream ss(line);
            std::string cell;
            std::vector<std::string> lv;

            int j=0;
            while(std::getline(ss,cell,':')) {
                lv.push_back(cell);
                if(++j==2)
                    break;
            }

            if((lv.size()!=2) || (!std::getline(ss,cell))) {
                std::cerr << "expexted 3 cells found " << lv.size() << " in '" << line << "' |" << cell << "|" << std::endl;
                return false;
            }

            lv.push_back(cell);

            int n;

            {
                std::stringstream ss(lv[1]);

                if( ! (ss>>n)) {
                    std::cerr << "expected number found '" << lv[1] << "' in '" << line << "'" << std::endl;
                    return false;
                }
            }

            if(lv[0] == "m") {
                if(movies.find(n)!=movies.end()) {
                    std::cerr << "movie #" << n << "already defined as '" << movies[n] << "'" << std::endl
                              << "found '" << lv[2] << "'" << std::endl;
                    return false;
                }
                movies[n] = lv[2];
            } else if(lv[0] == "p") {
                if(persons.find(n)!=persons.end()) {
                    std::cerr << "person #" << n << "already defined as '" << persons[n] << "'" << std::endl
                              << "found '" << lv[2] << "'" << std::endl;
                    return false;
                }
                orphans.erase(n);
                persons[n] = lv[2];
            } else if(lv[0] == "r") {

                int o;
                std::stringstream ss(lv[2]);

                if( ! (ss>>o)) {
                    std::cerr << "expected number found '" << lv[2] << "' in '" << line << "'" << std::endl;
                    return false;
                }

                movie_to_person_t::const_iterator itm = movie_to_person.find(n);

                if(movies.find(n)==movies.end()) {
                    std::cerr << "relation to unknown movie #" << n << std::endl;
                    return false;
                }

                if(persons.find(o)==persons.end())
                    orphans.insert(o);

                if((itm=movie_to_person.find(n))!=movie_to_person.end() && itm->second.find(o)!=itm->second.end()) {
                    std::cerr << "relation movie #" << n << " '" << movies[n] << "' => #" << o
                              << " already defined " << std::endl;
                    continue;
                }

                person_to_movie_t::const_iterator itp = person_to_movie.find(n);

                if( (itp=person_to_movie.find(o))!=person_to_movie.end() && itp->second.find(n)!=itp->second.end()) {
                    std::cerr << "relation m=>p (#" << o << "=>#" << n<< ") already defined" << std::endl;
                    continue;
                }

                movie_to_person[n].insert(o);
                person_to_movie[o].insert(n);

            } else {
              std::cerr << "expected m|r|p found '" << lv[0] << "' in '" << line << "'" << std::endl;
              return false;
            }
        }

        std::cerr << persons.size() << ":" << movies.size() << ":"
                  << person_to_movie.size() << ":" << movie_to_person.size() << std::endl;

        persons.swap(_persons);
        movies.swap(_movies);
        person_to_movie.swap(_person_to_movie);
        movie_to_person.swap(_movie_to_person);
        orphans.swap(_orphans);
        return true;
    }

    const person_map_t & persons() const {
        return _persons;
    }

    const movie_map_t & movies() const {
        return _movies;
    }

    const person_to_movie_t & personToMovie() const {
        return _person_to_movie;
    }

    const movie_to_person_t & movieToPerson() const {
        return _movie_to_person;
    }

    const std::set<int> & ophans() const {
        return _orphans;
    }

private:
    std::set<int> _orphans;
    person_map_t _persons;
    movie_map_t  _movies;
    movie_to_person_t _movie_to_person;
    person_to_movie_t _person_to_movie;
};

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

        std::ifstream fs("bacon");
        TmdbMap m;
        m.load(fs);

        std::cerr << " --> " << m.ophans().size() << std::endl;
//        if(l.size()==1)
//            ScanTmdb(l.front().id(),movies,persons,0,0,fs);



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
