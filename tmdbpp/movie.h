#ifndef TMDBPP_MOVIE_H
#define TMDBPP_MOVIE_H

#include <tmdbpp/util.h>
#include <list>

namespace tmdbpp {
    class MediaReleaseSummary : public MediaSummary {
    private:
        typedef MediaSummary super;
    public:
        MediaReleaseSummary() : super() {
        }
        MediaReleaseSummary(const boost::property_tree::ptree & p) : super(p) {
        }        
        MediaReleaseSummary(std::istream & is ) : super(is) {
        }
    private:
        friend
        void swap(MediaReleaseSummary & m0, MediaReleaseSummary & m1) {
            using std::swap;
            swap(static_cast<super&>(m0), static_cast<super&>(m1));
        }
    };

    class MediaInfo : public MediaReleaseSummary {
    private:
        typedef MediaReleaseSummary super;
    public:
        
        MediaInfo() : super() {
        }
        MediaInfo(const boost::property_tree::ptree & p) : super(p) {
        }        
        MediaInfo(std::istream & is ) : super(is) {
        }
        bool adult() {
            return ptree().get<bool>("adult",false);
        }
        std::string original_title() const {
            return ptree().get<std::string>("original_title","");
        }
        float popularity() const {
            return ptree().get<float>("popularity",0.0);
        }
        float vote_average() const {
            return ptree().get<float>("vote_average",0.0);
        }
        int vote_count() const {
            return ptree().get<int>("vote_count",0);
        }
        bool video() const {
            return ptree().get<bool>("video",false);
        }
    private:
        friend
        void swap(MediaInfo & m0, MediaInfo & m1) {
            using std::swap;
            swap(static_cast<super&>(m0), static_cast<super&>(m1));
        }
    };

    class Media : public MediaInfo {

    private:

        typedef MediaInfo super;

    public:

        Media() : super() {
        }

        Media(const boost::property_tree::ptree & p) : super(p) {
        }        

        Media(std::istream & is ) : super(is) {
        }

        Media(Media && m) : super(m) {
            swap(*this,m);
        }

        Media(const Media & m) : super(m) {
            setup();
        }


        std::string belongs_to_collection() const {
            return ptree().get<std::string>("belongs_to_collection","");
        }

        long budget() const {
            return ptree().get<long>("budget",0L);
        }

        std::string homepage() const {
            return ptree().get<std::string>("homepage","");
        }

        std::string imdb_id() const {
            return ptree().get<std::string>("imdb_id","");
        }


        std::string overview() const {
            return ptree().get<std::string>("overview","");
        }


        long revenue() const {
            return ptree().get<long>("revenue",0L);
        }

        int runtime() const {
            return ptree().get<int>("runtime",0);
        }

        std::string status() const {
            return ptree().get<std::string>("status","");
        }

        std::string tagline() const {
            return ptree().get<std::string>("tagline","");
        }


        long vote_count() const {
            return ptree().get<long>("vote_count",0L);
        }

        const std::list<Genre> & genres() const {
            return _genres;
        }

        const std::list<Language> & spoken_languages() const {
            return _spoken_languages;
        }

        const std::list<CompanySummary> & production_companies() const {
            return _production_companies;
        }

        const std::list<Country> & production_countries() const {
            return _production_countries;
        }

        Media & operator=(Media m) {
            swap(*this,m);
            return *this;
        }

    private:

        friend void swap(Media & m0,Media & m1) {
            using std::swap;

            swap(static_cast<super &>(m0),static_cast<super &>(m1));

            m0._genres.swap(m1._genres);
            m0._production_companies.swap(m1._production_companies);
            m0._production_countries.swap(m1._production_countries);
            m0._spoken_languages.swap(m1._spoken_languages);
        }

        void setup() {
            for(auto n : ptree().get_child("genres")) {
                _genres.push_back(Genre(n.second));
            }

            for(auto n : ptree().get_child("spoken_languages")) {
                _spoken_languages.push_back(Language(n.second));
            }

            for(auto n : ptree().get_child("production_countries")) {
                _production_countries.push_back(Country(n.second));
            }

            for(auto n : ptree().get_child("production_companies")) {
                _production_companies.push_back(CompanySummary(n.second));
            }
        }
        std::list<Genre>           _genres;
        std::list<Country>         _production_countries;
        std::list<Language>        _spoken_languages;
        std::list<CompanySummary>  _production_companies;
    };

    typedef MediaSummary MovieSummary;
    typedef Media        Movie;

}

#endif // TMDBPP_MOVIE_H
