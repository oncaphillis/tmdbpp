#ifndef TMDBPP_GET_H
#define TMDBPP_GET_H

#include <tmdbpp/apiagent.h>
#include <tmdbpp/movie.h>
#include <tmdbpp/collection.h>
#include <tmdbpp/company.h>
#include <tmdbpp/keyword.h>

namespace tmdbpp {

    class Get : public ApiAgent {
    private:
        typedef ApiAgent super;
    public:
        Get(Api & api ) : super(api) {
        }
        Movie movie(int id);
        Collection collection(int id);
        Company company(int id);
        Keyword keyword(int id);

    private:
    };
}
#endif // GET_H
