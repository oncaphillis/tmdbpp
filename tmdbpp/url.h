#ifndef TMDBPP_URL_H
#define TMDBPP_URL_H

#include <list>
#include <sstream>
#include <string>

namespace tmdbpp {

    /** @short Represents one arg added to the end of an URL like "name=value" or simply "name"
     */

    class UrlArg {
    public:
        UrlArg() : _v_set(false) {
        }

        UrlArg(const std::string & name) : _v_set(false) {
            _n = name;
            _o = name;
        }

        template<class T>
        UrlArg(const std::string & name,const T & value) : _v_set(true) {
            std::stringstream ss;
            ss << value;
    #ifdef _WIN32
            // WinHttpClient takes care of argument escaping
            _o = name + "=" + ss.str();
    #else
            _o = name + "=" + curlpp::escape(ss.str());
    #endif
        }

        UrlArg operator()(const std::string & name) {
            return UrlArg(name);
        }

        UrlArg operator()(const std::string & name,const std::string & val) {
            return UrlArg(name,val);
        }

        operator std::string() const {
            return _o;
        }

        const std::string & toString() const {
            return _o;
        }

    private:
        std::string _n;
        std::string _v;
        std::string _o;
        bool _v_set;
    };

    inline
    std::string operator+(const std::string &s,const UrlArg &a) {
        return s+(std::string)a;
    }

    inline
    std::ostream & operator<<(std::ostream &os,const UrlArg &a) {
        return os << a.toString();
    }

    class Url {
    private:
    public:
        Url(const std::string & base) : _base(base) {
        }

        template<class CONTAINER >
        Url(const std::string & base,const CONTAINER & c) : _base(base) {
            std::copy(c.begin(),c.end(),std::back_inserter(_args));
        }

        Url(const Url &u) : _base(u._base),_args(u._args),_path(u._path) {
        }

        Url(Url && u) {
            swap(*this,u);
        }

        Url & operator=(Url u) {
            swap(*this,u);
            return *this;
        }

        Url & add(const std::string & path) {
            _path.push_back(path);
            return *this;
        }

        Url & add(const UrlArg & a) {
            _args.push_back(a);
            return *this;
        }

        Url & operator+=(const UrlArg & a) {
            return add(a);
        }

        std::string toString() const {

            std::stringstream ss;

            ss << _base;



            for(auto p : _path) {
                ss << ( (!ss.str().empty()) && ss.str().back()!='/' && !p.empty() && p.front()!='/' ? "/" : "") << p;
            }
            int i=0;
            for(auto a : _args) {
                ss << (i++==0 ? "?" : "&") << a;
            }

            return ss.str();
        }

        operator std::string () const {
            return toString();
        }
    private:
        friend
        void swap(Url &a, Url &b) {
            using std::swap;
            a._base.swap(b._base);
            a._path.swap(b._path);
            a._args.swap(b._args);
        }

        std::string _base;
        std::list<std::string> _path;
        std::list<UrlArg> _args;
    };
    inline
    std::ostream & operator<<(std::ostream & os,const Url & u) {
        return os << u.toString();
    }
}

#endif // URL_H
