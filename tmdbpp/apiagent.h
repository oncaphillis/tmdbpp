#ifndef APIAGENT_H
#define APIAGENT_H

#ifdef _WIN32
#include <tmdbpp/win_wget.h>
#else
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#endif

#include <tmdbpp/util.h>
#include <string>
#include <sstream>

namespace tmdbpp {

    class Api;

    // The base class for every class that likes to access the TMDBB
    // Api

    class ApiAgent {
    private:
    public:
        ApiAgent(Api & api) : _p_api(&api) {

        }

        std::string fetch(const std::string & url) {
#ifdef _WIN32
            return WGet::instance().get(url);
#else
            std::stringstream ss;
            curlpp::options::Url myUrl(url);
            curlpp::Easy myRequest;
            myRequest.setOpt(myUrl);
            myRequest.setOpt(curlpp::options::WriteStream(&ss));
            myRequest.perform();
            return ss.str();
#endif
        }

        template<class T>
        T & fetch(const std::string & url,T & t) {
            std::stringstream ss(fetch(url));
            t = T(ss);
            return t;
        }

        template<class T>
        std::list<T>  & fetch(const std::string & url,std::list<T> & l,const std::string & stree="") {
            std::string s = fetch(url);

            JSonMapper js(s);

            l.clear();
            if(!stree.empty())  {
                for( auto a : js.ptree().get_child(stree)) {
                    l.push_back(a.second);
                }
            } else {
                for( auto a : js.ptree()) {
                    l.push_back(a.second);
                }
            }
            return l;
        }

        Api & api() {
            return *_p_api;
        }

    private:
        Api * _p_api;
    };


}
#endif // APIAGENT_H
