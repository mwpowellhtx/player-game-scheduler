#include "game.h"
#include "player.h"
#include "regex_helpers.h"

#include <regex>
#include <cstdlib>
#include <sstream>

#include "../../tools/cpplinq/cpplinq.hpp"

namespace sched
{
    //G G2 H:7 E:6 P:4
    std::regex game::pattern("^G (G(\\d+)) H:(\\d+) E:(\\d+) P:(\\d+)$");

    game::game()
        : scheduled(""),
        players() {
    }

    game::game(const char* n)
        : scheduled(n),
        players() {
    }

    long game::operator*(const player& j) const {
        return dot_product(j);
    }

    long game::dot_product(const player& j) const {

        using namespace cpplinq;

        auto keys = {
            AspectType::HandEyeCoordination,
            AspectType::Endurance,
            AspectType::Pizzazz
        };

        auto result = from(keys) >> select([this, &j](AspectType k) {
            return aspects.at(k) * j.aspects.at(k); }) >> sum();

        return result;
    }

    void game::add(player* pp) {

        if (pp->assignment != nullptr) return;

        if (players.find(pp) == players.end()) {
            players.emplace(pp);
            pp->assignment = this;
        }
    }

    bool game::is_line_of(const std::string& line) {
        return line.length() && line[0] == 'G';
    }

    //http://www.cplusplus.com/reference/regex/ECMAScript/
    //Which there are some minor differences between groups in C++11 regex and .NET
    //0=type, 1=name, 2=h, 3=e, 4=p

    game* game::try_parse(const std::string& line) {

        game* result = nullptr;

        //Sniff the line for the proper header character.
        if (!line.length()) return result;

        auto init_result = [&result]() { result = new game; };
        auto recv_name = [&result](std::string g) { result->name = g; };
        auto recv_id = [&result](std::string g) { result->id = std::atol(g.c_str()); };
        auto recv_aspect_h = [&result](std::string g) { result->set_aspect(AspectType::HandEyeCoordination, std::atol(g.c_str())); };
        auto recv_aspect_e = [&result](std::string g) { result->set_aspect(AspectType::Endurance, std::atol(g.c_str())); };
        auto recv_aspect_p = [&result](std::string g) { result->set_aspect(AspectType::Pizzazz, std::atol(g.c_str())); };

        trx::match_and_parse(pattern, line, init_result,
        { { 1, recv_name }, { 2, recv_id }, { 3, recv_aspect_h },
        { 4, recv_aspect_e }, { 5, recv_aspect_p } });

        return result;
    }

    bool game::is_game() const {
        return true;
    }

    std::string game::format_report(data_context const & dc) const {

        using namespace cpplinq;

        //G2 P6 G2:128 G1:31 G0:188, P3 G2:120 G0:171 G1:31, P10 G0:120 G2:86 G1:21, P0 G2:83 G0:104 G1:17
        std::stringstream ss;

        // Player formatter handler.
        auto pformatter = [&dc](player* p) {
            return p->format_report(dc);
        };

        // The formatted players.
        auto temp_players = from(players)
            >> orderby_descending([this](player* p) { return (*this) * (*p); })
            >> select(pformatter);

        // The first player to seed aggregation.
        auto seeded = temp_players >> first();

        // Aggregate handler to join the formatted players.
        auto joining = [](std::string curr, std::string x) {
            std::stringstream ss;
            ss << curr << COMMA << SPACE << x;
            return ss.str();
        };

        // players to append to the result.
        auto app_players = temp_players
            >> skip(1) >> aggregate(seeded, joining);

        ss << name << SPACE << app_players;

        return ss.str();
    }
}
