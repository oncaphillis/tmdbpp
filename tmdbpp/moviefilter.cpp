#include <tmdbpp/api.h>
#include <tmdbpp/moviefilter.h>
#include <tmdbpp/credits.h>

namespace tmdbpp {
    PersonalCredits MovieFilter::credits(int id,const std::string & lang) {
        PersonalCredits cr;
        std::stringstream ss;
        ss << id;
        std::string url = Api::BaseUrl+Api::MethodMovie+"/"+ss.str()+Api::ObjectCredits+"?"+
                 UrlArg("api_key",api().key());

        if(!lang.empty()) {
            url+="&"+UrlArg("language",lang);
        }

        return fetch(url,cr);
    }
}


