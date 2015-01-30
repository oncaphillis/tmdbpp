#ifndef TMDBPP_UTIL_H
#define TMDBPP_UTIL_H

#include <set>
#include <memory>
#include <list>
#include <map>

#ifdef _WIN32
#else
#include <curlpp/cURLpp.hpp>
#endif

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

namespace tmdbpp {

    /** @short Interface between JSON data structures and TMDBPP data
        objects.

        Uses boost::property_tree data structures to pull information
        from the JSON tree and manages allocation/deallocation of ptree
        structures.
    */

    class JSonMapper {
    public:
        typedef boost::property_tree::ptree ptree_t;
        typedef std::string timestamp_t;
        typedef ptree_t::const_iterator const_iterator;

        JSonMapper() : _up_ptree() {
        }

        JSonMapper(const boost::property_tree::ptree & pt)  : _up_ptree(new ptree_t(pt)) {
        }

        JSonMapper(const JSonMapper & jsm) :
            _up_ptree(jsm._up_ptree.get() == NULL ? NULL : new ptree_t(*jsm._up_ptree)) {
        }

        JSonMapper(JSonMapper && jsm)  {
            swap(*this,jsm);
        }

        JSonMapper(std::istream & json) : _up_ptree(new ptree_t()) {
            parse(json);
        }

        JSonMapper(const std::string & s) : _up_ptree(new boost::property_tree::ptree()) {
            std::stringstream ss(s);
            parse(ss);
        }

        ~JSonMapper() {
        }

        size_t size() const {
            return ptree().size();
        }

        const_iterator begin() const {
            return ptree().begin();
        }

        const_iterator end() const {
            return ptree().end();
        }

        std::string toString() const {
            std::stringstream ss;
            print(ss);
            return ss.str();
        }

        void print(std::ostream & os) const {
            print(ptree(),os);
        }

        operator const void * () const {
            return _up_ptree.get() == NULL ? NULL : this;
        }

        JSonMapper & operator=(JSonMapper jsm) {
            swap(*this,jsm);
            return *this;
        }

        const ptree_t & ptree() const {
            if(_up_ptree.get()==NULL)
                throw std::runtime_error("illegal access to invalid ptree");
            return *_up_ptree;
        }

        static
        void print(const  boost::property_tree::ptree & pt,std::ostream & os,int n=0)  {
            if(pt.empty()) {
                os << std::string(n,' ') << "<" << pt.data() << ">";
            } else {
                for(auto nd : pt) {
                    os << std::string(n,' ') << "'" << nd.first << "':" << "[";
                    print(nd.second,os,n+1);
                    os << "]" << std::endl;
                }
            }
        }

    private:

        friend void swap(JSonMapper & first, JSonMapper & second) {
             using std::swap;
             first._up_ptree.swap(second._up_ptree);
        }

        void parse(std::istream & is) {
            if(_up_ptree.get()==NULL)
                throw std::runtime_error("illegal ptree access");

            boost::property_tree::ptree &pt = *_up_ptree;
            boost::property_tree::read_json(is,pt);

            std::set<std::string> se;
            for(auto n : pt) {
                if(n.first.empty()) {
                    throw std::runtime_error("illegal json with empty top level name");
                }
                if(se.find(n.first)!=se.end()) {
                    throw std::runtime_error("illegal json with duplicate top level name");
                }

                se.insert(n.first);;
            }
        }


        // If we're the owner of this ptree we hold the pointer here
        std::unique_ptr<boost::property_tree::ptree> _up_ptree;
    };

    inline
    std::ostream & operator << (std::ostream & os,const JSonMapper & jsm)  {
        return os << jsm.toString();
    }

    inline
    std::ostream & operator<<(std::ostream & os,const JSonMapper::ptree_t & pt) {
        JSonMapper::print(pt,os);
        return os;
    }

    /** @short Class reptesentation of TMDB error records normaly associated with
        a 405 error.
    */

