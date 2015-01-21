#include "game.h"
#include "player.h"
#include "regex_helpers.h"

#include <regex>
#include <cstdlib>
#include <sstream>
#include <algorithm>

#include "../../tools/cpplinq/cpplinq.hpp"

namespace sched
{
    //G G2 H:7 E:6 P:4
    std::regex game::pattern("^G (G(\\d+)) H:(\\d+) E:(\\d+) P:(\\d+)$");

    game::game()
        : scheduled(""),
        assigned(),
        available() {
    }

    game::game(const char* n)
        : scheduled(n),
        assigned(),
        available() {
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

    void game::prepare() {

        using namespace cpplinq;

        available = from(available)
            >> orderby_descending([](candidator* x) { return x->value; })
            >> to_deque();
    }

    void game::add(player* p) {

        if (p->assignment != nullptr) return;

        if (assigned.find(p) == assigned.end()) {
            assigned.emplace(p);
            p->assignment = this;
        }
    }

    void game::refresh() {

        using namespace cpplinq;

        auto pending_remove = from(available)
            >> where([](candidator* x) { return x->p->assignment != nullptr; })
            >> to_vector();

        available = from(available)
            >> except(from(pending_remove)) >> to_deque();
    }

    size_t game::get_remaining_choices(size_t max_size) {
        return max_size - assigned.size();
    }

    long game::next_value() {
        return available.size() ? available.front()->value : 0L;
    }

    std::vector<player*>::size_type game::true_available_size() {

        using namespace cpplinq;

        return from(available)
            >> where([](candidator* x) { return x->p->assignment == nullptr; })
            >> count();
    }

    void game::evaluate() {

        using namespace cpplinq;

        available = from(available)
            >> orderby_ascending([](candidator* x) { return x->p->assignment == nullptr ? 0 : 1; })
            >> thenby_descending([this](candidator* x) { return (*this) * (*(x->p)); })
            >> to_deque();
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

    std::string game::format_report(data_context const & dc) const {

        using namespace cpplinq;

        //G2 P6 G2:128 G1:31 G0:188, P3 G2:120 G0:171 G1:31, P10 G0:120 G2:86 G1:21, P0 G2:83 G0:104 G1:17
        std::stringstream ss;

        // Player formatter handler.
        auto pformatter = [&dc](player* p) {
            return p->format_report(dc);
        };

        // The formatted assigned players.
        auto temp_assigned = from(assigned)
            >> orderby_descending([this](player* p) { return (*this) * (*p); })
            >> select(pformatter);

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
