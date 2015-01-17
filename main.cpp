#include <string>
#include <sstream>
#include <iostream>
#include <sstream>
#include <set>
#include <algorithm>
#include <vector>

#include <tmdbpp/api.h>
#include <tmdbpp/search.h>

#include <nifty/IndexedMap.h>

class TmdbMap {

private:
    typedef std::map<int,std::string>     person_map_t;
    typedef std::map<int,std::set<int> >  movie_to_person_t;
    typedef std::map<int,std::set<int> >  person_to_movie_t;

    typedef nifty::BasicIndexedMap<int,char,nifty::CaseLessLess<char> >    movie_map_t;
    typedef nifty::IndexedMap::key_t      movie_key_t;
public:

    typedef std::set<movie_key_t>         movie_set_t;

    const std::string & getMovie(const movie_key_t & k) {
        return _movies[k];
    }

    movie_set_t searchMovies(const std::string & s) {
        time_t t0;
        time_t t1;

        t0 = time(&t0);

        std::set<movie_key_t> se =  _movies.find(s,movie_map_t::AnchorStart);

        return se;
    }

    bool load(std::istream & is) {
        person_map_t      persons;
        movie_map_t       movies;
        movie_to_person_t movie_to_person;
        person_to_movie_t person_to_movie;

        std::set<int> orphans;

        int i=0;
        std::string line;

        while(std::getline(is,line) ) {
            if((i % 1000) == 0)
                std::cerr << "\x1b[K" << i << "\r" << std::flush;
            i++;

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

            movie_key_t mkey;

            {
                std::stringstream ss(lv[1]);

                if( ! (ss >> mkey)) {
                    std::cerr << "expected number found '" << lv[1] << "' in '" << line << "'" << std::endl;
                    return false;
                }
            }

            if(lv[0] == "m") {
                if(movies[mkey] != "") {
                    std::cerr << "movie #" << mkey << "already defined as '" << movies[mkey] << "'" << std::endl
                              << "found '" << lv[2] << "'" << std::endl;
                    return false;
                }
                //movies[n] = lv[2];

                movies.add(mkey,lv[2]);

            } else if(lv[0] == "p") {
                if(persons.find(mkey)!=persons.end()) {
                    std::cerr << "person #" << mkey << "already defined as '" << persons[mkey] << "'" << std::endl
                              << "found '" << lv[2] << "'" << std::endl;
                    return false;
                }
                orphans.erase(mkey);
                persons[mkey] = lv[2];
            } else if(lv[0] == "r") {

                int o;
                std::stringstream ss(lv[2]);

                if( ! (ss>>o)) {
                    std::cerr << "expected number found '" << lv[2] << "' in '" << line << "'" << std::endl;
                    return false;
                }

                movie_to_person_t::const_iterator itm = movie_to_person.find(mkey);

                if( movies[mkey].empty() ) {
                    std::cerr << "relation to unknown movie #" << mkey << std::endl;
                    return false;
                }

                if(persons.find(o)==persons.end())
                    orphans.insert(o);

                if((itm=movie_to_person.find(mkey))!=movie_to_person.end() && itm->second.find(o)!=itm->second.end()) {
                    continue;
                }

                person_to_movie_t::const_iterator itp = person_to_movie.find(mkey);

                if( (itp=person_to_movie.find(o))!=person_to_movie.end() && itp->second.find(mkey)!=itp->second.end()) {
                    std::cerr << "relation m=>p (#" << o << "=>#" << mkey<< ") already defined" << std::endl;
                    continue;
                }

                movie_to_person[mkey].insert(o);
                person_to_movie[o].insert(mkey);

            } else {
              std::cerr << "expected m|r|p found '" << lv[0] << "' in '" << line << "'" << std::endl;
              return false;
            }
        }

        std::cerr << std::endl << i << std::endl;

        std::cerr << "persons:" << persons.size() << " movies:" << movies.size() << ":"
                  << " p=>m:" << person_to_movie.size() << " m=>p:" << movie_to_person.size() << std::endl;

        persons.swap(_persons);
        movies.swap(_movies);
        person_to_movie.swap(_person_to_movie);
        movie_to_person.swap(_movie_to_person);
        orphans.swap(_orphans);

        return true;
    }

