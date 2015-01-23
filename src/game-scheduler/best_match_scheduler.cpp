//#include "best_match_scheduler.h"
//#include "candidator.h"
//
//#include "../../tools/cpplinq/cpplinq.hpp"
//
//#include <cassert>
//#include <algorithm>
//
//namespace sched {
//
//    best_match_scheduler::best_match_scheduler()
//        : scheduler_base() {
//    }
//
//    bool unassigned_player_pred(player* p) {
//        return p && p->assignment == nullptr;
//    }
//
//    bool try_select_players(data_context const & dc,
//        std::vector<player*>& selected,
//        const std::function<bool(player*)>& pred) {
//
//        using namespace cpplinq;
//
//        from(dc.players) >> where(pred)
//            >> for_each([&selected](player* p) { selected.push_back(p); });
//
//        return selected.size() > 0;
//    }
//
//    bool best_match_scheduler::run(data_context & dc) {
//
//        using namespace cpplinq;
//
//        size_t team_size = 0;
//
//        //Test for early failure.
//        if (!dc.try_get_team_size(team_size))
//            return false;
//
//        //Work through only unassigned games.
//        auto pending_games = from(dc.games) >> where([](game* const & g) {
//            return g->assigned.size() == 0;
//        }) >> to_vector();
//
//        from(pending_games) >> for_each([](game* g) { g->prepare(); });
//
//        //TODO: instead of loading players with game preferences: add players to games as candidates, and do the refining from there...
//        //TODO: might also keep an 'assigned' vector during the process
//        for (auto* g : pending_games) {
//
//            std::vector<player*> unassigned_players;
//
//            if (!try_select_players(dc, unassigned_players, unassigned_player_pred))
//                return false;
//
//            auto candidates = from(unassigned_players)
//                >> select([&g](player* p) { return candidator(g, p); }) >> to_vector();
//
//            from(candidates) >> take(team_size)
//                >> for_each([](candidator const & x) { x.g->add(x.p); });
//
//            //auto game_pref_pred = [&g](player* p) {
//            //    return (from(p->preferences) >> contains(g)) ? 1 : 0; };
//
//            //auto best_match_pred = [&g](player* p) { return (*g) * (*p); };
//
//            ////Place the best possible players in the game.
//            //from(unassigned_players) >> orderby_descending(game_pref_pred)
//            //    >> thenby_descending(best_match_pred) >> take(team_size)
//            //    >> for_each([&g](player* p) { g->add(p); });
//        }
//
//        auto assigned_count = from(dc.games)
//            >> select([](game* g) { return g->assigned.size(); }) >> sum();
//
//        assert(assigned_count == dc.players.size());
//
//        return true;
//    }
//}
