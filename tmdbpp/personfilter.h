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

        PersonalCredits movie_credits(int id,const std::string & language="");
        PersonalCredits tv_credits(int id,const std::string & language="");
        PersonalCredits combined_credits(int id,const std::string & language="");
    private:

    };
}

#endif // PERSONFILTER_H
