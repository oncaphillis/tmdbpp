#ifndef TMDBPP_SEARCH_H
#define TMDBPP_SEARCH_H

#include <list>
#include <string>

#include <tmdbpp/movies.h>
#include <tmdbpp/companies.h>
#include <tmdbpp/collections.h>
#include <tmdbpp/apiagent.h>

namespace tmdbpp {
    class Api;
    class Search : public ApiAgent {
    private:
        typedef ApiAgent super;
    public:
        Search(Api & api) : super(api) {
        }

        Movies      movie(const std::string & query,const std::string & language="",int page=-1);
        Companies   company(const std::string &query,int page=-1);
        Collections collection(const std::string &query,int page=-1);
    };
}


#endif // SEARCH_H
