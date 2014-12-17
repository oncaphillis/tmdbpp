#ifndef APIAGENT_H
#define APIAGENT_H

#ifdef _WIN32
#include <tmdbpp/win_wget.h>
#else
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#endif
#include <string>

namespace tmdbpp {

    class Api;

    // The base class for every class that likes to access the TMDBB
    // Api

    class ApiAgent {
    private:
    public:
        ApiAgent(Api & api) : _p_api(&api) {

        }

        template<class T>
        T & fetch(const std::string & url,T & t) {
#ifdef _WIN32
            std::string s=WGet::instance().get(url);
            t = T(std::stringstream(s));
#else
            std::stringstream ss;
            curlpp::options::Url myUrl(url);
            curlpp::Easy myRequest;
            myRequest.setOpt(myUrl);
            myRequest.setOpt(curlpp::options::WriteStream(&ss));
            myRequest.perform();
            t = T(ss);
#endif
            return t;
        }

        Api & api() {
            return *_p_api;
        }

    private:
        Api * _p_api;
    };


}
#endif // APIAGENT_H
