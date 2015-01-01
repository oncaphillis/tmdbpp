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

    void scan(std::list<int> ids, std::ostream & os=std::cerr) {

        time_t t0 = time(&t0);
        int n  = 0;
        int ch = 0;

        while(!ids.empty()) {

            int id = ids.front();
            ids.pop_front();

            if(_persons.find(id) != _persons.end()) {
                continue;
            }

            time_t t1 = time(&t1);

            std::cerr << "\x1b[K"
                      << " i:" << std::setw(5) <<  ids.size()
                      << " n:" << std::setw(5) <<  n
                      << " ch:" << std::setw(5) << ch
                      << std::fixed << std::setprecision(2)
                      << " t/s:" << (float) n / (float)(t1-t0)
                      << "  p:"   << std::setw(5) << _persons.size()
                      << "  m:"   << std::setw(5) << _movies.size()
                      << "  o:"   << std::setw(5) << _orphans.size()
                      << " mp:"   << std::setw(5) << _movie_to_person.size()
                      << " pm:"   << std::setw(5) << _person_to_movie.size()
                      << "\r" << std::flush;

            tmdbpp::Api &api(tmdbpp::Api::instance());
            tmdbpp::Person pnode=api.get().person(id);
            n++;

            os << "p:" << pnode.id() << ":" << pnode.name() << std::endl;
            _persons[id] = pnode.name();
            _orphans.erase(id);

            tmdbpp::MovieCredits mc = api.search().person().movie_credits(id);
            n++;

            for(auto m : mc.as_cast()) {

                movie_to_person_t::const_iterator it;

                if((it=_movie_to_person.find(m.media_id())) == _movie_to_person.end()) {

                    movie_map_t::const_iterator movie_it;

                    if((movie_it = _movies.find(m.media_id())) != _movies.end()) {
                        std::stringstream ss;
                        ss << "movie #" << m.media_id() << " already defined as '" << movie_it->second << "' "
                           << " but no m=> p mapping available "
                           << std::endl;
                        throw std::runtime_error(ss.str());
                    }

                    os << "m:" << m.media_id() << ":" << m.title() << std::endl;

                    _movies[m.media_id()] = m.title();

                    auto mc = api.search().movie().credits(m.media_id());
                    n++;

                    it = _movie_to_person.insert(movie_to_person_t::value_type(m.media_id(),std::set<int>())).first;

                    for( auto c : mc.as_cast() ) {
                        os << "r:" << m.media_id() << ":" << c.id() << std::endl;
                        _movie_to_person[m.media_id()].insert(c.id());
                        _person_to_movie[c.id()].insert(m.media_id());

                        if(_persons.find(c.id())==_persons.end()) {
                            _orphans.insert(c.id());
                        }
                    }
                } else {
                    ch++;
                }
#if 0
                if(it->second.size()>200) {
                    std::cerr << std::endl
                              << " !! #" << m.media_id() << " '" << _movies[m.media_id()] << "' "
                              << it->second.size() << " @ " << _movie_to_person[m.media_id()].size()
                              << std::endl;

                    ::exit(1);
                }
#endif
                for(auto p : it->second) {
                    ids.push_back(p);
                }
            }
        }
        std::cerr << "-DONE-" << std::endl;
    }

    void scan(int id, std::ostream & os=std::cerr) {
        int n  = 0;
        int ch = 0;
        std::list<int> ids;
        ids.push_back(id);
        scan(ids,os);
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
            std::copy(persons.begin(),persons.end(),std::back_inserter(l));
        }

        std::map<int,std::shared_ptr<tmdbpp::PersonalCredits> > movies;
        std::set<int> persons;

        std::ifstream fs("bacon");
        TmdbMap m;
        m.load(fs);

        std::cerr << std::endl << std::endl;

        std::ofstream fo("bacon_o");

        std::cerr << " orphans:" << m.ophans().size() << std::endl;

        while(!m.ophans().empty()) {
            m.scan(*(m.ophans().begin()),fo);
        }

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
