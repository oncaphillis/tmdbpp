#ifndef TMDBPP_TVFILTER_H
#define TMDBPP_TVFILTER_H

#include <tmdbpp/apiagent.h>
#include <tmdbpp/tvseriescollection.h>

namespace tmdbpp {

    class TvFilter : public ApiAgent {
    private:
        typedef ApiAgent super;
    public:
        using super::super;

        TvSeriesCollection popular(const std::string & language,int page=1);
        TvSeriesCollection popular(int page=1);

    private:

    };
}
#endif // TVFILTER_H
