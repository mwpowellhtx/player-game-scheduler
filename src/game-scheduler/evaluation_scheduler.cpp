////TODO: was an interesting experiment but I'm not sure this is the way to handle it...
//#include "evaluation_scheduler.h"
//
//#include "../../tools/cpplinq/cpplinq.hpp"
//
//#include <future>
//#include <thread>
//#include <cassert>
//
//namespace sched {
//
//    evaluation_scheduler::evaluation_scheduler() {}
//
//    void service_all(std::vector<game*>& games,
//        std::function<void(game*)> const & handler) {
//
//        std::vector<std::thread> preparations;
//
//        //For each preparation.
//        auto make_preparation = [&preparations, &handler](game* g) {
//            preparations.push_back(std::thread(handler, g));
//        };
//
//        //Make the preparations.
//        std::for_each(games.begin(), games.end(), make_preparation);
//
//        //Rejoin each of the preparations.
//        std::for_each(preparations.begin(), preparations.end(),
//            [](std::thread & t) { t.join(); });
//    }
//
//    void evaluation_scheduler::add(
//        size_t team_size,
//        player* p, game* g,
//        std::vector<game*>& games,
//        std::vector<player*>& players) {
//
//        using namespace cpplinq;
//
//        g->add(p);
//
//        ////Service all of the game refresh requests.
//        //service_all(games, [](game* g) { g->refresh(); });
//
//        //Do the erase-remove-if operation prior to verifying games validity.
//        if (g->assigned.size() == team_size) {
//            games.erase(
//                std::remove_if(games.begin(), games.end(),
//                [&g](game* const & x) { return x == g; }));
//        }
//
////#ifdef _DEBUG
////        assert(from(games) >> all([](game* g) {
////            return g->available.size() > 0;
////        }));
////#endif
//
//    }
//
//    bool evaluation_scheduler::evaluate(
//        size_t team_size,
//        std::vector<game*>& games,
//        std::vector<player*>& players) {
//
//        using namespace cpplinq;
//
//        games = from(games)
//            >> orderby_ascending([](game* g) { return g->available.size(); })
//            >> thenby_descending([](game* g) { return g->assigned.size(); })
//            >> to_vector();
//
//        players = from(players)
//            >> where([](player* p) { return !p->is_assigned(); }) >> to_vector();
//
//        //Return whether there are players and games, and that the games have available players.
//        return from(players) >> any()
//            && from(games) >> any()
//            && from(games) >> all([](game* g) { return from(g->available) >> any(); });
//    }
//
//    bool evaluation_scheduler::run(data_context & dc) {
//
//        //TODO: you may just get all the permutations and find the one with the unique combination(s)? that also maximize performance?
//        using namespace cpplinq;
//
//        size_t team_size;
//
//        dc.try_get_team_size(team_size);
//
//        auto games = from(dc.games) >> to_vector();
//
//        service_all(games, [](game* g) { g->prepare(); });
//
//        auto players = from(dc.players) >> to_vector();
//
//        //TODO: handle 'corner cases' of exhausted choices
//        while (evaluate(team_size, games, players)) {
//
//            assert(games.size());
//            assert(players.size());
//
//            //Evaluation is preparation for the next choice
//            auto g = *games.begin();
//            auto x = g->available.front();
//            g->available.pop_front();
//
//            assert(g);
//            assert(x);
//
//            auto p = x->p;
//
//            //Will pull resources from the broader context when exhausted
//            if (p->is_assigned()) {
//
//                p = from(players)
//                    >> orderby_descending([&g](player* p) {return (*g) * (*p); })
//                    >> first_or_default();
//
//                //Should have next candidate for assignment
//                assert(p);
//            }
//
//            add(team_size, p, g, games, players);
//        }
//
//        //TODO: put some guards in and an accurate return value
//
//        auto game_balance_pred = [&team_size](game* g) { return g->assigned.size() == team_size; };
//        auto all_games_balanced = from(dc.games) >> all(game_balance_pred);
//
//        auto player_assigned_pred = [](player* p) { return p->is_assigned(); };
//        auto all_players_assigned = from(dc.players) >> all(player_assigned_pred);
//
//        assert(all_players_assigned && all_games_balanced);
//
//        return all_players_assigned && all_games_balanced;
//    }
//}
