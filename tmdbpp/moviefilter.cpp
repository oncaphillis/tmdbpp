#include <tmdbpp/api.h>
#include <tmdbpp/moviefilter.h>
#include <tmdbpp/credits.h>

namespace tmdbpp {
    MediaCredits MovieFilter::credits(int id,const std::string & lang) {
        MediaCredits cr;
        std::stringstream ss;
        ss << id;
        std::string url = Api::BaseUrl+Api::MethodMovie+"/"+ss.str()+Api::ObjectCredits+"?"+
                 Arg("api_key",api().key());

        if(!lang.empty()) {
            url+="&"+Arg("language",lang);
        }

        return fetch(url,cr);
    }
}


