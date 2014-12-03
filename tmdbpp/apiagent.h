#ifndef APIAGENT_H
#define APIAGENT_H

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

namespace tmdbpp {

    class Api;

    class ApiAgent {
    private:
    public:
        ApiAgent(Api & api) : _p_api(&api) {

        }

        template<class T>
        T & fetch(const std::string & url,T & t) {
            std::stringstream ss;
            curlpp::options::Url myUrl(url);
            curlpp::Easy myRequest;
            myRequest.setOpt(myUrl);
            myRequest.setOpt(curlpp::options::WriteStream(&ss));
            myRequest.perform();
            t = T(ss);
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
