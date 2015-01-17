#include <tmdbpp/search.h>
#include <tmdbpp/api.h>

namespace tmdbpp {

    Movies Search::movie(const std::string & query,const std::string & language,int page) {
       Url u = api().url().add(Api::MethodSearch).add(Api::ObjectMovie).add(UrlArg("query",query));

       if(page>0)
           u = u.add(UrlArg("page",page));

       std::cerr << " -- " << u << " -- " << std::endl;

       Movies m;

       return fetch(u,m);
    }

    Companies Search::company(const std::string & query,int page) {
       Url u = api().url().add(Api::MethodSearch).add(Api::ObjectCompany).add(UrlArg("query",query));

       if(page>0)
           u += UrlArg("page",page);

       Companies c;
       return fetch(u,c);
    }

    Collections Search::collection(const std::string & query,int page) {

       Url u = api().url().add(Api::MethodSearch).add(Api::ObjectCollection).add(UrlArg("query",query));

       if(page>0)
           u += UrlArg("page",page);

       Collections c;
       return fetch(u,c);
    }

    Keywords Search::keyword(const std::string & query,int page) {
       Url u = api().url().add(Api::MethodSearch).add(Api::ObjectKeyword).add(UrlArg("query",query));

       if(page>0)
           u += UrlArg("page",page);

       Keywords k;
       return fetch(u,k);
    }

    TvSeriesCollection Search::tv(const std::string & query,int page) {
        TvSeriesCollection coll;
        Url u = api().url().add(Api::MethodSearch).add(Api::ObjectTv).add(UrlArg("query",query));

        if(page>0)
            u += UrlArg("page",page);

        return fetch(u,coll);
    }

    Persons Search::person(const std::string & query,int page,bool adult) {
        Persons coll;

        Url u = api().url().add(Api::MethodSearch).add(Api::ObjectPerson).add(UrlArg("query",query));

        if(page>0)
            u += UrlArg("page",page);

        u += UrlArg("include_adult",adult);

        return fetch(u,coll);
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
