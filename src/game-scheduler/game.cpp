#include "game.h"
#include "player.h"
#include "regex_helpers.h"
#include "data_context.h"

#include <regex>
#include <cstdlib>
#include <sstream>
#include <algorithm>

#include "../../tools/cpplinq/cpplinq.hpp"

namespace sched
{
    //G G2 H:7 E:6 P:4
    std::regex game::pattern("^G (G(\\d+)) H:(\\d+) E:(\\d+) P:(\\d+)$");

    game::game(data_context* pdc, const char* n)
        : scheduled(pdc, n),
        value(0),
        remaining_size(0),
        assigned(),
        available() {
    }

    game::~game() {
        //Game owns neither of these.
        assigned.clear();
        available.clear();
    }

    long game::operator*(const player& p) const {
        return dot_product(p);
    }

    long game::dot_product(const player& p) const {

        using namespace cpplinq;

        auto keys = {
            AspectType::HandEyeCoordination,
            AspectType::Endurance,
            AspectType::Pizzazz
        };

        auto result = from(keys) >> select([this, &p](AspectType k) {
            return aspects.at(k) * p.aspects.at(k); }) >> sum();

        return result;
    }

    bool game::has_candidate(candidate* x) const {
        return assigned.find(x) != assigned.end();
    }

    void game::refresh() {

        using namespace cpplinq;

        value = from(assigned)
            >> select([](candidate* x) { return x->value; }) >> avg();

        remaining_size = pdc->team_size - assigned.size();
    }

    bool game::is_line_of(const std::string& line) {
        return line.length() && line[0] == 'G';
    }

    //http://www.cplusplus.com/reference/regex/ECMAScript/
    //Which there are some minor differences between groups in C++11 regex and .NET
    //0=type, 1=name, 2=h, 3=e, 4=p

    game* game::try_parse(data_context* pdc, const std::string& line) {

        game* result = nullptr;

        //Sniff the line for the proper header character.
        if (!line.length()) return result;

        auto init_result = [&pdc, &result]() { result = new game(pdc); };
        auto recv_name = [&result](std::string grp) { result->name = grp; };
        auto recv_id = [&result](std::string grp) { result->id = std::atol(grp.c_str()); };
        auto recv_aspect_h = [&result](std::string grp) { result->set_aspect(AspectType::HandEyeCoordination, std::atol(grp.c_str())); };
        auto recv_aspect_e = [&result](std::string grp) { result->set_aspect(AspectType::Endurance, std::atol(grp.c_str())); };
        auto recv_aspect_p = [&result](std::string grp) { result->set_aspect(AspectType::Pizzazz, std::atol(grp.c_str())); };

        trx::match_and_parse(pattern, line, init_result,
        { { 1, recv_name }, { 2, recv_id }, { 3, recv_aspect_h },
        { 4, recv_aspect_e }, { 5, recv_aspect_p } });

        return result;
    }

    bool game::is_game() const {
        return true;
    }

    std::string game::format_report() const {

        using namespace cpplinq;

        //G2 P6 G2:128 G1:31 G0:188, P3 G2:120 G0:171 G1:31, P10 G0:120 G2:86 G1:21, P0 G2:83 G0:104 G1:17
        std::stringstream ss;

        // Candidate formatter handler.
        auto xformatter = [](candidate* x) {
            return x->p->format_report();
        };

        // The formatted assigned candidates.
        auto temp_assigned = from(assigned)
            >> orderby_descending([this](candidate* x) { return x->value; })
            >> select(xformatter);

        // The first assigned player to seed aggregation.
        auto seeded = temp_assigned >> first();

        // Aggregate handler to join the formatted players.
        auto joining = [](std::string curr, std::string x) {
            std::stringstream ss;
            ss << curr << COMMA << SPACE << x;
            return ss.str();
        };

        // Assigned players to append to the result.
        auto app_assigned = temp_assigned
            >> skip(1) >> aggregate(seeded, joining);

        ss << name << SPACE << app_assigned;

        return ss.str();
    }
}
