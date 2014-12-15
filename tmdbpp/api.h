#ifndef TMDBPP_API_H
#define TMDBPP_API_H

#include <string>
#include <tmdbpp/util.h>
#include <tmdbpp/search.h>
#include <tmdbpp/get.h>
#include <tmdbpp/apiagent.h>

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
        static const std::string MethodCollection;
        static const std::string MethodCompany;
        static const std::string MethodKeyword;
        static const std::string MethodTv;

        static const std::string ObjectCompany;
        static const std::string ObjectMovie;
        static const std::string ObjectCollection;
        static const std::string ObjectKeyword;
        static const std::string ObjectTv;
        static const std::string ObjectPopular;
        static const std::string ObjectTopRated;
        static const std::string ObjectAiringToday;
        static const std::string ObjectOnTheAir;


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
            ApiAgent ag(*this);
            std::string url = BaseUrl+MethodConfig+"?api_key="+apiKey;
            _config = ag.fetch(url,_config);
        }

        std::string   _key;
        Configuration _config;
    };
}

#endif // API_H
