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
#include <tmdbpp/movielister.h>
#include <tmdbpp/authentication.h>


namespace tmdbpp {

    /** @short Api Proxy for all "get" operations i.e. single object result operations.
     *
     * "Gets" primarily fetch a single object loke Series, Move, Person
     * or a filter for a collection but not a list or other collection
     * of something. Sess Api::list() and Api::search() for this.
     */

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
        Authentication authentication();

    private:
    };
}
#endif // GET_H
