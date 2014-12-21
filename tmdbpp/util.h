#ifndef TMDBPP_UTIL_H
#define TMDBPP_UTIL_H

#include <set>
#include <memory>

#ifdef _WIN32
#else
#include <curlpp/cURLpp.hpp>
#endif

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace tmdbpp {

    class JSonMapper {
    public:
        JSonMapper() : _up_ptree() {
        }

        JSonMapper(const boost::property_tree::ptree & pt)  : _up_ptree(new boost::property_tree::ptree(pt)) {
        }

        JSonMapper(const JSonMapper & jsm) : JSonMapper(*jsm._up_ptree) {
        }

        JSonMapper(JSonMapper && jsm)  {
            swap(*this,jsm);
        }

        JSonMapper(std::istream & json) : _up_ptree(new boost::property_tree::ptree()) {
            parse(json);
        }

        ~JSonMapper() {
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

        const boost::property_tree::ptree & ptree() const {
            if(_up_ptree.get()==NULL)
                throw std::runtime_error("illegal access to invalid ptree");
            return *_up_ptree;
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

        void print(const  boost::property_tree::ptree & pt,std::ostream & os,int n=0) const {
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

        // If we're the owner of this ptree we hold the pointer here
        std::unique_ptr<boost::property_tree::ptree> _up_ptree;
    };

    inline
    std::ostream & operator << (std::ostream & os,const JSonMapper & jsm)  {
        return os << jsm.toString();
    }

    class IdHolder : public JSonMapper {

    private:
        typedef JSonMapper super;

    public:
        IdHolder() : super() {
        }

        IdHolder(const boost::property_tree::ptree & p) : super(p) {
        }
        IdHolder(std::istream & is ) : super(is) {
        }

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

    class CompanySummary : public NameIdHolder {
    private:
        typedef NameIdHolder super;
    public:
        CompanySummary() : super() {
        }
        CompanySummary(const boost::property_tree::ptree & p) : super(p) {
        }
        CompanySummary(std::istream & is) : super(is) {
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

    class Configuration : public JSonMapper {
    private:
        typedef JSonMapper super;
    public:
        
        Configuration() : super() {
        }
        
        Configuration(const boost::property_tree::ptree & p) : super(p) {
        }
        
        Configuration(std::istream & is) : super(is) {
        }

    private:
    };

    class Arg {
    public:
        template<class T>
        Arg(const std::string & name,const T & value)  {
            std::stringstream ss;
            ss << value;
#ifdef _WIN32
            // WinHttpClient takes care of argument escaping
            _o = name + "=" + ss.str();
#else
            _o = name + "=" + curlpp::escape(ss.str());
#endif
        }

        operator std::string() const {
            return _o;
        }
    private:
        std::string _o;
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



    inline
    std::string operator+(const std::string &s,const Arg &a) {
        return s+(std::string)a;
    }

}

#endif // TMDBPP_UTIL_H
