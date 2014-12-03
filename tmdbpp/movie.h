#ifndef TMDBPP_MOVIE_H
#define TMDBPP_MOVIE_H

#include <tmdbpp/util.h>
#include <list>

namespace tmdbpp {

    class MovieSummary : public IdHolder {
    private:
        typedef IdHolder super;
    public:
        using super::super;

        bool adult() {
            return ptree().get<bool>("adult",false);
        }

        std::string backdrop_path() const {
            return ptree().get<std::string>("backdrop_path","");
        }

        std::string original_title() const {
            return ptree().get<std::string>("original_title","");
        }

        std::string release_date() const {
            return ptree().get<std::string>("release_date","");
        }

        float popularity() const {
            return ptree().get<float>("popularity",0.0);
        }

        std::string poster_path() const {
            return ptree().get<std::string>("poster_path","");
        }

        std::string title() const {
            return ptree().get<std::string>("title","");
        }

        float vote_average() const {
            return ptree().get<float>("vote_average",0.0);
        }

    private:
    };

    class Movie : public MovieSummary {

    private:

        typedef MovieSummary super;

    public:

        Movie() : super() {
        }

        Movie(const boost::property_tree::ptree & pt) : super(pt) {
            setup();
        }

        Movie(Movie && m) : super(m) {
            swap(*this,m);
        }

        Movie(const Movie & m) : super(m) {
            setup();
        }

        Movie(std::istream & is) : super(is) {
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

        Movie & operator=(Movie m) {
            swap(*this,m);
            return *this;
        }

    private:

        friend void swap(Movie & m0,Movie & m1) {
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
        std::list<Genre>    _genres;
        std::list<Country>  _production_countries;
        std::list<Language> _spoken_languages;
        std::list<CompanySummary>  _production_companies;
    };
}

#endif // TMDBPP_MOVIE_H
