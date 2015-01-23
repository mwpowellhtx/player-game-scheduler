#ifndef SCHED_CANDIDATE_ANALYSIS_H
#define SCHED_CANDIDATE_ANALYSIS_H

#include "candidate.h"
#include "data_context.h"

#include "statistics.h"

#include "sparse_matrix.hpp"

namespace sched {

    struct candidate_analysis {

        typedef trx::tuple_sparse_matrix<game*, player*, candidate*> candidate_map_type;

        typedef std::map<game*, trx::statistics> game_stats_map_type;

        candidate_map_type candidates;

        game_stats_map_type game_stats;

        struct overall_t {

            trx::statistics sum;
            trx::statistics min;
            trx::statistics max;
            trx::statistics mean;
            trx::statistics variance;
            trx::statistics stdev;

            overall_t();

            overall_t(overall_t const & other);

            bool try_analyze(std::vector<trx::statistics> const & stats);

        } overall;

        candidate_analysis();

        candidate_analysis(candidate_analysis const & other);

        virtual ~candidate_analysis();

        bool try_analyze(data_context & dc);
    };
}

#endif //SCHED_CANDIDATE_ANALYSIS_H
