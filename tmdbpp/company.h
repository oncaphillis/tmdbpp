#ifndef TMDBPP_COMPANY_H
#define TMDBPP_COMPANY_H

#include <tmdbpp/util.h>
#include <string>

namespace tmdbpp {
    class Company : public CompanySummary {
    private:
        typedef CompanySummary super;
    public:
        using super::super;
    private:
        std::string logo_path() const {
            return ptree().get<std::string>("logo_path","");
        }
    };
}
#endif // COMPANY_H
