#include <tmdbpp/get.h>
#include <tmdbpp/api.h>

#include <string>

namespace tmdbpp {
    Media Get::movie(int id,const std::string & lang) {
        Media m;
        std::stringstream ss;
        ss << id;
        std::string url = Api::BaseUrl+Api::MethodMovie+"/"+ss.str()+"?"+UrlArg("api_key",api().key());

        if(!lang.empty())
            url+="&"+UrlArg("language",lang);

        return fetch(url,m);
    }
    Collection Get::collection(int id) {
        Collection c;
        std::stringstream ss;
        ss << id;
        std::string url = Api::BaseUrl+Api::MethodCollection+"/"+ss.str()+"?"+UrlArg("api_key",api().key());
        return fetch(url,c);
    }
    Company Get::company(int id) {
        Company c;
        std::stringstream ss;
        ss << id;
        std::string url = Api::BaseUrl+Api::MethodCompany+"/"+ss.str()+"?"+UrlArg("api_key",api().key());
        return fetch(url,c);
    }
    Keyword Get::keyword(int id) {
        Keyword k;
        std::stringstream ss;
        ss << id;
        std::string url = Api::BaseUrl+Api::MethodKeyword+"/"+ss.str()+"?"+UrlArg("api_key",api().key());
        return fetch(url,k);
    }

    Person Get::person(int id) {
        Person p;
        std::stringstream ss;
        ss << id;
        std::string url = Api::BaseUrl+Api::MethodPerson+"/"+ss.str()+"?"+UrlArg("api_key",api().key());
        return fetch(url,p);
    }

    TvSeries Get::tv(int id,const std::string & lang) {
        TvSeries tv;
        std::stringstream ss;
        ss << id;
        std::string url = Api::BaseUrl+Api::MethodTv+"/"+ss.str()+"?"+UrlArg("api_key",api().key());

        if(!lang.empty())
            url+="&"+UrlArg("language",lang);

        return fetch(url,tv);
    }

    TvLister Get::tv() {
        return TvLister(api());
    }

    MovieLister Get::movie() {
        return MovieLister(api());
    }
}
