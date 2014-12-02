#include <tmdbpp/search.h>
#include <tmdbpp/api.h>

namespace tmdbpp {

    Movies Search::movies(const std::string & query,const std::string & language,int page) {
       std::stringstream ss;
       std::string url = Api::BaseUrl+Api::MethodSearch+Api::ObjectMovie+"?"+
                Arg("api_key",api().key())+"&"+Arg("query",query);

       if(page>0)
           url+="&"+Arg("page",page);

       std::cerr << ss.str() << std::endl;
       Movies m;
       return fetch(url,m);
    }
}
