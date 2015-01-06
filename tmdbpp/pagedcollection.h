#ifndef TMDBPP_PAGEDCOLLECTION_H
#define TMDBPP_PAGEDCOLLECTION_H

#include <tmdbpp/util.h>

namespace tmdbpp {

    /** @short A single page in a collection of entries from
        TmDb.

        Many results are returned in the form of JSON pages.
        Each page holds a (1) variable number of entries. (2)
        The total number of entries and the total number of pages
        to expect.
    */

    template<class ITEM>
    class PagedCollection : public JSonMapper {
    private:
        typedef JSonMapper super;
    public:

        typedef typename std::list<ITEM>::const_iterator const_iterator;

        PagedCollection() : super() {
        }

        PagedCollection(const PagedCollection & pc) : super(pc) {
            setup();
        }

        PagedCollection(PagedCollection && pc) : super(pc) {
            swap(*this,pc);
        }

        PagedCollection(const boost::property_tree::ptree & pt) : super(pt) {
            setup();
        }

        PagedCollection(std::istream & is) : super(is) {
            setup();
        }

        PagedCollection & operator=(PagedCollection pc) {
            swap(*this,pc);
            return *this;
        }

        const std::list<ITEM> & list() const {
            return _list;
        }


        int page() const {
            return this->ptree().template get<int>("page",0);
        }

        int total_pages() const {
            return this->ptree().template get<int>("total_pages",0);
        }

        int total_results() const {
            return this->ptree().template get<int>("total_results",0);
        }

        const_iterator begin() const {
            return _list.begin();
        }

        const_iterator end() const {
            return _list.end();
        }

        bool empty() const {
            return _list.empty();
        }

        size_t size() const {
            return _list.size();
        }
    private:

        void setup() {
            for(auto n : this->ptree().get_child("results")) {
                _list.push_back(ITEM(n.second));
            }
        }

        friend
        void swap(PagedCollection & c0,PagedCollection &c1) {
            using std::swap;
            swap(static_cast<super &>(c0),static_cast<super &>(c1));
            c0._list.swap(c1._list);
        }

        std::list<ITEM> _list;
    };
}
#endif // PAGEDCOLLECTION_H
