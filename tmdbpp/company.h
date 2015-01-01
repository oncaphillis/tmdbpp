#ifndef TMDBPP_COMPANY_H
#define TMDBPP_COMPANY_H

#include <tmdbpp/util.h>
#include <string>

namespace tmdbpp {

    class CompanySummary : public NameIdHolder {
    private:
        typedef NameIdHolder super;
    public:
        CompanySummary() : super() {
        }
        CompanySummary(const boost::property_tree::ptree & p) : super(p) {
        }
        CompanySummary(std::istream & is) : super(is) {
        }
        std::string logo_path() const {
            return ptree().get<std::string>("logo_path","");
        }
    };

    class Company : public CompanySummary {
    private:
        typedef CompanySummary super;
    public:
        Company() : super() {
        }
        Company(const boost::property_tree::ptree & p) : super(p) {
        }        
        Company(std::istream & is ) : super(is) {
        }

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
