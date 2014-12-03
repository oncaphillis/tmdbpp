#ifndef TMDBPP_PAGEDCOLLECTION_H
#define TMDBPP_PAGEDCOLLECTION_H

#include <tmdbpp/util.h>

namespace tmdbpp {

    class PagedCollectionBase : public JSonMapper {
    private:
        typedef JSonMapper super;
    public:
        using super::super;

        int page() const {
            return this->ptree().get<int>("page",0);
        }

        int total_pages() const {
            return this->ptree().get<int>("total_pages",0);
        }

        int total_results() const {
            return this->ptree().get<int>("total_results",0);
        }
    };

    template<class ITEM>
    class PagedCollection : public PagedCollectionBase {
    private:
        typedef PagedCollectionBase super;
    public:

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

    private:

        void setup() {
            for(auto n : ptree().get_child("results")) {
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
