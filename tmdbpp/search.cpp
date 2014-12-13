#include <tmdbpp/search.h>
#include <tmdbpp/api.h>

namespace tmdbpp {

    Movies Search::movie(const std::string & query,const std::string & language,int page) {
       std::stringstream ss;
       std::string url = Api::BaseUrl+Api::MethodSearch+Api::ObjectMovie+"?"+
                Arg("api_key",api().key())+"&"+Arg("query",query);

       if(page>0)
           url+="&"+Arg("page",page);

       Movies m;
       return fetch(url,m);
    }

    Companies Search::company(const std::string & query,int page) {
       std::stringstream ss;
       std::string url = Api::BaseUrl+Api::MethodSearch+Api::ObjectCompany+"?"+
                Arg("api_key",api().key())+"&"+Arg("query",query);

       if(page>0)
           url+="&"+Arg("page",page);

       Companies c;
       return fetch(url,c);
    }

    Collections Search::collection(const std::string & query,int page) {
       std::stringstream ss;
       std::string url = Api::BaseUrl+Api::MethodSearch+Api::ObjectCollection+"?"+
                Arg("api_key",api().key())+"&"+Arg("query",query);

       if(page>0)
           url+="&"+Arg("page",page);

       Collections c;
       return fetch(url,c);
    }

    Keywords Search::keyword(const std::string & query,int page) {
       std::stringstream ss;
       std::string url = Api::BaseUrl+Api::MethodSearch+Api::ObjectKeyword+"?"+
                Arg("api_key",api().key())+"&"+Arg("query",query);

       if(page>0)
           url+="&"+Arg("page",page);

       Keywords k;
       return fetch(url,k);
    }

    TvSeriesCollection Search::tv(const std::string & query,int page) {
       TvSeriesCollection coll;
       std::stringstream ss;
       std::string url = Api::BaseUrl+Api::MethodSearch+Api::ObjectTv+"?"+
                Arg("api_key",api().key())+"&"+Arg("query",query);

       if(page>0)
           url+="&"+Arg("page",page);

       std::cerr << "[" << url << "]" << std::endl;

       return fetch(url,coll);
    }

}
