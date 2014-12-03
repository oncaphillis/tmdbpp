#ifndef TMDBPP_KEYWORDS_H
#define TMDPBB_KEYWORDS_H

#include <tmdbpp/keyword.h>
#include <tmdbpp/pagedcollection.h>

namespace tmdbpp {
    class Keywords : public PagedCollection<KeywordSummary> {
    private:
        typedef PagedCollection<KeywordSummary> super;
    public:
        Keywords() : super() {
        }

        Keywords(std::istream & is) : super(is) {
        }
    };
}

#endif // KEYWORDS_H
