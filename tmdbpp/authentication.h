#ifndef TMDBPP_AUTHENTICATION_H
#define TMDBPP_AUTHENTICATION_H

#include <tmdbpp/apiagent.h>
#include <tmdbpp/util.h>


namespace tmdbpp {

    class SessionToken : public JSonMapper {
    private:
        typedef JSonMapper super;
    public:
        SessionToken() : super() {
        }

        SessionToken(const std::string & s) : super( s ) {
        }

        SessionToken(std::istream & is) : super(is) {
        }

    private:
    };

    class RequestToken : public JSonMapper {
    private:
        typedef JSonMapper super;
    public:
        RequestToken() : super() {
        }
        RequestToken(const std::string & s) : super(s) {
        }
        RequestToken(std::istream & is) : super(is) {
        }

        timestamp_t expires_at() const {
            return ptree().get<timestamp_t>("expires_at",timestamp_t());
        }

        std::string request_token() const {
            return ptree().get<std::string>("request_token","");
        }

    private:
    };

    /** @short Bundles all API calls we need for SessionId handling and
     * user authentication/authorization.
     */

    class Authentication : public ApiAgent {
    private:
        typedef ApiAgent super;
    public:
        Authentication(Api & api);
        SessionToken getSessionToken(const RequestToken & rt);
        SessionToken getSessionToken(const std::string &usr,const std::string &pwd);
        RequestToken getRequestToken();
        RequestToken validateRequestToken(const RequestToken & ,const std::string & user, const std::string & pwd);
    private:
    };
}
#endif // AUTHENTICATION_H
