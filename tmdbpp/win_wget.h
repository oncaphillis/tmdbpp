#ifndef TMDBPP_WIN_WGET_H
#define TMDBPP_WIN_WGET_H

#include <winsock2.h>
#include <Winhttp.h>
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
        HINTERNET _hsession;
    };
    
};

#endif // TMDBPP_WIN_WGET_H
