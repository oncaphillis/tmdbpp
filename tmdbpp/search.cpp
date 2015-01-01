#include <tmdbpp/search.h>
#include <tmdbpp/api.h>

namespace tmdbpp {

    Movies Search::movie(const std::string & query,const std::string & language,int page) {
       std::stringstream ss;
       std::string url = Api::BaseUrl+Api::MethodSearch+Api::ObjectMovie+"?"+
                UrlArg("api_key",api().key())+"&"+UrlArg("query",query);

       if(page>0)
           url+="&"+UrlArg("page",page);

       Movies m;
       return fetch(url,m);
    }

    Companies Search::company(const std::string & query,int page) {
       std::stringstream ss;
       std::string url = Api::BaseUrl+Api::MethodSearch+Api::ObjectCompany+"?"+
                UrlArg("api_key",api().key())+"&"+UrlArg("query",query);

       if(page>0)
           url+="&"+UrlArg("page",page);

       Companies c;
       return fetch(url,c);
    }

    Collections Search::collection(const std::string & query,int page) {
       std::stringstream ss;
       std::string url = Api::BaseUrl+Api::MethodSearch+Api::ObjectCollection+"?"+
                UrlArg("api_key",api().key())+"&"+UrlArg("query",query);

       if(page>0)
           url+="&"+UrlArg("page",page);

       Collections c;
       return fetch(url,c);
    }

    Keywords Search::keyword(const std::string & query,int page) {
       std::stringstream ss;
       std::string url = Api::BaseUrl+Api::MethodSearch+Api::ObjectKeyword+"?"+
                UrlArg("api_key",api().key())+"&"+UrlArg("query",query);

       if(page>0)
           url+="&"+UrlArg("page",page);

       Keywords k;
       return fetch(url,k);
    }

    TvSeriesCollection Search::tv(const std::string & query,int page) {
        TvSeriesCollection coll;
        std::stringstream ss;
        std::string url = Api::BaseUrl+Api::MethodSearch+Api::ObjectTv+"?"+
                 UrlArg("api_key",api().key())+"&"+UrlArg("query",query);

        if(page>0)
            url+="&"+UrlArg("page",page);

        return fetch(url,coll);
    }

    Persons Search::person(const std::string & query,int page,bool adult) {
        Persons coll;
        std::stringstream ss;
        std::string url = Api::BaseUrl+Api::MethodSearch+Api::ObjectPerson+"?"+
                 UrlArg("api_key",api().key())+"&"+UrlArg("query",query);

        if(page>0)
            url+="&"+UrlArg("page",page);

        url+="&"+UrlArg("include_adult",adult);
        return fetch(url,coll);
    }

    MovieFilter Search::movie() {
        return MovieFilter(this->api());
    }

    TvFilter Search::tv() {
        return TvFilter(this->api());
    }

    PersonFilter Search::person() {
        return PersonFilter(this->api());
    }
}
