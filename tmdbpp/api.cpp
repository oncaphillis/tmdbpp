#include <tmdbpp/api.h>

namespace tmdbpp {
    const std::string Api::BaseUrl       = "https://api.themoviedb.org/3";
    const std::string Api::MethodConfig  = "/configuration";
    const std::string Api::MethodSearch  = "/search";
    const std::string Api::MethodMovie   = "/movie";

    const std::string Api::ObjectMovie   = "/movie";
    const std::string Api::ObjectCompany = "/company";
};


