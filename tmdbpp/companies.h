#ifndef TMDBPP_COMPANIES_H
#define TMDBPP_COMPANIES_H

#include <tmdbpp/pagedcollection.h>
#include <tmdbpp/company.h>

namespace tmdbpp {

    class Companies : public PagedCollection<CompanyInfo> {
    private:
        typedef PagedCollection<CompanyInfo> super;
    public:
        using super::super;

        Companies & operator=(Companies c) {
            swap(*this,c);
        }

    private:
        friend
        void swap(Companies & c0,Companies &c1 ) {
            using std::swap;
            swap(static_cast<super &>(c0),static_cast<super &>(c1));
        }
    };
}
#endif // COMPANIES_H