    class ErrorStatus : public JSonMapper {
    private:
        typedef JSonMapper super;
    public:
        using super::ptree_t;
        ErrorStatus() : super () {
        }

        ErrorStatus(const ptree_t &pt) : super(pt) {
        }

        ErrorStatus(const std::string & s) : super(s) {
        }

        ErrorStatus(const ErrorStatus & e) : super(e) {
        }

        std::string status_message() const {
            return (*this) ? ptree().get<std::string>("status_message","") : "";
        }

        int status_code() const {
            return (*this) ? ptree().get<int>("status_code",0) : 0;
        }
    private:
    };


    inline
    std::ostream & operator<<(std::ostream & os,const ErrorStatus & es) {
        return os << " #" << es.status_code() << " '" << es.status_message() << "'";
    }

    /** @short Base class for everything in the TMDBPP API that holds an in ID and
        provides an id() call.
    */

    class IdHolder : public JSonMapper {

    private:
        typedef JSonMapper super;

    public:
        using super::ptree_t;

        IdHolder() : super() {
        }

        IdHolder(const ptree_t & p) : super(p) {
        }

        IdHolder(std::istream & is ) : super(is) {
        }

        /** @short Return the "id" entry in the associated JSON tree as an
            int.
        */

        const int  id() const {
            return ptree().get<int>("id");
        }
    private:
        friend
        void swap(IdHolder &i0, IdHolder & i1) {
            using std::swap;
            swap(static_cast<super &>(i0), static_cast<super &>(i1));
        }
         int _id;
    };

    class NameHolder : public JSonMapper {
    private:
        typedef JSonMapper super;
    public:
        
        NameHolder() : super() {
        }

        NameHolder(const boost::property_tree::ptree &p) : super(p) {
        }

        NameHolder(std::istream & is ) : super(is) {
        }

        std::string name() const {
            return ptree().get<std::string>("name","");
        }
    };

    class NameIdHolder : public JSonMapper {
    private:
        typedef JSonMapper super;
    public:
        NameIdHolder() : super() {
        }
        
        NameIdHolder(const boost::property_tree::ptree & p) : super(p) {

        }

        NameIdHolder(std::istream & is) : super(is)  {

        }

        std::string name() const {
            return ptree().get<std::string>("name","");
        }
        const int  id() const {
            return ptree().get<int>("id");
        }
    };

    class Country : public NameHolder {
    private:
        typedef NameHolder super;
    public:
        Country() : super() {
        }
        Country(const boost::property_tree::ptree & p) : super(p) {
        }        
        Country(std::istream & is ) : super(is) {
        }
        std::string iso_3166_1() const {
            return  ptree().get<std::string>("iso_3166_1","");
        }
    private:
    };

    class Language : public NameHolder {
    private:
        typedef NameHolder super;
    public:
        Language() : super() {
        }
        Language(const boost::property_tree::ptree & p) : super(p) {
        }        
        Language(std::istream & is ) : super(is) {
        }

        std::string iso_639_1() const {
            return  ptree().get<std::string>("iso_639_1","");
        }

    };


    class Genre : public NameIdHolder {
    private:
        typedef NameIdHolder super;
    public:
        Genre() : super() {
        }

        Genre(const boost::property_tree::ptree & p) : super(p) {
        }
    };

    class Timezone : public JSonMapper {
    private:
        typedef JSonMapper super;
    public:
        Timezone() : super() {
        }
        Timezone(const boost::property_tree::ptree & p) : super(p) {
            parse();
        }

        Timezone(const Timezone & tz) : super(tz) {
            parse();
        }

        Timezone(Timezone && tz) {
            swap(*this,tz);
        }

        std::string id() const {
            return ptree().begin()->first;
        }

        Timezone & operator=(Timezone  tz) {
            swap(*this,tz);
            return *this;
        }

        const std::list<std::string> & synonyms() const {
            return _list;
        }
    private:

        void parse() {
            _list.clear();
            for(auto n : ptree().begin()->second)
                _list.push_back(n.second.data());
        }

