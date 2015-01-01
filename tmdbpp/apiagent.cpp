#include <tmdbpp/apiagent.h>
#include <tmdbpp/api.h>

namespace tmdbpp {

/** @short Most basic fetch() method for an ApiAgent. Retrieve
    an URL addressed data as a string.
 */

    std::string ApiAgent::fetch(const std::string & url) {
        try {
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
                if(c==404 && !ss.str().empty() && ErrorStatus(ss.str()).status_code() == Api::StatusCode::StatusInvalidId) {
                    return "";
                }
                std::stringstream ss;
                ss << "code #" << c << "'" << ss.str() << "'";
                throw std::runtime_error(ss.str());
            }
    #endif
            return ss.str();
        } catch(std::exception ex) {
            throw;
        }
    }
}
