#include "statistics.h"

#include "../../tools/cpplinq/cpplinq.hpp"

#include <cmath>

//http://stackoverflow.com/questions/7988486/how-do-you-calculate-the-variance-median-and-standard-deviation-in-c-or-java
namespace trx {

    double mean;

    double variance;

    double stdev;

    statistics::statistics()
        : sum(0),
        min(0),
        max(0),
        mean(0),
        variance(0),
        stdev(0) {
    }

    statistics::statistics(statistics const & other)
        : sum(other.sum),
        min(other.min),
        max(other.max),
        mean(other.mean),
        variance(other.variance),
        stdev(other.stdev) {
    }

    double calculate_sum(std::vector<double> const & values) {
        return cpplinq::from(values) >> cpplinq::sum();
    }

    double calculate_min(std::vector<double> const & values) {
        return cpplinq::from(values) >> cpplinq::min();
    }

    double calculate_max(std::vector<double> const & values) {
        return cpplinq::from(values) >> cpplinq::max();
    }

    double calculate_mean(std::vector<double> const & values) {
        return cpplinq::from(values) >> cpplinq::avg();
    }

    double calculate_variance(double mean, std::vector<double> const & values) {
        return cpplinq::from(values)
            >> cpplinq::select([mean](double x) { return (mean - x)*(mean - x); })
            >> cpplinq::avg();
    }

    double calculate_stdev(double variance) {
        return std::sqrt(variance);
    }

    bool statistics::try_calculate(std::vector<double> const & values) {
        sum = calculate_sum(values);
        min = calculate_min(values);
        max = calculate_max(values);
        mean = calculate_mean(values);
        variance = calculate_variance(mean, values);
        stdev = calculate_stdev(variance);
        return true;
    }
}
