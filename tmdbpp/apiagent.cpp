#include <tmdbpp/apiagent.h>
#include <tmdbpp/api.h>

namespace tmdbpp {
    std::string ApiAgent::fetch(const std::string & url) {
        return api().fetch(url);
    }
}
