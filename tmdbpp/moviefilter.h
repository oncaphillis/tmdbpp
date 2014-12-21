#ifndef TMDBPP_MOVIEFILTER_H
#define TMDBPP_MOVIEFILTER_H

#include <tmdbpp/apiagent.h>
#include <tmdbpp/credits.h>

namespace tmdbpp {

    class MovieFilter : public ApiAgent {
    private:
        typedef ApiAgent super;
    public:
        MovieFilter(Api &a ) : super(a) {
        }

        MediaCredits credits(int id,const std::string & language="");
    private:
    };
}

#endif // MOVIEFILTER_H
