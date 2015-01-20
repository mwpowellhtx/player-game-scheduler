#ifndef TRX_ALGORITHMS_H
#define TRX_ALGORITHMS_H

#include <algorithm>

namespace trx {

    template <typename Iterator>
    inline bool next_combination(const Iterator first, Iterator k, const Iterator last)
    {
        /* Credits: Thomas Draper */
        //http://stackoverflow.com/a/5097100/8747
        //http://stackoverflow.com/questions/9501742/c-next-permutation-algorithm
        if ((first == last) || (first == k) || (last == k))
            return false;
        Iterator itr1 = first;
        Iterator itr2 = last;
        ++itr1;
        if (last == itr1)
            return false;
        itr1 = last;
        --itr1;
        itr1 = k;
        --itr2;
        while (first != itr1)
        {
            if (*--itr1 < *itr2)
            {
                Iterator j = k;
                while (!(*itr1 < *j)) ++j;
                std::iter_swap(itr1, j);
                ++itr1;
                ++j;
                itr2 = k;
                std::rotate(itr1, j, last);
                while (last != j)
                {
                    ++j;
                    ++itr2;
                }
                std::rotate(k, itr2, last);
                return true;
            }
        }
        std::rotate(first, k, last);
        return false;
    }
}

#endif //TRX_ALGORITHMS_H