#include "data_context.h"

#include "unicode.h"

#include "../../tools/cpplinq/cpplinq.hpp"

#include <iostream>
#include <fstream>

#include <future>
#include <algorithm>

namespace sched {

    data_context::data_context(const char* n)
        : team_size(0),
        name(n),
        games(),
        players(),
        candidates() {
    }

    data_context::~data_context() {
        reset();
    }

    void data_context::reset() {

        using namespace cpplinq;

        from(games) >> for_each([](game* p) { if (p) delete p; });
        from(players) >> for_each([](player* p) { if (p) delete p; });

        games.clear();
        players.clear();

        //Does not own the candidates themselves.
        candidates.clear();
    }

    bool data_context::load() {

        auto path = name + ".txt";

        std::vector<std::string> lines;

        if (!load(path, lines)) return false;

        auto parsed = parse_lines(lines);

        if (parsed)
            team_size = players.size() / games.size();

        return parsed;
    }

    bool data_context::load(std::string const & path,
        std::vector<std::string> & lines) {

        lines.clear();

        //TODO: this one is actually very slow. consider loading all text into memory? or at least more blocks...
        std::ifstream ifs(path, std::ios::in);

        if (ifs) {

            //Apparently needing to skip a Unicode Byte Order Mark
            trx::skip_bom(ifs);

            std::string l;

            while (std::getline(ifs, l))
                lines.push_back(l);
        }

        return lines.size() > 0;
    }

    bool data_context::parse_lines(std::vector<std::string> const & lines) {

        using namespace cpplinq;

        auto games_parser = [](data_context* pdc, std::vector<std::string> const & lines) {
            std::for_each(lines.begin(), lines.end(), [&pdc](std::string const & l) {
                auto fg = std::async(game::try_parse, pdc, l);
                pdc->games.push_back(fg.get());
            });
            return true;
        };

        auto players_parser = [](data_context* pdc, std::vector<std::string> const & lines) {
            std::for_each(lines.begin(), lines.end(), [&pdc](std::string const & l) {
                auto fp = std::async(player::try_parse, pdc, l);
                pdc->players.push_back(fp.get());
            });
            return true;
        };

        //Make a serious effort to do as much of this in parallel as possible.
        auto game_lines = from(lines) >> where(game::is_line_of) >> to_vector();
        auto parsing_games = std::async(games_parser, this, game_lines);

        //Must await games to be parsed prior to players.
        auto games_parsed = parsing_games.get();

        //Scales being what they are squeeze as much performance out of this as possible...
        auto player_lines = from(lines) >> where(player::is_line_of) >> to_vector();
        auto parsing_players = std::async(players_parser, this, player_lines);

        //Now we can proceed with the players.
        auto players_parsed = parsing_players.get();

        candidates = from(players)
            >> select_many([](player* p) { return from(p->preferences); })
            >> to_vector();

        return games_parsed && players_parsed && is_satisfied();
    }

    game* data_context::get_game_by_id(long id) const {
        using namespace cpplinq;
        auto match_game_id_pred = [&id](game* g) { return g->id == id; };
        return from(games) >> first_or_default(match_game_id_pred);
    }

    game* data_context::get_game_by_name(const std::string& name) const {

        using namespace cpplinq;

        auto match_game_name_pred = [&name](game* g) {
            return !strcmp(g->name.c_str(), name.c_str());
        };

        return from(games) >> first_or_default(match_game_name_pred);
    }

    bool data_context::default_any_handler(data_context* pdc) {

        using namespace cpplinq;

        return pdc && (pdc->games.size() || pdc->players.size());
    }

    bool data_context::is_satisfied(
        const std::function<bool(data_context*)>& handler) {
        return handler(this);
    }

    bool data_context::report(std::ostream& os) const {

        using namespace cpplinq;

        if (os) {
            from(games) >> reverse() >> for_each([&os](game* g) {
                os << g->format_report() << std::endl;
            });
        }

        return true;
    }
}
