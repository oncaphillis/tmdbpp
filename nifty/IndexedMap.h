#ifndef NIFTY_INDEXHASH_H
#define NIFTY_INDEXHASH_H

#include <string>
#include <set>
#include <map>

namespace nifty {

    template<class CHR>
    class CaseLessLess {
    public:
        bool operator()(const CHR & a , const CHR & b) {
            return ::tolower(a) < ::tolower(b);
        }
    };

    /** @short A Key,Value hash with the additional function of searching for
        substrings.

        - One may add entries like _hash[key] = value
        - Delete via _map.erase(key)
        - But also search for substrings like std::set<KEY> = _hash.find_sub("str")

    */

    template<class KEY,class CHR,class CHR_CMP = std::less<CHR> >
    class BasicIndexedMap {

    public:

        typedef KEY                      key_t;
        typedef std::basic_string<CHR>   string_t;
        typedef std::map<key_t,string_t> map_t;
        typedef CHR_CMP                  chr_cmp_t;
        typedef CHR                      char_t;

        enum match_t {
            // Match anywhere
            FreeMatch   = 0x00,
            // Match has to be on the words start
            AnchorStart = 0x01,
            // Match has to be at the words end
            AnchorEnd   = 0x02,
            // Match has to be whole word
            WholeWord   = 0x03
        };
    private:

        // This is the way we address a single substring in our
        // full size suffix array

        typedef std::pair<key_t,typename string_t::size_type> pos_t;

        /** @short Custom comparator to implement a suffix array.

            Takes to positions and compares the two addressed
            substrings located in the associated map.

        */

        class Comparator {
        public:
            Comparator(const map_t & m,const chr_cmp_t & c=chr_cmp_t()) : _mp(&m), _cmp(c) {
            }

            // Compare two substings currently held in a std::map<std::string>
            //

            bool operator()(const pos_t & a,const pos_t & b) {

                const string_t & as = _mp->find(a.first)->second;
                const string_t & bs = _mp->find(b.first)->second;

                typename string_t::size_type ai  = a.second == as.length() ? string_t::npos : a.second;
                typename string_t::size_type bi  = b.second == bs.length() ? string_t::npos : b.second;

                // Handle the special "end of string" case
                //

                if( ai == string_t::npos || bi == string_t::npos )
                    return ai==bi ? a.first < b.first : ai==string_t::npos ? true : false;

                for(;ai < as.length() && bi < bs.length(); ai++, bi++) {
                    if( _cmp(as[ai],bs[bi]) || _cmp(bs[bi],as[ai]) ) {
                        return _cmp(as[ai],bs[bi]);
                    }
                }
                return as.length() - ai == bs.length() - bi ? a.first > b.first : as.length() - ai < bs.length() - bi  ;
            }

            // Comparator has to keep its state and is not
            // allowed to overwrite _mp
            Comparator & operator=(const Comparator &) {
                return *this;
            }

        private:
            Comparator();
            

            const map_t * _mp;
            chr_cmp_t     _cmp;
        };

    public:
        typedef std::set< pos_t, Comparator >  suffixidx_t;
        typedef typename map_t::const_iterator const_iterator;
        typedef typename map_t::size_type size_type;

        
        BasicIndexedMap(const chr_cmp_t & chr_cmp = chr_cmp_t()) :
            _comparator( _map, chr_cmp),
            _idx({},_comparator),
            _dirty(true),
            _chr_cmp(chr_cmp) {
        }

        const string_t  & operator[](const key_t & k) const {
            const_iterator i = _map.find(k);
            if(i==end()) {
                return _dummy;
            }
            return i->second;
        }

        void dump() {
            clean();
            std::cerr << " !! " << _vector.size() << std::endl;
            if(!_vector.empty())
                std::cerr << "'" << suffix(0) << "'...'" << suffix(_vector.size()/2) << "'...'" << suffix(_vector.size()-1) << "'" << std::endl;

        }

        std::set<key_t> find(const string_t & str,match_t match=FreeMatch)  {
            std::set<key_t> se;

            if(str.empty())
                return se;

            clean();


            int i=0;
            if( (i = search(str,0,_vector.size()-1))!=-1 ) {
                while(_eq(suffix(i).substr(0,str.size()),str)) {
                    key_t k               = _vector[i].first;
                    typename string_t::size_type s = _vector[i].second;

                    if( ( (match & AnchorStart) != 0 && s != 0 && !::isspace(_map[k][s-1]) ) ||
                          (match & AnchorEnd  ) != 0 && s + str.size() != _map[k].size() && ! ::isspace(_map[k][s+str.size()])) {
                        i++;
                        continue;
                    }

                    se.insert(k);
                    i++;
                }
            }
            return se;
        }

