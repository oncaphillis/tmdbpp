#include <tmdbpp/api.h>
#include <tmdbpp/personfilter.h>
#include <tmdbpp/credits.h>

namespace tmdbpp {
    PersonalCredits PersonFilter::movie_credits(int id,const std::string & lang) {
        PersonalCredits cr;
        std::stringstream ss;
        ss << id;
        std::string url = Api::BaseUrl+Api::MethodPerson+"/"+ss.str()+Api::ObjectMovieCredits+"?"+
                 Arg("api_key",api().key());

        if(!lang.empty()) {
            url+="&"+Arg("language",lang);
        }

        return fetch(url,cr);
    }



    PersonalCredits PersonFilter::tv_credits(int id,const std::string & lang) {
        PersonalCredits cr;
        std::stringstream ss;
        ss << id;
        std::string url = Api::BaseUrl+Api::MethodPerson+"/"+ss.str()+Api::ObjectTvCredits+"?"+
                 Arg("api_key",api().key());

        if(!lang.empty()) {
            url+="&"+Arg("language",lang);
        }

        return fetch(url,cr);
    }


    PersonalCredits PersonFilter::combined_credits(int id,const std::string & lang) {
        PersonalCredits coll;
        std::stringstream ss;
        ss << id;
        std::string url = Api::BaseUrl+Api::MethodPerson+"/" + ss.str() + Api::ObjectCombinedCredits+"?"+
                 Arg("api_key",api().key());

        if(!lang.empty()) {
            url+="&"+Arg("language",lang);
        }

        return fetch(url,coll);
    }
}

