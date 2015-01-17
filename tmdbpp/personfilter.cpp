#include <tmdbpp/api.h>
#include <tmdbpp/personfilter.h>
#include <tmdbpp/credits.h>

namespace tmdbpp {

    /** @short Return all movies the person was casted for.
     */

    MovieCredits PersonFilter::movie_credits(int id,const std::string & lang) {
        MovieCredits cr;
        std::stringstream ss;
        ss << id;
        std::string url = Api::BaseUrl+Api::MethodPerson+"/"+ss.str()+Api::ObjectMovieCredits+"?"+
                 UrlArg("api_key",api().key());

        if(!lang.empty()) {
            url+="&"+UrlArg("language",lang);
        }

        return fetch(url,cr);
    }

    /** @short Return all TV shows the person was casted for
     */

    MovieCredits PersonFilter::tv_credits(int id,const std::string & lang) {
        MovieCredits cr;
        std::stringstream ss;
        ss << id;
        std::string url = Api::BaseUrl+Api::MethodPerson+"/"+ss.str()+Api::ObjectTvCredits+"?"+
                 UrlArg("api_key",api().key());

        if(!lang.empty()) {
            url+="&"+UrlArg("language",lang);
        }

        return fetch(url,cr);
    }

    /** @short Return all TV Shows/Movies the person was casted for.
     */

    MovieCredits PersonFilter::combined_credits(int id,const std::string & lang) {
        MovieCredits coll;
        std::stringstream ss;
        ss << id;
        std::string url = Api::BaseUrl+Api::MethodPerson+"/" + ss.str() + Api::ObjectCombinedCredits+"?"+
                 UrlArg("api_key",api().key());

        if(!lang.empty()) {
            url+="&"+UrlArg("language",lang);
        }

        return fetch(url,coll);
    }
}