        void add(const key_t & k,const std::string & s) {
            erase(k);
            _map[k] = s;
            for(typename string_t::size_type i=0;i<=s.length();i++) {
                _idx.insert(pos_t(k,i));
            }
            _dirty = true;
        }

        size_type erase(const key_t & k) {
            int n=0;
            if(_map.find(k)!=_map.end()) {
                for(typename suffixidx_t::iterator i=_idx.begin();i!=_idx.end();) {
                    if(i->first == k) {
                        i = _idx.erase(i);
                        n++;
                    } else {
                        i++;
                    }
                }
                _dirty = true;
                return _map.erase(k);
            }
            return 0;
        }

        const_iterator begin() const {
            return _map.begin();
        }

        const_iterator end() const {
            return _map.end();
        }

        size_type size() const {
            return _map.size();
        }

        void swap(BasicIndexedMap & m) {
            using std::swap;
            _map.swap(m._map);
            _idx.swap(m._idx);
            _vector.swap(m._vector);
            bool b = _dirty;
            _dirty = m._dirty;
            m._dirty = b;
        }

    private:

        void clean() {
            if(_dirty) {
                std::vector<pos_t> v(_idx.size());
                _vector.swap(v);
                std::copy(_idx.begin(),_idx.end(),_vector.begin());
                _dirty=false;
            }
        }

        bool _lt(const char_t & a,const char_t &b) {
            return _chr_cmp(a,b);
        }

        bool _gt(const char_t & a,const char_t &b) {
            return _chr_cmp(b,a);
        }

        bool _eq(const char_t & a,const char_t &b) {
            return !_lt(a,b) && !_gt(a,b);
        }

        bool _lte(const char_t & a,const char_t &b) {
            return _lt(a,b) || _eq(a,b);
        }

        bool _gte(const char_t & a,const char_t &b) {
            return _gt(a,b) || _eq(a,b);
        }

        bool _eq(const string_t & a,const string_t &b) {
            typename string_t::const_iterator ai = a.begin();
            typename string_t::const_iterator bi = b.begin();
            while(ai!=a.end() && bi!=a.end() && _eq(*ai,*bi)) {
                ai++;
                bi++;
            }
            return ai==a.end() && bi==b.end() ? true : false;
        }

        bool _gt(const string_t & a,const string_t &b) {
            typename string_t::const_iterator ai = a.begin();
            typename string_t::const_iterator bi = b.begin();
            while(ai!=a.end() && bi!=a.end() && _eq(*ai,*bi)) {
                ai++;
                bi++;
            }
            return ai==a.end() && bi==b.end() ? false : ai==a.end() ? false : bi==b.end() ? true : _gt(*ai,*bi);
        }

        bool _gte(const string_t & a,const string_t &b) {
            typename string_t::const_iterator ai = a.begin();
            typename string_t::const_iterator bi = b.begin();
            while(ai!=a.end() && bi!=a.end() && _eq(*ai,*bi)) {
                ai++;
                bi++;
            }
            return ai==a.end() && bi==b.end() ? true : ai==a.end() ? false : bi==b.end() ? true : _gt(*ai,*bi);
        }

        bool _lte(const string_t & a,const string_t &b) {
            typename string_t::const_iterator ai = a.begin();
            typename string_t::const_iterator bi = b.begin();
            while(ai!=a.end() && bi!=a.end() && _eq(*ai,*bi)) {
                ai++;
                bi++;
            }
            return ai==a.end() && bi==b.end() ? true : ai==a.end() ? true : bi==b.end() ? false : _lt(*ai,*bi);
        }

        std::string suffix(int i) const {
            return _map.find(_vector[i].first)->second.substr(_vector[i].second);
        }

        /** @short Binary search over a current subset of the
            suffix array.
         */

        int search(const std::string & t,int i,int j) {

            if(_lte(t,suffix(i)))
                return i;

            if(_gt(t,suffix(j)))
                return j+1;

            int k = i+(j-i)/2;

            return _lte(t,suffix(k)) ? search(t,1,k) : search(t,k+1,j);

        }

        map_t              _map;
        Comparator         _comparator;
        suffixidx_t        _idx;
        std::vector<pos_t> _vector;
        bool               _dirty;
        string_t           _dummy;
        chr_cmp_t          _chr_cmp;
    };

    typedef BasicIndexedMap<int,char> IndexedMap;
};

#endif // NIFTY_INDEXEDHASH_H
