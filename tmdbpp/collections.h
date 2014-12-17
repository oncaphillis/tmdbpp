#ifndef TMDBPP_COLLECTIONS_H
#define TMDBPP_COLLECTIONS_H

#include <tmdbpp/collection.h>
#include <tmdbpp/pagedcollection.h>

namespace tmdbpp {
    class Collections : public PagedCollection<CollectionSummary> {
    private:
        typedef PagedCollection super;
    public:
        using PagedCollection<CollectionSummary>::PagedCollection;
    };
}
#endif // COLLECTIONS_H
