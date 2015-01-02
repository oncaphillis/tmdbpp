#ifndef APIAGENT_H
#define APIAGENT_H

#ifdef _WIN32
    #include <tmdbpp/win_wget.h>
#else
    #include <curlpp/cURLpp.hpp>
    #include <curlpp/Easy.hpp>
    #include <curlpp/Info.hpp>
    #include <curlpp/Options.hpp>
#endif

#include <tmdbpp/util.h>

#include <string>
#include <sstream>
#include <list>

namespace tmdbpp {

    class Api;

    /** @short  The base class for every class that likes to access the TMDBB
        Api.

        On construction it gets passed a Api reference which it provides to its
        subclasses via the api() call.

        Provides basic I/O via the fetch() methods which reads URLs and transforms
        JSON to C++ classes.

    */

    class ApiAgent {
    private:
    public:
        ApiAgent(Api & api) : _p_api(&api) {
        }


        std::string fetch(const std::string & url);

        /** @short Fetch a single TMDBPP Object from the TMDB
            server.

            Data is read from a give URL and interprered as a JSON Tree.
            This is used to initialize a JSonMapper subclass given as
            template arg T.
        */

        template<class T>
        T & fetch(const std::string & url,T & t) {
            std::string r;
            std::stringstream ss(fetch(url));
            try {
                if(!ss.str().empty()) {
                        t = T(ss);
                } else  {
                    // We return an invalid e.g empty object on failure
                    t =  T();
                }
            } catch(std::exception & ex) {
                std::cerr << "CAUGHT::" << ex.what() << std::endl;
            }

            return t;
        }

        /** @short Fetch data from the TMDB server and read in a complete list of TMDBPP
            objects.

            The String is expected to represent a linear list or a named entry within the JSon Tree
            represents the list.

        */

        template<class T>
        std::list<T>  & fetch(const std::string & url,std::list<T> & lo,const std::string & stree="") {
            int t=0;

            while(t++<3) {
                try {
                    std::string s = fetch(url);

                    JSonMapper js(s);

                    std::list<T> l;
                    if(!stree.empty())  {
                        for( auto a : js.ptree().get_child(stree)) {
                            l.push_back(a.second);
                        }
                    } else {
                        for( auto a : js.ptree()) {
                            l.push_back(a.second);
                        }
                    }
                    lo.swap(l);
                    return lo;
                } catch(std::exception & ex) {
                    std::cerr << "caught '" << ex.what() << "' retry" << std::endl;
                }
            }
        }

        /** @short Return the Api we are associated with.
         */

        Api & api() {
            return *_p_api;
        }

    private:
        ErrorStatus _status;
        Api       * _p_api;
    };
}
#endif // APIAGENT_H
