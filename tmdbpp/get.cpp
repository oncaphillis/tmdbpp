#include <tmdbpp/get.h>
#include <tmdbpp/api.h>

#include <string>

namespace tmdbpp {
    Movie Get::movie(int id) {
        Movie m;
        std::stringstream ss;
        ss << id;
        std::string url = Api::BaseUrl+Api::MethodMovie+"/"+ss.str()+"?"+Arg("api_key",api().key());
        return fetch(url,m);
    }
    Collection Get::collection(int id) {
        Collection c;
        std::stringstream ss;
        ss << id;
        std::string url = Api::BaseUrl+Api::MethodCollection+"/"+ss.str()+"?"+Arg("api_key",api().key());
        return fetch(url,c);
    }
    Company Get::company(int id) {
        Company c;
        std::stringstream ss;
        ss << id;
        std::string url = Api::BaseUrl+Api::MethodCompany+"/"+ss.str()+"?"+Arg("api_key",api().key());
        return fetch(url,c);
    }
    Keyword Get::keyword(int id) {
        Keyword k;
        std::stringstream ss;
        ss << id;
        std::string url = Api::BaseUrl+Api::MethodKeyword+"/"+ss.str()+"?"+Arg("api_key",api().key());
        return fetch(url,k);
    }
}

