#ifndef TMDBPP_API_H
#define TMDBPP_API_H

#include <tmdbpp/util.h>
#include <tmdbpp/search.h>
#include <tmdbpp/get.h>
#include <tmdbpp/apiagent.h>

#include <string>
#include <map>

namespace tmdbpp {
    
    /** @short Central manager of the TMDBPP interface. Holds the API key
        and the configuration for a given connection.

        An Api can not be constructed directly but has to be retrieved
        via the instance() method.

        @see Configuration, ApiAgent
    */

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
        static const std::string MethodPerson;
        static const std::string MethodGenre;

        static const std::string ObjectCompany;
        static const std::string ObjectMovie;
        static const std::string ObjectCollection;
        static const std::string ObjectKeyword;
        static const std::string ObjectTv;
        static const std::string ObjectPopular;
        static const std::string ObjectTopRated;
        static const std::string ObjectAiringToday;
        static const std::string ObjectOnTheAir;
        static const std::string ObjectPerson;
        static const std::string ObjectCredits;
        static const std::string ObjectMovieCredits;
        static const std::string ObjectTvCredits;
        static const std::string ObjectCombinedCredits;
        static const std::string ObjectList;

        static const std::string OptionTv;

        enum StatusCode {
            StatusInvalidId = 6
        };
        
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
