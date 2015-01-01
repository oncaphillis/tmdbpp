#include <tmdbpp/tvlister.h>
#include <tmdbpp/api.h>
#include <string>

namespace tmdbpp {
    std::list<Genre> TvLister::genres() {
        std::list<Genre> ges;
        std::string url = Api::BaseUrl+Api::MethodGenre+Api::OptionTv+Api::ObjectList+"?"
                +UrlArg("api_key",api().key());

        return fetch(url,ges,"genres");
    }
}
