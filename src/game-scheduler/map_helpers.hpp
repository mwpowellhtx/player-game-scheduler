#ifndef TRX_MAP_HELPERS_HPP
#define TRX_MAP_HELPERS_HPP

template<class map_type>
class key_iterator : public map_type::iterator
{
public:

    typedef typename map_type::iterator iterator;
    typedef typename map_type::value_type::first_type key_type;

    key_iterator(iterator const & other)
        : map_type::iterator(other) {
    };

    key_type & operator *() {
        return map_type::iterator::operator*().first;
    }
};

// helpers to create iterators easier:
template<class map_type>
key_iterator<map_type> key_begin(map_type& m) {
    return key_iterator<map_type>(m.begin());
}

template<class map_type>
key_iterator<map_type> key_end(map_type& m) {
    return key_iterator<map_type>(m.end());
}

#endif //TRX_MAP_HELPERS_HPP
