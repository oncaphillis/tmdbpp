#ifndef TMDBPP_TVSERIESCOLLECTION_H
#define TMDBPP_TVSERIESCOLLECTION_H

#include <tmdbpp/tvseries.h>
#include <tmdbpp/pagedcollection.h>

namespace tmdbpp {
    class TvSeriesCollection : public PagedCollection<TvSeriesSummary> {
    private:
        typedef PagedCollection<TvSeriesSummary> super;
    public:
        using super::super;
    private:
    };
}
#endif // TVSERIESCOLLECTION_H
