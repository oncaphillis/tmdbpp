#ifndef TMDBPP_PERSONS_H
#define TMDBPP_PERSONS_H
#include <tmdbpp/person.h>
#include <tmdbpp/pagedcollection.h>

namespace tmdbpp {

    class Persons : public PagedCollection<PersonSummary> {
    private:
        typedef PagedCollection<PersonSummary> super;
    public:
        Persons() : super() {
        }
        Persons(const boost::property_tree::ptree & p) : super(p) {
        }
        Persons(std::istream & is ) : super(is) {
        }
    private:

    };
};

#endif // PERSONS_H
