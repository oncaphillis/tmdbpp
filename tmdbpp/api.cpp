#include <tmdbpp/api.h>

namespace tmdbpp {
    const std::string Api::BaseUrl          = "https://api.themoviedb.org/3";
    const std::string Api::MethodConfig     = "/configuration";
    const std::string Api::MethodSearch     = "/search";
    const std::string Api::MethodMovie      = "/movie";
    const std::string Api::MethodCollection = "/collection";
    const std::string Api::MethodCompany    = "/company";
    const std::string Api::MethodKeyword    = "/keyword";
    const std::string Api::MethodTv                = "/tv";
    const std::string Api::MethodPerson            = "/person";
    const std::string Api::MethodGenre             = "/genre";
    const std::string Api::MethodAuthentication    = "/authentication";

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
    const std::string Api::ObjectSession           = "/session";
    const std::string Api::ObjectToken             = "/token";

    const std::string Api::OptionTv                = "/tv";
    const std::string Api::OptionNew               = "/new";
    const std::string Api::OptionValidateWithLogin = "/validate_with_login";


    /** @short Most basic fetch() method for an ApiAgent. Retrieve
        an URL addressed data as a string.
    */

    std::string Api::fetch(const std::string & url) {
        int retry = 0;
        std::string content;
        while (retry++ < 3) {
            try {
                _status = ErrorStatus();
                int code;
#ifdef _WIN32
                WGet::Result r = WGet::instance().get(url);
                code = r.code;
                content = r.content;
#else
                std::stringstream ss;
                curlpp::options::Url myUrl(url);
                curlpp::Easy myRequest;

                myRequest.setOpt(myUrl);
                myRequest.setOpt(curlpp::options::WriteStream(&ss));
                myRequest.setOpt(curlpp::options::FailOnError(false));

                myRequest.perform();

                curlpp::InfoGetter::get(myRequest,CURLINFO_RESPONSE_CODE,code);
                content = ss.str();
#endif
                if (code != 200) {

                    // Gateway timeout.. wait and try again max 3 times
                    if (code == 504) {
#ifdef _WIN32
                        ::Sleep(retry * 1000);
#else
                        ::sleep(retry);
#endif
                        continue;
                    }

                    if (!content.empty()) {
                        _status = ErrorStatus(content);
                    }

                    if (code == 404 && !content.empty() && _status.status_code() == Api::StatusCode::StatusInvalidId) {
                        return "";
                    }
                    if (code == 401 && !content.empty() && _status.status_code() == Api::StatusCode::StatusDenied) {
                        return "";
                    }
                    if (code == 401 && !content.empty() && _status.status_code() == Api::StatusCode::StatusInvalidLogin) {
                        return "";
                    }

                    std::stringstream ss;

                    ss << "code #" << code << " status_message '" << _status.status_message() << "' "
                        << " status code='" << _status.status_code() << "'"
                        << " on URL '" << url << "'" << std::endl;

                    throw std::runtime_error(ss.str());
                }
                return content;
            }
            catch (std::exception ex) {
                throw;
            }
        }
    }
};


