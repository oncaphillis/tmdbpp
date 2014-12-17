#ifndef TMDBPP_KEYWORD_H
#define TMDBPP_KEYWORD_H

#include <tmdbpp/util.h>

namespace tmdbpp {
    class KeywordSummary : public NameIdHolder {
    private:
        typedef NameIdHolder super;
    public:
        KeywordSummary() : super() {
        }
        KeywordSummary(const boost::property_tree::ptree & p) : super(p) {
        }        
        KeywordSummary(std::istream & is ) : super(is) {
        }
    };

    class Keyword : public KeywordSummary {
    private:
        typedef KeywordSummary super;
    public:
        Keyword() : super() {
        }
        Keyword(const boost::property_tree::ptree & p) : super(p) {
        }        
        Keyword(std::istream & is ) : super(is) {
        }
    };
}


#endif // KEYWORD_H
