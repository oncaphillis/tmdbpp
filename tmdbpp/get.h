#ifndef TMDBPP_GET_H
#define TMDBPP_GET_H

#include <tmdbpp/apiagent.h>
#include <tmdbpp/movie.h>
#include <tmdbpp/tvseries.h>
#include <tmdbpp/collection.h>
#include <tmdbpp/company.h>
#include <tmdbpp/keyword.h>
#include <tmdbpp/tvlister.h>
#include <tmdbpp/movielister.h>

namespace tmdbpp {

    class Get : public ApiAgent {
    private:
        typedef ApiAgent super;
    public:
        Get(Api & api ) : super(api) {
        }

        TvSeries tv(int id,const std::string & lang="");
        Media movie(int id,const std::string & lang="");
        Collection collection(int id);
        Company company(int id);
        Keyword keyword(int id);
        Person person(int id);
        TvLister tv();
        MovieLister movie();


    private:
    };
}
#endif // GET_H
