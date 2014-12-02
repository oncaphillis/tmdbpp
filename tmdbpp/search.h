#ifndef TMDBPP_SEARCH_H
#define TMDBPP_SEARCH_H

#include <list>
#include <string>
#include <tmdbpp/movie.h>
#include <tmdbpp/movies.h>
#include <tmdbpp/apiagent.h>

namespace tmdbpp {
    class Api;
    class Search : public ApiAgent {
    private:
        typedef ApiAgent super;
    public:
        Search(Api & api) : super(api) {
        }

        Movies movies(const std::string & query,const std::string & language="",int page=-1);
    };
}


#endif // SEARCH_H
