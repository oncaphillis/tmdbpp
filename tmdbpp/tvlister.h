#ifndef TMDBPP_TVLISTER_H
#define TMDBPP_TVLISTER_H

#include <tmdbpp/util.h>
#include <tmdbpp/apiagent.h>

namespace tmdbpp {

    class TvLister : public ApiAgent  {
    private:
        typedef ApiAgent super;
    public:
        TvLister(Api & api) : super(api) {
        }

        std::list<Genre> genres();
    };
}
#endif // TVLISTER_H
