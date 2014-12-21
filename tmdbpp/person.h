#ifndef TMDBPP_PERSON_H
#define TMDBPP_PERSON_H

#include <tmdbpp/util.h>
#include <tmdbpp/movie.h>
#include <string>

namespace tmdbpp {
    class PersonInfo : public IdHolder {
    private:
        typedef IdHolder super;
    public:
        PersonInfo() : super() {
        }
        PersonInfo(const boost::property_tree::ptree & p) : super(p) {
        }
        PersonInfo(std::istream & is ) : super(is) {
        }
        PersonInfo(const PersonInfo & p) : super(p) {
        }
        std::string name() const {
            return ptree().get<std::string>("name","");
        }
        std::string profile_path() const{
            return ptree().get<std::string>("profile_path","");
        }
     private:
        friend
        void swap(PersonInfo & p0,PersonInfo & p1) {
            using std::swap;
            swap(static_cast<super &>(p0),static_cast<super&>(p1));
        }
    };

    class PersonSummary : public PersonInfo {
    private:
        typedef PersonInfo super;
    public:

        PersonSummary() : super() {
        }

        PersonSummary(const boost::property_tree::ptree & p) : super(p) {
        }

        PersonSummary(std::istream & is ) : super(is) {
        }

        PersonSummary(const PersonSummary & p) : super(p) {

        }

        std::string biography() {
            return ptree().get<std::string>("biograpghy","");
        }
        bool adult() const {
            return ptree().get<bool>("adult",false);
        }

        std::string also_known_as()  const {
            return ptree().get<std::string>("also_known_as","");
        }

        std::string birthday()  const {
            return ptree().get<std::string>("birthday","");
        }

        std::string deathday()  const {
            return ptree().get<std::string>("deathday","");
        }

        std::string homepage()  const  {
            return ptree().get<std::string>("homepage","");
        }

        std::string imdb_id()  const {
            return ptree().get<std::string>("imdb_id","");
        }

        std::string place_of_birth() const {
            return ptree().get<std::string>("place_of_birth","");
        }

        float popularity()  const  {
            return ptree().get<float>("popularity",0.0);
        }
    private:
        friend
        void swap(PersonSummary & p0,PersonSummary & p1) {
            using std::swap;
            swap(static_cast<super &>(p0),static_cast<super&>(p1));
        }

    };

    class Person : public PersonSummary {
    private:
        typedef PersonSummary super;
    public:
        Person() : super() {
        }

        Person(const boost::property_tree::ptree & p) : super(p) {
        }

        Person(std::istream & is ) : super(is) {
        }

        Person(const Person & p): super(p) {
        }

        Person(Person && p): super(p) {
            swap(*this,p);
        }

        Person & operator=(Person p) {
            swap(*this,p);
            return *this;
        }
    private:
        friend
        void swap(Person & p0,Person & p1) {
            using std::swap;
            swap(static_cast<super &>(p0),static_cast<super&>(p1));
        }
    };
}
#endif // TMDBPP_PERSON_H
