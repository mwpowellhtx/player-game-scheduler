//#include "candidate_scheduler.h"
//
//#include <algorithm>
//
//#include "../../tools/cpplinq/cpplinq.hpp"
//
//namespace sched {
//
//    candidate_scheduler::candidate_scheduler()
//        : scheduler_base() {
//    }
//
//    bool candidate_scheduler::run(data_context & dc) 
//    {
//        using namespace cpplinq;
//
//        while (evaluate(dc)) {
//            fulfill_front_candidate(dc);
//        }
//
//        auto verified = verify(dc);
//
//        assert(verified);
//
//        return verified;
//    }
//
//    bool candidate_scheduler::evaluate(data_context & dc) {
//
//        if (!dc.candidates.size()) return false;
//
//        using namespace cpplinq;
//
//        from(dc.candidates) >> for_each([](candidate* x) { x->refresh(); });
//
//        //Capture a couple of predicates used for ordering purposes.
//        auto remaining_size_pred = [](candidate* x) { return x->g->remaining_size; };
//        auto available_size_pred = [](candidate* x) { return x->g->available.size(); };
//        auto delta_value_pred = [](candidate* x) { return x->delta_value; };
//        auto potential_value_pred = [](candidate* x) { return x->potential_value; };
//        auto value_pred = [](candidate* x) { return x->value; };
//
//        if (1 == from(dc.games)
//            >> select([](game* g) { return g->assigned.size(); })
//            >> distinct() >> count()) {
//
//            //With fewer choices available you want to identify one of those first.
//            //Then ranking by candidate closeness, potential value, and individual value.
//            dc.candidates = from(dc.candidates)
//                >> orderby_ascending(available_size_pred)
//                >> thenby_ascending(delta_value_pred)
//                >> thenby_descending(potential_value_pred)
//                >> thenby_descending(value_pred)
//                >> to_vector();
//        }
//        else {
//
//            //Draw candidates before they are exhausted.
//            //Prefer closer to center matches, or by game and player value.
//            dc.candidates = from(dc.candidates)
//                >> orderby_descending(remaining_size_pred)
//                >> thenby_ascending(available_size_pred)
//                >> thenby_ascending(delta_value_pred)
//                >> thenby_descending(potential_value_pred)
//                >> thenby_descending(value_pred)
//                >> to_vector();
//        }
//
//        return true;
//    }
//
//    void candidate_scheduler::fulfill_front_candidate(data_context & dc) {
//
//        using namespace cpplinq;
//
//        auto x = dc.candidates.front();
//
//        //TODO: do we need to do any backtracking of any sort?
//        x->fulfill();
//
//        //Remove the candidate from each game available candidates.
//        from(dc.games) >> for_each([&x](game* g) {
//            g->available = from(g->available)
//                >> where([&x](candidate* y) { return y->p != x->p; }) >> to_vector();
//        });
//
//        auto team_size = dc.team_size;
//
//        //Now remove the candidates themselves. Also leaving out fulfilled games.
//        dc.candidates = from(dc.candidates)
//            >> where([&x](candidate* y) { return !(y == x || y->p == x->p); })
//            >> where([&team_size](candidate* y) { return y->g->assigned.size() < team_size; })
//            >> to_vector();
//    }
//
//    bool candidate_scheduler::verify(data_context & dc) {
//
//        using namespace cpplinq;
//
//        auto no_more_candidates = !(from(dc.candidates) >> any());
//
//        auto team_size = dc.team_size;
//
//        auto all_game_assigned_sizes_good = from(dc.games)
//            >> all([&team_size](game* g) { return g->assigned.size() == team_size; });
//
//        return no_more_candidates && all_game_assigned_sizes_good;
//    }
//}
