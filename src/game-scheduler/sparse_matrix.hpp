#ifndef TRX_SPARSE_MATRIX_HPP
#define TRX_SPARSE_MATRIX_HPP

#include <map>
#include <set>
#include <tuple>
#include <vector>
#include <utility>
#include <algorithm>

namespace trx {

    template<class I, class J, class V>
    struct sparse_matrix {

        typedef V value_type;

        typedef I row_index_type;

        typedef J col_index_type;

        typedef std::map<J, V> row_type;

        typedef std::map<I, row_type> matrix_type;

    private:

        matrix_type data;

        row_type & ensure_row(row_index_type const & i) {
            if (data.find(i) == data.end())
                data.emplace(i, row_type());
            return data.at(i);
        }

        value_type & ensure_value(row_type & row, col_index_type const & j) {
            if (row.find(j) == row.end())
                row.emplace(j, value_type());
            return row.at(j);
        }

    public:

        sparse_matrix()
            : data() {
        }

        value_type & operator () (row_index_type const & i, col_index_type const & j) {
            auto & row = ensure_row(i);
            auto & value = ensure_value(row, j);
            return value;
        }

        value_type operator () (row_index_type const & i, col_index_type const & j) const {
            auto & row = ensure_row(i);
            auto & value = ensure_value(row, j);
            return value_type(value);
        }
    };

    template<class I, class J, class V>
    struct tuple_sparse_matrix {

        typedef I                                   first_key_type;
        typedef J                                   second_key_type;
        typedef V                                   mapped_type;
        typedef std::tuple<I, J>                    key_type;
        typedef std::pair<key_type, mapped_type>    value_type;
        typedef std::map<key_type, mapped_type>     matrix_type;

    private:

        matrix_type data;

        mapped_type & ensure_value(I const & i, J const & j) {
            auto key = std::make_tuple(i, j);
            if (data.find(key) == data.end())
                data.emplace(key, mapped_type());
            return data[key];
        }

    public:

        tuple_sparse_matrix()
            : data() {
        }

        mapped_type & operator () (I const & i, J const & j) {
            auto & value = ensure_value(i, j);
            return value;
        }

        mapped_type operator () (I const & i, J const & j) const {
            auto & value = ensure_value(i, j);
            return mapped_type(value);
        }

        size_t size() const {
            return data.size();
        }

        void clear() {
            data.clear();
        }

        std::set<first_key_type> first_keys() const {
            std::set<first_key_type> keys;
            std::for_each(data.begin(), data.end(), [&keys](value_type d) {
                auto x = std::get<0>(d.first);
                if (keys.find(x) == keys.end())
                    keys.emplace(x);
            });
            return keys;
        }

        std::set<second_key_type> second_keys() const {
            std::set<second_key_type> keys;
            std::for_each(data.begin(), data.end(), [&keys](value_type d) {
                auto x = std::get<1>(d.first);
                if (keys.find(x) == keys.end())
                    keys.emplace(x);
            });
            return keys;
        }

        typedef std::vector<std::pair<second_key_type, mapped_type>> first_key_values_vector;

        typedef struct {
            second_key_type second;
            mapped_type value;
        } first_key_value_type;

        typedef struct {
            first_key_type first;
            mapped_type value;
        } second_key_value_type;

        typedef std::vector<first_key_value_type> first_key_value_vector;
        typedef std::vector<second_key_value_type> second_key_value_vector;

        first_key_value_vector get_values_for(first_key_type key) const {
            first_key_value_vector values;
            std::for_each(data.begin(), data.end(), [&key, &values](value_type d) {
                if (std::get<0>(d.first) == key)
                    values.push_back(first_key_value_type{ std::get<1>(d.first), d.second });
            });
            return values;
        }

        second_key_value_vector get_values_for(second_key_type key) const {
            second_key_value_vector values;
            std::for_each(data.begin(), data.end(), [&key, &values](value_type d) {
                if (std::get<1>(d.first) == key)
                    values.push_back(second_key_value_type{ std::get<0>(d.first), d.second });
            });
            return values;
        }

        typedef std::vector<mapped_type> mapped_type_vector;

        mapped_type_vector get_values() {
            mapped_type_vector values;
            for (auto& x : data)
                values.push_back(x.second);
            return values;
        }
    };
}

#endif //TRX_SPARSE_MATRIX_HPP
