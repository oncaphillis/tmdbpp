#include <tmdbpp/api.h>

namespace tmdbpp {
    const std::string Api::BaseUrl          = "https://api.themoviedb.org/3";
    const std::string Api::MethodConfig     = "/configuration";
    const std::string Api::MethodSearch     = "/search";
    const std::string Api::MethodMovie      = "/movie";
    const std::string Api::MethodCollection = "/collection";
    const std::string Api::MethodCompany    = "/company";
    const std::string Api::MethodKeyword    = "/keyword";
    const std::string Api::MethodTv         = "/tv";
    const std::string Api::MethodPerson     = "/person";
    const std::string Api::MethodGenre      = "/genre";

    const std::string Api::ObjectMovie             = "/movie";
    const std::string Api::ObjectCompany           = "/company";
    const std::string Api::ObjectCollection        = "/collection";
    const std::string Api::ObjectKeyword           = "/keyword";
    const std::string Api::ObjectTv                = "/tv";
    const std::string Api::ObjectPopular           = "/popular";
    const std::string Api::ObjectTopRated          = "/top_rated";
    const std::string Api::ObjectAiringToday       = "/airing_today";
    const std::string Api::ObjectPerson            = "/person";
    const std::string Api::ObjectCredits           = "/credits";
    const std::string Api::ObjectMovieCredits      = "/movie_credits";
    const std::string Api::ObjectTvCredits         = "/tv_credits";
    const std::string Api::ObjectCombinedCredits   = "/combined_credits";
    const std::string Api::ObjectList              = "/list";

    const std::string Api::OptionTv                = "/tv";

    /** @short Most basic fetch() method for an ApiAgent. Retrieve
        an URL addressed data as a string.
    */

    std::string Api::fetch(const std::string & url) {
        int retry = 0;
        while(retry++<3) {
            try {
                _status = ErrorStatus();
#ifdef _WIN32
                return WGet::instance().get(url);
#else
                std::stringstream ss;
                curlpp::options::Url myUrl(url);
                curlpp::Easy myRequest;

                myRequest.setOpt(myUrl);
                myRequest.setOpt(curlpp::options::WriteStream(&ss));
                myRequest.setOpt(curlpp::options::FailOnError(false));

                myRequest.perform();

                int c;
                curlpp::InfoGetter::get(myRequest,CURLINFO_RESPONSE_CODE,c);

                if(c!=200) {

                    if(!ss.str().empty()) {
                        _status = ErrorStatus(ss.str());
                    }

                    if(c==404 && !ss.str().empty() && _status.status_code() == Api::StatusCode::StatusInvalidId) {
                        return "";
                    }
                    // Gateway timeout.. wait and try again max 3 times
                    if(c==504) {
#ifdef _WIN32
                      ::Sleep(retry * 1000);
#else
                        ::sleep(retry);
#endif
                        continue;
                    }

                    std::stringstream ss;

                    ss << "code #" << c << "'" << ss.str() << "'" << std::endl
                       << "on URL '"<< url << "'" << std::endl;

                    throw std::runtime_error(ss.str());
                }
#endif
                return ss.str();
            } catch(std::exception ex) {
                throw;
            }
        }
    }
};


