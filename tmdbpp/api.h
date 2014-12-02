#ifndef TMDBPP_API_H
#define TMDBPP_API_H

#include <string>
#include <tmdbpp/util.h>
#include <tmdbpp/search.h>
#include <tmdbpp/get.h>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>


namespace tmdbpp {

    class Api {
    private:
    public:
        static const std::string BaseUrl;
        static const std::string MethodConfig;
        static const std::string MethodSearch;
        static const std::string MethodMovie;

        static const std::string ObjectMovie;


        static Api & instance(const std::string & apiKey="")  {
            static std::map<std::string,std::shared_ptr<Api>> m;
            std::string k=apiKey;
            if(k.empty()) {
                if(::getenv("apikey") == NULL ) {
                    throw std::runtime_error(" no env apikey set");
                }
                k = std::string(::getenv("apikey"));
            }


            if(m.find(k)==m.end()) {
                m[k] = std::shared_ptr<Api>(new Api(k));
            }

            return *m[k];
        }

        const Configuration & configuration() const {
            return _config;
        }

        const std::string & key() const {
            return _key;
        }

        Search  search() {
            return Search(*this);
        }

        Get get() {
            return Get(*this);
        }

    private:
        Api(const std::string & apiKey) : _key(apiKey) {
            curlpp::options::Url myUrl(BaseUrl+MethodConfig+"?api_key="+apiKey);
            std::cerr << "|" << myUrl.getValue() << "|" << std::endl;
            curlpp::Easy myRequest;
            std::stringstream ss;
            myRequest.setOpt(myUrl);
            myRequest.setOpt(curlpp::options::WriteStream(&ss));
            myRequest.perform();

            _config = Configuration(ss);
        }

        std::string   _key;
        Configuration _config;
    };
}

#endif // API_H
