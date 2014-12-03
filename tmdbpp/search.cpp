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
}
