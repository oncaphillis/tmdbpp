#ifndef NIFTY_INDEXHASH_H
#define NIFTY_INDEXHASH_H

#include <string>
#include <set>
#include <map>

namespace nifty {

    /** @short A Key,Value hash with the additional function of searching fir
        substrings.

        - One may add entries like _hash[key] = value
        - Delete via _hash.erase(key)
        - But also search for substrings like std::set<KEY> = _hash.find_sub("str")

    */

    template<class KEY,class C>
    class BasicIndexedMap {

    public:

        typedef KEY                      key_t;
        typedef std::basic_string<C>     string_t;
        typedef std::map<key_t,string_t> map_t;

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
            Comparator(const map_t & m) : _m(m) {
            }

            // Compare two substings
            bool operator()(const pos_t & a,const pos_t & b) {

                const string_t & as = _m.find(a.first)->second;
                const string_t & bs = _m.find(b.first)->second;

                typename string_t::size_type ai  = a.second == as.length() ? string_t::npos : a.second;
                typename string_t::size_type bi  = b.second == bs.length() ? string_t::npos : b.second;

                if( ai == string_t::npos || bi == string_t::npos )
                    return ai==bi ? a.first < b.first : ai<bi;

                for(;ai < as.length() && bi < bs.length(); ai++, bi++) {
                    if(as[ai] != bs[bi]) {
                        return as[ai]<bs[bi];
                    }
                }
                return as.length() - ai == bs.length() - bi ? a.first > b.first : as.length() - ai < bs.length() - bi  ;
            }

        private:
            const map_t & _m;
        };


    public:

        typedef std::set< pos_t, Comparator >  suffixidx_t;
        typedef typename map_t::const_iterator const_iterator;
        typedef typename map_t::size_type size_type;
        BasicIndexedMap() :
            _comparator(_map),
            _idx({},_comparator),
            _dirty(true) {
        }


        std::string operator[](int i) const {
            return _map.find(_vector[i].first)->second.substr(_vector[i].second);
        }

        void add(int idx,const std::string & s) {

            _map[idx] = s;

            for(int i=0;i<=s.length();i++) {
                _idx.insert(pos_t(idx,i));
            }
        }

        const suffixidx_t & getIdx() const {
            return _idx;
        }

        void dump(const std::string & name) {
            std::ofstream of(name);
            for(auto a : _map) {
                of << a.first << ":" << a.second << std::endl;
            }

            for(auto a : _idx) {
                of << a.first << ":" << a.second << std::endl;
            }
        }

        /** @short Binary search over the full suffix
            array.

            Either returns the first entry in the suffixidx_t
            that matches or the first one that goes after the given
            substring.

        */

        int search(const std::string & t) {
            return search(t,0,_vector.size()-1);
        }

        size_type erase(const key_t & k) {
            return _map.erase(k);
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

    private:

        std::string operator()(int i) const {
            return _map.find(_vector[i].first)->second;
        }

        /** @short Binary search over a current subset of the
            suffix array.
         */

        int search(const std::string & t,int i,int j) {
            // If something has been added in between
            // we have to transfer the std::set into our std::vector

            if(_dirty) {
                _vector.swap(std::vector<pos_t>(_idx.size()));
                std::copy(_idx.begin(),_idx.end(),_vector.begin());
            }

            if(t <= (*this)[i])
                return i;

            if(t > (*this)[j] )
                return j+1;

            int k = i+(j-i)/2;

            return t<=(*this)[k] ? search(t,1,k) : search(t,k+1,j);

        }

        map_t              _map;
        Comparator         _comparator;
        suffixidx_t        _idx;
        std::vector<pos_t> _vector;
        bool               _dirty;
    };
    typedef BasicIndexedMap<int,char> IndexedMap;
};


#if 0
int main(int argc,char ** argv) {
    IndexedHash ii;
    std::string s;
    int n=0;

    if(argc==2) {
        while(std::getline(std::cin,s)) {
            ii.add(n,s);
            std::cerr << n++ << "\r" << std::flush;
        }
        std::cerr << std::endl;
        int n=ii.search(argv[1]);

        std::cerr << ii[n] << "::" << ii(n) << std::endl;;
    }
    return 0;
}
#endif

#endif // NIFTY_INDEXEDHASH_H
