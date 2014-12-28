#ifndef APIAGENT_H
#define APIAGENT_H

#ifdef _WIN32
#include <tmdbpp/win_wget.h>
#else
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#endif

#include <tmdbpp/util.h>
#include <string>
#include <sstream>

namespace tmdbpp {

    class Api;

    // The base class for every class that likes to access the TMDBB
    // Api

    class ApiAgent {
    private:
    public:
        ApiAgent(Api & api) : _p_api(&api) {

        }

        std::string fetch(const std::string & url) {
#ifdef _WIN32
            return WGet::instance().get(url);
#else
            try {
                std::stringstream ss;
                curlpp::options::Url myUrl(url);
                curlpp::Easy myRequest;

                myRequest.setOpt(myUrl);
                myRequest.setOpt(curlpp::options::WriteStream(&ss));
                myRequest.setOpt(curlpp::options::FailOnError(true));

                myRequest.perform();
                return ss.str();
            } catch(...) {
                std::cerr << "CAUGHT SOMETHING " << std::endl;
                return "";
            }
#endif
        }

        template<class T>
        T & fetch(const std::string & url,T & t) {
            int tr=0;
            while(tr++<3) {
                std::stringstream ss(fetch(url));
                try {
                    t = T(ss);
                }  catch(std::exception & ex) {
                    std::cerr << "Caught:'" << ex.what() << "' retry #" << tr << std::endl;
                    ::sleep(1);
                }
            }
            return t;
        }

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
                    ::sleep(1);
                }
            }
        }

        Api & api() {
            return *_p_api;
        }

    private:
        Api * _p_api;
    };


}
#endif // APIAGENT_H