    void crawlPerson(std::set<int> persons, std::ostream & os=std::cerr) {

        while(!persons.empty()) {

            int id = *persons.begin();
            persons.erase(id);
            _orphans.erase(id);

            if(_persons.find(id) != _persons.end()) {
                _pch++;
                continue;
            }

            time_t t1 = time(&t1);

            std::cerr << "\x1b[K"
                      << " ids:" << std::setw(5) <<  persons.size()
                      << " mrq:" << std::setw(5) <<  _mrq
                      << " mch:" << std::setw(5) <<  _mch
                      << " prq:" << std::setw(5) <<  _prq
                      << " pch:" << std::setw(5) <<  _pch
                      << std::fixed << std::setprecision(2)
                      << " t/s:" << (float) (_mrq+_prq) / (float)(t1-_t0)
                      << "  p:"   << std::setw(5) << _persons.size()
                      << "  m:"   << std::setw(5) << _movies.size()
                      << "  o:"   << std::setw(5) << _orphans.size()
                      << " mp:"   << std::setw(5) << _movie_to_person.size()
                      << " pm:"   << std::setw(5) << _person_to_movie.size()
                      << "\r" << std::flush;

            tmdbpp::Api &api(tmdbpp::Api::instance());
            tmdbpp::Person pnode=api.get().person(id);
            _prq++;

            if(!pnode) {
                std::cerr << " failed to fetch person #" << id << " " << api.status() << "'" << std::endl;
                continue;
            }

            os << "p:" << pnode.id() << ":" << pnode.name() << std::endl;

            _persons[id] = pnode.name();

            tmdbpp::MovieCredits mc = api.search().person().movie_credits(id);
            _prq++;

            for(auto m : mc.as_cast()) {

                movie_to_person_t::const_iterator it;

                if((it=_movie_to_person.find(m.media_id())) == _movie_to_person.end()) {

                    if( ! _movies[ m.media_id()].empty() ) {
                        std::stringstream ss;
                        std::cerr << "movie #" << m.media_id() << " already defined as '" << _movies[m.media_id()] << "' "
                                  << " but no m=> p mapping available => refetching"
                                  << std::endl;
                    } else {
                        os << "m:" << m.media_id() << ":" << m.title() << std::endl;
                        //_movies[m.media_id()] = m.title();
                        _movies.add(m.media_id(),m.title());
                    }

                    auto mc = api.search().movie().credits(m.media_id());
                    _mrq++;

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
                    _mch++;
                }

                for(auto p : it->second) {
                    persons.insert(p);
                }
            }
        }
        std::cerr << "-DONE-" << std::endl;
    }

    void crawlMovie(std::set<int> mids, std::ostream & os=std::cerr) {

        while(!mids.empty()) {

            int id = *mids.begin();
            mids.erase(id);

            if( ! _movies[id].empty() ) {
                _mch++;
                if(_movie_to_person.find(id)!=_movie_to_person.end()) {
                    continue;
                }
                std::cerr << " ?? movie #" << id << " '" << _movies[id] << "' already known; but no actors" << std::endl;
            }

            tmdbpp::Api &api(tmdbpp::Api::instance());
            auto mnode = api.get().movie(id);
            _mrq++;

            if(!mnode) {
                std::cerr << " failed to fetch movie #" << id << " " << api.status() << "'" << std::endl;
                continue;
            }

            os << "m:" << mnode.id() << ":" << mnode.title() << std::endl;

            _movies.add(id,mnode.title());

            tmdbpp::PersonalCredits pc = api.search().movie().credits(id);
            _mrq++;
            std::set<int> se;

            _movie_to_person.insert(movie_to_person_t::value_type(id,std::set<int>()));

            for(auto p : pc.as_cast()) {
                os << "r:" << mnode.id() << ":" << p.id() << std::endl;
                _movie_to_person[id].insert(p.id());
                _person_to_movie[p.id()].insert(id);
                se.insert(p.id());
            }
            crawlPerson(se,os);
        }
    }


