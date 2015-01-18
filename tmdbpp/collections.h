#ifndef TMDBPP_COLLECTIONS_H
#define TMDBPP_COLLECTIONS_H

#include <tmdbpp/collection.h>
#include <tmdbpp/pagedcollection.h>

namespace tmdbpp {
    class Collections : public PagedCollection<CollectionSummary> {
    private:
        typedef PagedCollection super;
    public:

        Collections() : super() {
        }
        Collections(const boost::property_tree::ptree & p) : super(p) {
        }        
        Collections(std::istream & is ) : super(is) {
        }
    };
}
#endif // COLLECTIONS_H
