#ifndef TMDBPP_CREDITS_H
#define TMDBPP_CREDITS_H

#include <tmdbpp/util.h>
#include <tmdbpp/movie.h>
#include <string>
namespace tmdbpp {

    class Credit : public MediaInfo {
    private:
        typedef MediaInfo super;
    public:
        Credit() : super() {
        }
        Credit(std::istream &is) : super(is) {
            setup();
        }
        Credit(const boost::property_tree::ptree & pt) : super(pt) {
            setup();
        }
        Credit(const Credit & c) : super(c) {
            setup();
        }

        Credit(Credit && c) {
            swap(*this,c);
        }

        Credit & operator=(Credit c) {
            swap(*this,c);
            return *this;
        }

        std::string credit_id() const {
            return ptree().get<std::string>("credit_id","");
        }
    private:
        void setup() {
        }

        friend
        void swap(Credit & c0,Credit &c1) {
            using std::swap;
            swap(static_cast<super &>(c0),static_cast<super&>(c1));
        }
    };

    class MediaCredit : public Credit {
    private:
        typedef Credit super;
    public:
        MediaCredit() : super() {
        }
        MediaCredit(std::istream &is) : super(is) {
        }
        MediaCredit(const boost::property_tree::ptree & pt) : super(pt) {
        }
        MediaCredit(const MediaCredit & c) : super(c) {
        }

        MediaCredit(MediaCredit && c) {
            swap(*this,c);
        }

        MediaCredit & operator=(MediaCredit c) {
            swap(*this,c);
            return *this;
        }

        int media_id() const {
            return super::id();
        }
    private:
        friend
        void swap(MediaCredit & c0,MediaCredit & c1) {
            using std::swap;
            swap(static_cast<super&>(c0),static_cast<super &>(c1));
        }
    };

    class PersonalCredit : public Credit {
    private:
        typedef Credit super;
    public:
        PersonalCredit() : super() {
        }
        PersonalCredit(std::istream &is) : super(is) {
        }
        PersonalCredit(const boost::property_tree::ptree & pt) : super(pt) {
        }
        PersonalCredit(const PersonalCredit & c) : super(c) {
        }
        PersonalCredit(PersonalCredit && c) {
            swap(*this,c);
        }
        PersonalCredit & operator=(PersonalCredit c) {
            swap(*this,c);
            return *this;
        }
        int person_id() const {
            return super::id();
        }
        std::string name() const {
            return super::ptree().template get<std::string>("name","");
        }
    private:
        friend
        void swap(PersonalCredit & c0,PersonalCredit & c1) {
            using std::swap;
            swap(static_cast<super&>(c0),static_cast<super &>(c1));
        }
    };

    template<class CREDIT>
    class CastMember : public CREDIT {
    private:
        typedef CREDIT super;
    public:
        CastMember() : super() {
        }
        CastMember(std::istream &is) : super(is) {
        }
        CastMember(const boost::property_tree::ptree & pt) : super(pt) {
        }
        CastMember(const CastMember & c) : super(c) {
        }
        CastMember(CastMember && c) {
            swap(*this,c);
        }
        CastMember & operator=(CastMember c) {
            swap(*this,c);
            return *this;
        }
        std::string character() const {
            return super::ptree().template get<std::string>("character","");
        }

    private:
        friend
        void swap(CastMember & c0,CastMember & c1) {
            using std::swap;
            swap(static_cast<super&>(c0),static_cast<super &>(c1));
        }
    };

    template<class CREDIT>
    class CrewMember : public CREDIT {
    private:
        typedef CREDIT super;
    public:
        CrewMember() : super() {
        }
        CrewMember(std::istream &is) : super(is) {
        }
        CrewMember(const boost::property_tree::ptree & pt) : super(pt) {
        }
        CrewMember(const CrewMember & c) : super(c) {
        }
        CrewMember(CrewMember && c) {
            swap(*this,c);
        }
        CrewMember & operator=(CrewMember c) {
            swap(*this,c);
            return *this;
        }

        std::string department() const {
           return super::ptree().template get<std::string>("department","");
        }

        std::string job() const {
            return super::ptree().template get<std::string>("job","");
        }

    private:
        friend
        void swap(CrewMember & c0,CrewMember & c1) {
            using std::swap;
            swap(static_cast<super&>(c0),static_cast<super &>(c1));
        }
    };

    template<class CASTM,class CREWM>
    class BasicCredits : public JSonMapper {
    private:
        typedef JSonMapper super;
    public:
        typedef CREWM crew_member_t;
        typedef CASTM cast_member_t;

        BasicCredits() : super() {
        }
        BasicCredits(std::istream &is) : super(is) {
            setup();
        }
        BasicCredits(const boost::property_tree::ptree & pt) : super(pt) {
            setup();
        }
        BasicCredits(const BasicCredits & c) : super(c) {
            setup();
        }

        BasicCredits(BasicCredits && c) {
            swap(*this,c);
        }

        BasicCredits & operator=(BasicCredits c) {
            swap(*this,c);
            return *this;
        }

        const std::list<cast_member_t> & as_cast() const {
            return _as_cast;
        }

        const std::list<crew_member_t> & as_crew() const {
            return _as_crew;
        }

    private:
        void setup() {
            for(auto a : ptree().get_child("crew") ) {
                _as_crew.push_back(a.second);
            }
            for(auto a : ptree().get_child("cast") ) {
                _as_cast.push_back(a.second);
            }
        }

        friend
        void swap(BasicCredits & c0,BasicCredits &c1) {
            using std::swap;
            swap(static_cast<super &>(c0),static_cast<super&>(c1));
            c0._as_cast.swap(c1._as_cast);
            c0._as_crew.swap(c1._as_crew);
        }

        std::list<cast_member_t> _as_cast;
        std::list<crew_member_t> _as_crew;
    };

    typedef BasicCredits< CastMember<MediaCredit> , CrewMember<MediaCredit> >       PersonalCredits;
    typedef BasicCredits< CastMember<PersonalCredit> , CrewMember<PersonalCredit> > MediaCredits;

}
#endif // CREDITS_H
