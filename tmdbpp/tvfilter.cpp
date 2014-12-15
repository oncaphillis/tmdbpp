#include <tmdbpp/api.h>
#include <tmdbpp/tvfilter.h>

namespace tmdbpp {
    TvSeriesCollection TvFilter::popular(const std::string & lang,int page) {
        TvSeriesCollection coll;
        std::stringstream ss;
        std::string url = Api::BaseUrl+Api::MethodTv+Api::ObjectPopular+"?"+
                 Arg("api_key",api().key());

        if(!lang.empty()) {
            url+="&"+Arg("language",lang);
        }

        if(page>0)
            url+="&"+Arg("page",page);

        return fetch(url,coll);
    }

    TvSeriesCollection TvFilter::popular(int page) {
        return popular("",page);
    }


    TvSeriesCollection TvFilter::airing_today(const std::string & lang,int page) {
        TvSeriesCollection coll;
        std::stringstream ss;
        std::string url = Api::BaseUrl+Api::MethodTv+Api::ObjectAiringToday+"?"+
                 Arg("api_key",api().key());

        if(!lang.empty()) {
            url+="&"+Arg("language",lang);
        }

        if(page>0)
            url+="&"+Arg("page",page);

        return fetch(url,coll);
    }

    TvSeriesCollection TvFilter::airing_today(int page) {
        return airing_today("",page);
    }


    TvSeriesCollection TvFilter::top_rated(const std::string & lang,int page) {
        TvSeriesCollection coll;
        std::stringstream ss;
        std::string url = Api::BaseUrl+Api::MethodTv+Api::ObjectTopRated+"?"+
                 Arg("api_key",api().key());

        if(!lang.empty()) {
            url+="&"+Arg("language",lang);
        }

        if(page>0)
            url+="&"+Arg("page",page);

        return fetch(url,coll);
    }

    TvSeriesCollection TvFilter::top_rated(int page) {
        return top_rated("",page);
    }

    TvSeriesCollection TvFilter::on_the_air(const std::string & lang,int page) {
        TvSeriesCollection coll;
        std::stringstream ss;
        std::string url = Api::BaseUrl+Api::MethodTv+Api::ObjectTopRated+"?"+
                 Arg("api_key",api().key());

        if(!lang.empty()) {
            url+="&"+Arg("language",lang);
        }

        if(page>0)
            url+="&"+Arg("page",page);

        return fetch(url,coll);
    }

    TvSeriesCollection TvFilter::on_the_air(int page) {
        return on_the_air("",page);
    }
}
