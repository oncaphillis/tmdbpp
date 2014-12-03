#ifndef TMDBPP_COMPANY_H
#define TMDBPP_COMPANY_H

#include <tmdbpp/util.h>
#include <string>

namespace tmdbpp {
    class CompanyInfo : public CompanySummary {
    private:
        typedef CompanySummary super;
    public:
        using super::super;
    private:
        std::string logo_path() const {
            return ptree().get<std::string>("logo_path","");
        }
    };

    class Company : public CompanyInfo {
    private:
        typedef CompanyInfo super;
    public:
        using super::super;

        std::string description() const {
            return ptree().get<std::string>("description","");
        }
        std::string headquarters() const {
            return ptree().get<std::string>("headquarters","");
        }
        std::string homepage() const {
            return ptree().get<std::string>("homepage","");
        }
        int parent_company() const {
            return ptree().get<int>("parent_company",0);
        }
    };
}
#endif // COMPANY_H