    void resetCounter() {
        _mrq = 0;
        _prq = 0;
        _mch = 0;
        _pch = 0;
        _t0 = time(&_t0);
    }

    void crawlPerson(int person, std::ostream & os = std::cerr) {
        std::set<int> ids;
        ids.insert(person);
        crawlPerson(ids,os);
    }

    void crawlMovie(int movie,std::ostream & os = std::cerr) {
        std::set<int> ids;
        ids.insert(movie);
        crawlMovie(ids,os);
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

    /** @short Singleton method for TmdbMap
     *
     */

    static TmdbMap & instance() {
        static TmdbMap m;
        return m;
    }

private:
    TmdbMap() {
    }

    int    _mch = 0;
    int    _pch = 0;
    int    _mrq = 0;
    int    _prq = 0;
    time_t _t0  = 0;

    std::set<int>     _orphans;
    person_map_t      _persons;
    movie_map_t       _movies;
    movie_to_person_t _movie_to_person;
    person_to_movie_t _person_to_movie;
};

static const std::string Usage = "Usage: tmdbcli command arg";

bool search(const std::vector<std::string> & args) {
    if(args.size()!=1) {
        std::cerr << Usage << std::endl;
        return false;
    }

    TmdbMap::instance().searchMovies(args[0]);

    return true;
}

int main(int argc, char **argv)
{
    try
    {
        tmdbpp::Api &api(tmdbpp::Api::instance());
        TmdbMap & m(TmdbMap::instance());

        std::ifstream fs("/shared/tmdb.dmp");
        m.load(fs);
        std::ofstream fo("bacon_o");

#if 1
        std::cerr << std::endl << " - - - " << std::endl;
        std::string l;


        while(true) {
            std::cout << " ?> "  << std::flush;

            if(!std::getline(std::cin,l)) {
                break;
            }

            std::set<int> s0 = m.searchMovies(l);

            std::set<int> s1;
            std::map<int,std::string> mo_map;

            int n=0;

            while(true) {
                auto r = api.search().movie(l,"",n++);

                if(r.empty())
                   break;

                std::for_each(r.begin(),r.end(),
                              [& s1,& mo_map](const tmdbpp::Movies::value_type & v) {
                                s1.insert(v.id());
                                mo_map[v.id()] = v.title();
                            });
            }

            std::cerr << " * * L O C A L " << std::endl;
            for(auto x : s0) {
                std::cerr << " " << (s1.find(x)!=s1.end() ? "+" : " ") << x << " '" << m.getMovie(x) << "'" << std::endl;
            }
            std::cerr << " * * E X T E R N " << std::endl;

            for(auto x : s1) {
                std::cerr << " " << (s0.find(x)!=s0.end() ? "+" : " ") << x << " '" << mo_map[x] << "'" << std::endl;
            }

            std::set<int> so;
            std::set_difference(s1.begin(),s1.end(),s0.begin(),s0.end(),std::inserter(so,so.end()));

            m.crawlMovie(so,fo);
        }

#else
        std::cerr << " orphans:" << m.ophans().size() << std::endl;

        /*
         if(!m.ophans().empty()) {
            while(!m.ophans().empty()) {
                m.scan(*(m.ophans().begin()),fo);
            }
        } else {
            std::set<int> s;
            for(std::map<int,std::string>::const_iterator it=m.persons().begin();it!=m.persons().end();it++) {
                s.insert(it->first);
            }
            m.reset();
            while(!s.empty()) {
                std::cerr << std::endl <<  "+" << s.size() << std::endl;
                m.scan(*(s.begin()),fo);
                s.erase(*(s.begin()));
            }
        }
        */

        // for(auto p : l) {
        std::cerr << " @:" << m.movies().size() << std::endl;
        m.crawlMovie(991,fo);
        // }
#endif

        return 0;
    }
    catch( std::exception &e )
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