        friend
        void swap(Timezone & ta,Timezone & tb) {
            using std::swap;
            ta._list.swap(tb._list);
            swap(static_cast<super &>(ta), static_cast<super &>(tb));
        }

        std::list<std::string> _list;
    };
    inline
    std::ostream & operator<<(std::ostream & os,const Timezone & tz) {
        os << tz.id() << " (";
        int i = 0;
        for( auto s : tz.synonyms() ){
            os << (i++ == 0 ? "" : ";") << s;
        }
        return os;
    }

    class Timezones {
    private:
        typedef std::map<std::string,Timezone> pmap_t;
        typedef std::map<std::string,std::string> smap_t; 
    public:
        typedef pmap_t::const_iterator const_iterator;
        Timezones() {
        }
        void insert(const Timezone & tz) {
            pmap_t::iterator i=_pmap.find(tz.id());
            if(i!=_pmap.end()) {
                for(auto j = i->second.synonyms().begin();
                    j!=i->second.synonyms().end();j++) {
                    _smap.erase(*j);
                }
                _pmap.erase(i);
            }

            _pmap[tz.id()]=tz;

            for(auto t : tz.synonyms()) {
                _smap[t]=tz.id();
            }
        }

        const_iterator begin() const {
            return _pmap.begin();
        }

        const_iterator end() const {
            return _pmap.end();
        }

        size_t size() const {
            return _pmap.size();
        }

    private:
        friend
        void swap(Timezones & t0,Timezones & t1) {
            using std::swap;
            t0._pmap.swap(t1._pmap);
            t0._smap.swap(t1._smap);
        }

        pmap_t _pmap;
        smap_t _smap;
    };



    class Configuration  : public JSonMapper {
    private:
        typedef JSonMapper super;
    public:
        Configuration() : super() {
        }

        Configuration(const ptree_t & pc,
                      const ptree_t & pt) : super(pc) {

            for(auto n : pt) {
                Timezone tz(n.second);
                _timezones.insert(tz);
            }
        }

        Configuration(const Configuration & c) 
            : super(c),_timezones(c._timezones) {
        }
        Configuration(Configuration && c)  {
            swap(*this,c);
        }
        Configuration & operator=(Configuration c)  {
            swap(*this,c);
            return *this;
        }
        const Timezones & timezones() const {
            return _timezones;
        }

    private:
        friend
        void swap(Configuration & c0,Configuration &c1) {
            using std::swap;
            swap(c0._timezones,c1._timezones);
            swap(static_cast<super &>(c0), static_cast<super &>(c1));
        }

        Timezones _timezones;
    };


    class MediaSummary : public IdHolder {
    private:
        typedef IdHolder super;
    public:
        MediaSummary() : super() {
        }
        MediaSummary(const boost::property_tree::ptree & p) : super(p) {
        }
        MediaSummary(std::istream & is ) : super(is) {
        }

        std::string media_type() const {
            return ptree().get<std::string>("media_type","movie");
        }
        std::string title() const {
            return ptree().get<std::string>("title","");
        }
        std::string poster_path() const {
            return ptree().get<std::string>("poster_path","");
        }
        std::string release_date() const {
            return ptree().get<std::string>("release_date","");
        }
        std::string backdrop_path() const {
            return ptree().get<std::string>("backdrop_path","");
        }
    private:
        friend
        void swap(MediaSummary & m0, MediaSummary & m1) {
            using std::swap;
            swap(static_cast<super&>(m0), static_cast<super&>(m1));
        }

    };

    class Network : public IdHolder {
    private:
        typedef IdHolder super;
    public:
        Network(const boost::property_tree::ptree & p) : super(p) {
        }
    private:

    };

    class SeasonSummary : public IdHolder {
    private:
        typedef IdHolder super;
    public:
        SeasonSummary() : super() {
        }
        SeasonSummary(const boost::property_tree::ptree & p) : super(p) {
        }        
        SeasonSummary(std::istream & is ) : super(is) {
        }
    private:
    };
}

#endif // TMDBPP_UTIL_H
