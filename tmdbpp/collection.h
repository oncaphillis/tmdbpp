#ifndef TMDBPP_COLLECTION_H
#define TMDBPP_COLLECTION_H

#include <tmdbpp/util.h>
#include <tmdbpp/movie.h>

namespace tmdbpp {
    class CollectionSummary : public NameIdHolder {
    private:
        typedef NameIdHolder super;
    public:
        using super::super;
        std::string backdrop_path() const {
            return ptree().get<std::string>("backdrop_path","");
        }
        std::string poster_path() const {
            return ptree().get<std::string>("poster_path","");
        }
    };

    class Part : public MovieReleaseSummary  {
    private:
        typedef MovieReleaseSummary super;
    public:
        using super::super;
    };

    class Collection : public CollectionSummary {
    private:
        typedef CollectionSummary super;
    public:
        Collection() : super() {

        }
        Collection(std::istream & is) : super(is) {

        }

        Collection(const boost::property_tree::ptree &pt) : super(pt) {
            setup();
        }

        Collection(const Collection & c) : super(c) {
            setup();
        }

        Collection(Collection && c) : super(c) {
            swap(*this,c);
        }

        Collection & operator=(Collection c) {
            swap(*this,c);
            return *this;
        }
        const std::list<Part> & parts() const  {
            return _parts;
        }
    private:
        friend
        void swap(Collection &c0,Collection &c1) {
            using std::swap;
            swap(static_cast<super &>(c0),static_cast<super &>(c1));
            c0._parts.swap(c1._parts);
        }

        void setup() {
            for(const auto & n : ptree().get_child("parts")) {
                _parts.push_back(n.second);
            }
        }
        std::list<Part> _parts;
    };
}


#endif // TMDBPP_COLLECTION_H
