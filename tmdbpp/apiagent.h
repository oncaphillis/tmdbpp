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


        /** @short Most basic fetch() method for an ApiAgent. Retrieve
            an URL addressed data as a string.

            - Returns empty string if the requested record it not available

            - Throws an exception if the I/O layer (i.e. network error) fails.
         */

        std::string fetch(const std::string & url);

        template<class T>
        T & fetch(const std::string & url,T & t) {
            int tr=0;
            // We try three times max to fetch the URL with a pause if 1 sec
            std::string r;
            while(tr++<3) {
                std::stringstream ss(r=fetch(url));
                try {
                    t = T(ss);
                    return t;
                }  catch(std::exception & ex) {
#ifdef _WIN32
                    ::Sleep(tr * 1000);
#else
                    ::sleep(tr);
#endif
                }
            }
            throw std::runtime_error(std::string("failed to fetch url:'")+url+"'\nlast reply:["+r+"]\n");
        }

        /** @short Fetch data from the TMDB server and read in a complete list of TMDBPP
            objects.

            The String is expected to represent a linear list or a named entry within the JSon Tree
            represents the list.

        */

        template<class T>
        std::list<T>  & fetch(const std::string & url,std::list<T> & lo,const std::string & stree="") {
            int t=0;
            std::string r;
            while(t++<3) {
                try {
                    std::string s = r = fetch(url);

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
#ifdef _WIN32
                    ::Sleep(t * 1000);
#else
                    ::sleep(t);
#endif
                }
            }
            throw std::runtime_error(std::string("failed to fetch url:'")+url+"'\nlast reply:["+r+"]\n");
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
