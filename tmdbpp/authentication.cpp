#include <tmdbpp/authentication.h>
#include <tmdbpp/api.h>

namespace tmdbpp {
    Authentication::Authentication(Api & api) : super(api)  {
    }

    SessionToken Authentication::getSessionToken(const RequestToken & rt) {
        SessionToken st;
        std::string url = Api::BaseUrl+Api::MethodAuthentication+Api::ObjectSession+Api::OptionNew+"?"+
                UrlArg("request_token",rt.request_token())+
                "&" + UrlArg("api_key",api().key());
        std::cerr << " --> " << url << std::endl;
        return fetch(url,st);
    }

    RequestToken Authentication::getRequestToken() {
        RequestToken rt;
        std::string url = Api::BaseUrl+Api::MethodAuthentication+Api::ObjectToken+Api::OptionNew+"?"+
                UrlArg("api_key",api().key());
        return fetch(url,rt);
    }

    RequestToken Authentication::validateRequestToken(const RequestToken & rti,const std::string & user, const std::string & pwd) {
        RequestToken rto;
        UrlArg $("username",user);
        std::string url = Api::BaseUrl+Api::MethodAuthentication+Api::ObjectToken+Api::OptionValidateWithLogin + "?" +
                UrlArg("api_key",api().key()) +
                "&" + UrlArg("username",user) +
                "&" + UrlArg("password",pwd) +
                "&" + UrlArg("request_token",rti.request_token());

        return fetch(url,rto);
    }

    SessionToken Authentication::getSessionToken(const std::string & user, const std::string & pwd) {
        RequestToken rti = getRequestToken();
        if(rti) {
            rti =  validateRequestToken(rti,user,pwd);
            if(rti) {
                return getSessionToken(rti);
            }
        }
        return SessionToken();
    }
}
