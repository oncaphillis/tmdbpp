#ifndef TMDBPP_TVSERIES_H
#define TMDBPP_TVSERIES_H

#include <tmdbpp/util.h>

namespace tmdbpp {

    class TvSeriesSummary : public MediaSummary {
    private:
        typedef MediaSummary super;
    public:
 
        TvSeriesSummary() : super() {
        }

        TvSeriesSummary(const boost::property_tree::ptree & pt ) : super(pt) {
            setup();
        }

        TvSeriesSummary( TvSeriesSummary && t ) : super(t) {
            swap(*this,t);
        }

        TvSeriesSummary(const TvSeriesSummary &t) : super(t) {
            setup();
        }

        TvSeriesSummary(std::istream & is) : super(is) {
            setup();
        }

        TvSeriesSummary & operator=(TvSeriesSummary t) {
            swap(*this,t);
            return *this;
        }

        std::string original_name() const {
            return ptree().get<std::string>("original_name","");
        }

        std::string name() const  {
            return ptree().get<std::string>("name","");
        }

        float vote_average() const {
            return ptree().get<float>("vote_average",0.0);
        }

        int vote_count() const {
            return ptree().get<int>("vote_count",0);
        }

        float popularity() const {
            return ptree().get<float>("popularity",0.0);
        }
        std::string first_air_date() const {
            return ptree().get<std::string>("first_air_date","");
        }

        const std::list<std::string> & origin_countries() const {
            return _origin_countries;
        }

    private:
        friend
        void swap(TvSeriesSummary & t0,TvSeriesSummary & t1) {
            using std::swap;
            swap(static_cast<super &>(t0),static_cast<super &>(t1));
            t0._origin_countries.swap(t1._origin_countries);
        }

        void setup() {
            for(const auto n : ptree().get_child("origin_country")) {
                _origin_countries.push_back(n.second.data());
            }
        }

        std::list<std::string>    _origin_countries;

    };

    class TvSeries : public MediaSummary {
    private:
        typedef MediaSummary super;
    public:

        TvSeries() : super() {
        }

        TvSeries(const boost::property_tree::ptree & pt ) : super(pt) {
            setup();
        }

        TvSeries( TvSeries && t ) : super(t) {
            swap(*this,t);
        }

        TvSeries(const TvSeries &t) : super(t) {
            setup();
        }

        TvSeries(std::istream & is) : super(is) {
            setup();
        }

        TvSeries & operator=(TvSeries t) {
            swap(*this,t);
            return *this;
        }

        std::string homepage() const  {
            return ptree().get<std::string>("homepage","");
        }


        std::string overview() const  {
            return ptree().get<std::string>("overview","");
        }

        std::string status() const {
            return ptree().get<std::string>("status","");
        }



        std::string last_air_date() const {
            return ptree().get<std::string>("last_air_date","");
        }

        int number_of_episodes() const {
            return ptree().get<int>("number_of_episodes",0);
        }

        int number_of_seasons() const {
            return ptree().get<int>("number_of_seasons",0);
        }


        bool in_production() const {
            return ptree().get<bool>("on_production",false);
        }

        const std::list<Genre> & genres() const {
            return _genres;
        }

        const std::list<Network> & networks() const {
            return _networks;
        }

        const std::list<PersonSummary> & created_bys() const {
            return _created_bys;
        }

        const std::list<SeasonSummary> & seasons() const {
            return _seasons;
        }

        const std::list<std::string> & languages() const {
            return _languages;
        }
        const std::list<CompanySummary> & production_companies() const {
            return _production_companies;
        }


    private:
        friend
        void swap(TvSeries & t0,TvSeries & t1) {
            using std::swap;
            swap(static_cast<super&>(t0),static_cast<super&>(t1));

            t0._genres.swap(t1._genres);
            t0._networks.swap(t1._networks);
            t0._created_bys.swap(t1._created_bys);
            t0._seasons.swap(t1._seasons);
            t0._languages.swap(t1._languages);
            t0._production_companies.swap(t1._production_companies);
        }

        void setup() {

            for(const auto n : ptree().get_child("genres")) {
                _genres.push_back(Genre(n.second));
            }

            for(const auto n : ptree().get_child("networks")) {
                _networks.push_back(Network(n.second));
            }

            for(const auto n : ptree().get_child("created_by")) {
                _created_bys.push_back(n.second);
            }

            for(const auto n : ptree().get_child("seasons")) {
                _seasons.push_back(n.second);
            }

            for(const auto n : ptree().get_child("languages")) {
                _languages.push_back(n.second.data());
            }

            for(const auto n : ptree().get_child("production_companies")) {
                _production_companies.push_back(n.second);
            }

        }

        std::list<Genre>          _genres;
        std::list<Network>        _networks;
        std::list<PersonSummary>  _created_bys;
        std::list<SeasonSummary>  _seasons;
        std::list<std::string>    _languages;
        std::list<CompanySummary> _production_companies;
    };
}

#endif // TVSERIES_H
