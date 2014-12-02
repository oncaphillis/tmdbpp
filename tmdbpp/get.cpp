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
}

