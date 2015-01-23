#include "mapping_scheduler.h"
#include "candidate_analysis.h"

#include "sparse_matrix.hpp"

#include "../../tools/cpplinq/cpplinq.hpp"

#include <deque>
#include <algorithm>

namespace sched {

    mapping_scheduler::mapping_scheduler()
        : scheduler_base(),
        candidate_map() {
    }



    bool mapping_scheduler::run(data_context & dc) {

        using namespace cpplinq;
        using std::next_permutation;

        //Should be in id-order.
        auto players = from(dc.players) >> to_vector();
        auto games = from(dc.games) >> to_vector();

        typedef std::multimap<game*, player*> map_type;

        std::deque<map_type> history;

        auto team_size = dc.team_size;

        std::deque<candidate_analysis> analyses;

        size_t count;

        do {

            candidate_analysis current;

            auto gbegin = games.begin();

            auto pbegin = players.begin();
            auto pend = players.end();

            for (auto pit = pbegin; pit != pend; pit++) {

                auto goffset = (pit - pbegin) / team_size;
                auto gcurrent = gbegin + goffset;

                auto p = *pit;
                auto g = *gcurrent;

                auto x = p->get_preference(g);

                if (x == nullptr) break;

                current.candidates(g, p) = x;
            }

            if (!current.try_analyze(dc))
                continue;

            //TODO: if empty, or supersedes the fore most analysis
            if (!analyses.size()) {
                analyses.push_front(current);
            }
            else {
                auto& front = analyses.front();
                if (current.overall.max.max > front.overall.max.max
                    && current.overall.mean.mean < front.overall.mean.mean) {
                    analyses.push_front(current);
                }
            }

        } while (next_permutation(players.begin(), players.end(),
            [](player* a, player* b) { return a->id < b->id; }));

        return true;
    }



    bool mapping_scheduler::evaluate(data_context & dc) {
        return true;
    }

    bool mapping_scheduler::verify(data_context & dc) {
        return true;
    }

    void mapping_scheduler::fulfill_front_candidate(data_context & dc) {
    }
}
