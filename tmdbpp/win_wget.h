#ifndef TMDBPP_WIN_WGET_H
#define TMDBPP_WIN_WGET_H

#include <string>

namespace tmdbpp {
    class WGet {
    private:
    public:
        static WGet & instance();
        std::string get(const std::string & url);


    private:
        WGet();
        ~WGet();

    };
    
};

#endif // TMDBPP_WIN_WGET_H
