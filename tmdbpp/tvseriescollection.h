#ifndef TMDBPP_TVSERIESCOLLECTION_H
#define TMDBPP_TVSERIESCOLLECTION_H

#include <tmdbpp/tvseries.h>
#include <tmdbpp/pagedcollection.h>

namespace tmdbpp {
    class TvSeriesCollection : public PagedCollection<TvSeriesSummary> {
    private:
        typedef PagedCollection<TvSeriesSummary> super;
    public:
        TvSeriesCollection () : super() {
        }
        TvSeriesCollection (const boost::property_tree::ptree & p) : super(p) {
        }        
        TvSeriesCollection (std::istream & is ) : super(is) {
        }
    private:
    };
}
#endif // TVSERIESCOLLECTION_H
