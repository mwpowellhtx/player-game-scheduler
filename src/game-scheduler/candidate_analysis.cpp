#include "candidate_analysis.h"

#include "../../tools/cpplinq/cpplinq.hpp"

namespace sched {

    candidate_analysis::overall_t::overall_t()
        : sum(), min(), max(),
        mean(), variance(), stdev() {
    }

    candidate_analysis::overall_t::overall_t(overall_t const & other)
        : sum(other.sum), min(other.min), max(other.max),
        mean(other.mean), variance(other.variance), stdev(other.stdev) {
    }

    bool candidate_analysis::overall_t::try_analyze(
        std::vector<trx::statistics> const & stats) {

        using namespace cpplinq;

        /* I'm not sure it makes sense to capture a 'sum of stdevs' for example,
        however, we'll just summarize the stats for later analysis, comparison,
        etc anyway. */

        auto sums = from(stats) >> select([](trx::statistics const & x) { return x.sum; }) >> to_vector();
        auto mins = from(stats) >> select([](trx::statistics const & x) { return x.min; }) >> to_vector();
        auto maxes = from(stats) >> select([](trx::statistics const & x) { return x.max; }) >> to_vector();
        auto means = from(stats) >> select([](trx::statistics const & x) { return x.mean; }) >> to_vector();
        auto variances = from(stats) >> select([](trx::statistics const & x) { return x.variance; }) >> to_vector();
        auto stdevs = from(stats) >> select([](trx::statistics const & x) { return x.stdev; }) >> to_vector();

        sum.try_calculate(sums);
        min.try_calculate(mins);
        max.try_calculate(maxes);
        mean.try_calculate(means);
        variance.try_calculate(variances);
        stdev.try_calculate(stdevs);

        return true;
    }

    candidate_analysis::candidate_analysis()
        : candidates(),
        game_stats(),
        overall() {
    }

    candidate_analysis::candidate_analysis(candidate_analysis const & other)
        : candidates(other.candidates),
        game_stats(other.game_stats),
        overall(other.overall) {
    }

    candidate_analysis::~candidate_analysis() {
        candidates.clear();
    }

    bool candidate_analysis::try_analyze(data_context & dc) {

        if (candidates.size() != dc.players.size())
            return false;

        auto candidate_games = candidates.first_keys();

        if (candidate_games.size() != dc.games.size())
            return false;

        using namespace cpplinq;

        auto team_size = dc.team_size;

        //TODO: need/want to know about input data context? i.e. for original games/players?
        auto are_games_balanced
            = from(candidate_games) >> all([this, &team_size](game* g) {
            auto values = candidates.get_values_for(g);
            return values.size() == team_size;
        });

        if (!are_games_balanced) return false;

        from(candidate_games) >> for_each([this](game* g) {

            game_stats.emplace(g, trx::statistics());

            auto values = candidates.get_values_for(g);

            auto selected_values = from(values)
                >> select([](candidate_map_type::first_key_value_type const & x) {
                return static_cast<double>(x.value->value); })
                >> to_vector();

            game_stats.at(g).try_calculate(selected_values);
        });

        auto selected_stats = from(game_stats)
            >> select([](game_stats_map_type::value_type const & x) { return x.second; })
            >> to_vector();

        overall.try_analyze(selected_stats);
    }
}
