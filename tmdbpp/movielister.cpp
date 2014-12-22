#include <tmdbpp/movielister.h>
#include <tmdbpp/api.h>
#include <string>

namespace tmdbpp {
    std::list<Genre> MovieLister::genres() {
        std::list<Genre> ges;
        std::string url = Api::BaseUrl+Api::MethodGenre+Api::ObjectList+"?"
                +Arg("api_key",api().key());

        return fetch(url,ges,"genres");
    }
}
