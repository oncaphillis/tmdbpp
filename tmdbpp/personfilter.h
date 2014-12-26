#ifndef TMDBPP_PERSONFILTER_H
#define TMDBPP_PERSONFILTER_H

#include <tmdbpp/credits.h>

namespace tmdbpp {

    class PersonFilter : public ApiAgent {
    private:
        typedef ApiAgent super;
    public:
        PersonFilter(Api &a ) : super(a) {
        }

        MovieCredits movie_credits(int id,const std::string & language="");
        MovieCredits tv_credits(int id,const std::string & language="");
        MovieCredits combined_credits(int id,const std::string & language="");
    private:

    };
}

#endif // PERSONFILTER_H
