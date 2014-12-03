#ifndef TMDBPP_KEYWORD_H
#define TMDBPP_KEYWORD_H

#include <tmdbpp/util.h>

namespace tmdbpp {
    class KeywordSummary : public NameIdHolder {
    private:
        typedef NameIdHolder super;
    public:
        using super::super;
    };

    class Keyword : public KeywordSummary {
    private:
        typedef KeywordSummary super;
    public:
        using super::super;

    };
}


#endif // KEYWORD_H
