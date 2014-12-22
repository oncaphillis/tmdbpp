#ifndef TMDBPP_TVFILTER_H
#define TMDBPP_TVFILTER_H

#include <tmdbpp/apiagent.h>
#include <tmdbpp/tvseriescollection.h>

namespace tmdbpp {

    class TvFilter : public ApiAgent {
    private:
        typedef ApiAgent super;
    public:
        TvFilter(Api &a ) : super(a) {
        }

        TvSeriesCollection popular(const std::string & language,int page=1);
        TvSeriesCollection popular(int page=1);

        TvSeriesCollection top_rated(const std::string & language,int page=1);
        TvSeriesCollection top_rated(int page=1);

        TvSeriesCollection airing_today(const std::string & language,int page=1);
        TvSeriesCollection airing_today(int page=1);

        TvSeriesCollection on_the_air(const std::string & language,int page=1);
        TvSeriesCollection on_the_air(int page=1);


    private:

    };
}
#endif // TVFILTER_H
