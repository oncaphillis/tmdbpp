#ifndef TMDBPP_WIN_WGET_H
#define TMDBPP_WIN_WGET_H

#include <string>

namespace tmdbpp {
    class WGet {
    private:
    public:
        
        class Result {
        public:
            Result(const std::string & con , int cod ) : content(con), code(cod) {
            }
            std::string content;
            int code;
        };

        static WGet & instance();
        Result get(const std::string & url);

    private:
        WGet();
        ~WGet();

    };
    
};

#endif // TMDBPP_WIN_WGET_H
