#ifndef TMDBPP_MOVIES_H
#define TMDBPP_MOVIES_H

#include <tmdbpp/pagedcollection.h>
#include <tmdbpp/movie.h>

namespace tmdbpp {

    class Movies : public PagedCollection<MovieSummary> {
    private:
        typedef PagedCollection<MovieSummary> super;
    public:
        Movies() : super() {
        }
        Movies(const boost::property_tree::ptree & p) : super(p) {
        }        
        Movies(std::istream & is ) : super(is) {
        }
    private:
    };
}

#endif // MOVIES_H
