#ifndef TRX_STATISTICS_H
#define TRX_STATISTICS_H

#include <vector>

namespace trx {

    struct statistics {

        double sum;

        double min;

        double max;

        double mean;

        double variance;

        double stdev;

        //TODO: median?

        statistics();

        statistics(statistics const & other);

    public:

        bool try_calculate(std::vector<double> const & values = {});
    };
}

#endif //TRX_STATISTICS_H
