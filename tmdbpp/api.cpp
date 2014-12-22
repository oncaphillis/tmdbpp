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

};


