#include "player.h"
#include "game.h"
#include "data_context.h"

#include "regex_helpers.h"

#include "tokenize.hpp"

#include <regex>
#include <cstdlib>
#include <sstream>

#include "../../tools/cpplinq/cpplinq.hpp"

namespace sched
{
    //G G2 H:7 E:6 P:4
    //P P0 H:3 E:9 P:2 G2,G0,G1
    std::regex player::pattern("^P (P(\\w+)) H:(\\d+) E:(\\d+) P:(\\d+) (G\\d+(,G\\d+)*)$");

    player::player()
        : scheduled(""),
        preferences(),
        assignment(nullptr) {
    }

    bool player::is_line_of(const std::string& line) {
        return line.length() && line[0] == 'P';
    }

    player* player::try_parse(const std::string& line, data_context* dc) {

        using namespace cpplinq;

        player* result = nullptr;

        //Sniff the line for the proper header character.
        if (!line.length()) return result;

        auto init_result = [&result]() { result = new player; };
        auto recv_name = [&result](std::string g) { result->name = g; };
        auto recv_id = [&result](std::string g) { result->id = std::atol(g.c_str()); };

        auto recv_aspect_h = [&result](std::string g) { result->set_aspect(AspectType::HandEyeCoordination, std::atol(g.c_str())); };
        auto recv_aspect_e = [&result](std::string g) { result->set_aspect(AspectType::Endurance, std::atol(g.c_str())); };
        auto recv_aspect_p = [&result](std::string g) { result->set_aspect(AspectType::Pizzazz, std::atol(g.c_str())); };

        auto recv_prefs = [&dc, &result](std::string g) {

            std::vector<std::string> tokenized;
            trx::tokenize(g, tokenized, ",");

            auto filtered = from(tokenized)
                >> select([](std::string const & n) { return atol(n.c_str() + 1); })
                >> select([&dc](long id) { return dc->get_game_by_id(id); })
                >> to_vector();

            result->preferences.insert(result->preferences.end(),
                filtered.begin(), filtered.end());
        };

        trx::match_and_parse(pattern, line, init_result,
        { { 1, recv_name }, { 2, recv_id }, { 3, recv_aspect_h },
        { 4, recv_aspect_e }, { 5, recv_aspect_p }, { 6, recv_prefs } });

        return result;
    }

    bool player::is_player() const {
        return true;
    }

    std::string player::format_preferences(data_context const & dc) const {

        using namespace cpplinq;

        //G2:128 G1:31 G0:188

        //Formats the game value aligned with the player.
        auto formatter = [this](game* pg) {
            std::stringstream ss;
            auto value = (*pg) * (*this);
            ss << pg->name << COLON << value;
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

    std::string player::format_report(data_context const & dc) const {

        //P6 G2:128 G1:31 G0:188
        std::stringstream ss;

        ss << name << SPACE << format_preferences(dc);

        return ss.str();
    }
}
