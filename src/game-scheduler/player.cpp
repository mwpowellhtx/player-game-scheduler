#include "player.h"
#include "game.h"
#include "data_context.h"

#include "regex_helpers.h"

#include "tokenize.hpp"

#include <regex>
#include <cstdlib>
#include <sstream>
#include <cassert>

#include "../../tools/cpplinq/cpplinq.hpp"

namespace sched
{
    //G G2 H:7 E:6 P:4
    //P P0 H:3 E:9 P:2 G2,G0,G1
    std::regex player::pattern("^P (P(\\w+)) H:(\\d+) E:(\\d+) P:(\\d+) (G\\d+(,G\\d+)*)$");

    player::player(data_context* pdc, const char* n)
        : scheduled(pdc, n),
        preferences(),
        assignment(nullptr) {
    }

    player::~player() {
        std::for_each(preferences.begin(), preferences.end(),
            [](candidate* x) { if (x) delete x; });
        preferences.clear();
    }

    bool player::is_line_of(const std::string& line) {
        return line.length() && line[0] == 'P';
    }

    player* player::try_parse(data_context* pdc, std::string const & line) {

        using namespace cpplinq;

        player* result = nullptr;

        //Sniff the line for the proper header character.
        if (!line.length()) return result;

        auto init_result = [&pdc, &result]() { result = new player(pdc); };
        auto recv_name = [&result](std::string grp) { result->name = grp; };
        auto recv_id = [&result](std::string grp) { result->id = std::atol(grp.c_str()); };

        auto recv_aspect_h = [&result](std::string grp) { result->set_aspect(AspectType::HandEyeCoordination, std::atol(grp.c_str())); };
        auto recv_aspect_e = [&result](std::string grp) { result->set_aspect(AspectType::Endurance, std::atol(grp.c_str())); };
        auto recv_aspect_p = [&result](std::string grp) { result->set_aspect(AspectType::Pizzazz, std::atol(grp.c_str())); };

        auto lookup = [&pdc](std::string const & name) {
            auto id = std::atol(name.c_str() + 1);
            return pdc->get_game_by_id(id);
        };

        auto recv_prefs = [&result, &lookup](std::string grp) {

            std::vector<std::string> tokenized;
            trx::tokenize(grp, tokenized, ",");

            std::for_each(tokenized.begin(), tokenized.end(),
                [&result, &lookup](std::string const & name) {

                auto g = lookup(name);
                assert(g);
                auto x = new candidate(g, result);
                result->preferences.push_back(x);
                g->available.push_back(x);
            });
        };

        trx::match_and_parse(pattern, line, init_result,
        { { 1, recv_name }, { 2, recv_id }, { 3, recv_aspect_h },
        { 4, recv_aspect_e }, { 5, recv_aspect_p }, { 6, recv_prefs } });

        return result;
    }

    bool player::is_player() const {
        return true;
    }

    bool player::is_preferred(game* g) const {
        return get_preference(g) != nullptr;
    }

    candidate* player::get_preference(game* g) const {

        using namespace cpplinq;

        return from(preferences)
            >> first_or_default([&g](candidate* x) { return x->g == g; });
    }

    bool player::is_assigned() const {
        return assignment != nullptr;
    }

    std::string player::format_preferences() const {

        using namespace cpplinq;

        //G2:128 G1:31 G0:188

        //Formats the game value aligned with the player.
        auto formatter = [this](candidate* x) {
            std::stringstream ss;
            ss << x->g->name << COLON << x->value;
            return ss.str();
        };

        //Extrapolate the formatted valued preferences.
        auto formatted = from(preferences)
            >> select(formatter) >> to_vector();

        //Aggregate the formatted preferences starting with the first one.
        auto seeded = from(formatted) >> cpplinq::first();

        //Join subsequent preferences in space delimited format.
        auto joining = [](std::string curr, std::string x) {
            std::stringstream ss;
            ss << curr << SPACE << x;
            return ss.str();
        };

        //Returns aggregated formatted preferences.
        return from(formatted) >> skip(1) >> aggregate(seeded, joining);
    }

    std::string player::format_report() const {

        //P6 G2:128 G1:31 G0:188
        std::stringstream ss;

        ss << name << SPACE << format_preferences();

        return ss.str();
    }
}
