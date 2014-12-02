#ifndef TMDBPP_MOVIES_H
#define TMDBPP_MOVIES_H

#include <tmdbpp/movie.h>

namespace tmdbpp {
    class Movies : public JSonMapper {
    private:
        typedef JSonMapper super;
    public:
        Movies()  : super() {

        }

        Movies(std::istream & is) : super(is) {
            setup();
        }

        Movies( const boost::property_tree::ptree & pt ) : super(pt) {
            setup();
        }

        Movies( const Movies & m ) : super(m) {
            setup();
        }

        Movies( Movies && m ) : super() {
            swap(*this,m);
        }

        int page() const {
            return ptree().get<int>("page",0);
        }

        int total_pages() const {
            return ptree().get<int>("total_pages",0);
        }

        int total_results() const {
            return ptree().get<int>("total_results",0);
        }

        const std::list<MovieSummary> & list() const {
            return _list;
        }

        Movies & operator=(Movies m) {
            swap(*this,m);
            return *this;
        }

    private:
        friend void swap(Movies & m0 ,Movies & m1) {
            using std::swap;
            swap(static_cast<super &>(m0),static_cast<super &>(m1));
            m0._list.swap(m1._list);
        }

        void setup() {
            for(auto n : ptree().get_child("results")) {
                _list.push_back(MovieSummary(n.second));
            }
        }
        std::list<MovieSummary> _list;
    };
}
#endif // MOVIES_H
