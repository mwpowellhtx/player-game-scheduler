#ifndef TRX_FILTER_HPP
#define TRX_FILTER_HPP

namespace trx
{
    //TODO: not that there's anything wrong with this function, but I really want to get to some more detailed question/answer projections
    template <typename Container, typename Predicate>
    Container filter(Container const& container, Predicate pred) {
        Container filtered(container);
        filtered.erase(remove_if(filtered.begin(), filtered.end(), pred), filtered.end());
        return filtered;
    }
}

#endif //TRX_FILTER_HPP
