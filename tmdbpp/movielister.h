#ifndef TMDBPP_MOVIELISTER_H
#define TMDBPP_MOVIELISTER_H

#include <tmdbpp/util.h>
#include <tmdbpp/apiagent.h>

namespace tmdbpp {

    class MovieLister : public ApiAgent  {
    private:
        typedef ApiAgent super;
    public:
        MovieLister(Api & api) : super(api) {
        }

        std::list<Genre> genres();
    };
}

#endif // MOVIELISTER_H
