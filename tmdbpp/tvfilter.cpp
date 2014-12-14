#include <tmdbpp/api.h>
#include <tmdbpp/tvfilter.h>

namespace tmdbpp {
    TvSeriesCollection TvFilter::popular(const std::string & lang,int page) {
        TvSeriesCollection coll;
        std::stringstream ss;
        std::string url = Api::BaseUrl+Api::MethodTv+Api::ObjectPopular+"?"+
                 Arg("api_key",api().key());

        if(page>0)
            url+="&"+Arg("page",page);

        return fetch(url,coll);
    }

    TvSeriesCollection TvFilter::popular(int page) {
        return popular("",page);
    }

}
