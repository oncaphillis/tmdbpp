#ifndef TMDBPP_MOVIES_H
#define TMDBPP_MOVIES_H

#include <tmdbpp/pagedcollection.h>
#include <tmdbpp/movie.h>

namespace tmdbpp {

    class Movies : public PagedCollection<MovieSummary> {
    private:
        typedef PagedCollection<MovieSummary> super;
    public:
        using PagedCollection<MovieSummary>::PagedCollection;
    private:
    };
}

#endif // MOVIES_H
