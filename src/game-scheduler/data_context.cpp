#include "data_context.h"

#include "unicode.h"

#include "../../tools/cpplinq/cpplinq.hpp"

#include <iostream>
#include <fstream>

#include <future>
#include <algorithm>

namespace sched {

    data_context::data_context(const char* n)
        : name(n), games(), players() {
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
    }

    bool data_context::load() {

        auto path = name + ".txt";

        std::vector<std::string> lines;

        if (!load(path, lines)) return false;

        return parse_lines(lines);
    }

    bool data_context::load(const std::string& path,
        std::vector<std::string>& lines) {

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

    bool games_parser(std::vector<game*>* games, std::vector<std::string>* lines) {

        std::for_each(lines->begin(), lines->end(),
            [&games](std::string & l) {

            auto fg = std::async(game::try_parse, l);
            games->push_back(fg.get());
        });

        return true;
    }

    bool players_parser(data_context* pdc, std::vector<player*>* players,
        std::vector<std::string>* lines) {

        std::for_each(lines->begin(), lines->end(),
            [&pdc, &players](std::string & l) {

            auto fp = std::async(player::try_parse, l, pdc);
            players->push_back(fp.get());
        });

        return true;
    }

    bool data_context::parse_lines(const std::vector<std::string>& lines) {

        using namespace cpplinq;

        //Make a serious effort to do as much of this in parallel as possible.
        auto game_lines = from(lines) >> where(game::is_line_of) >> to_vector();
        auto parsing_games = std::async(games_parser, &games, &game_lines);

        //Must await games to be parsed prior to players.
        auto games_parsed = parsing_games.get();

        //Scales being what they are squeeze as much performance out of this as possible...
        auto player_lines = from(lines) >> where(player::is_line_of) >> to_vector();
        auto parsing_players = std::async(players_parser, this, &players, &player_lines);

        //Now we can proceed with the players.
        auto players_parsed = parsing_players.get();

        return games_parsed && players_parsed && any();
    }

    bool data_context::try_get_team_size(size_t& size) const {
        if (!games.size()) return false;
        size = players.size() / games.size();
        return size > 0;
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

    bool data_context::any(const std::function<bool(data_context*)>& handler) {
        return handler(this);
    }

    bool data_context::report(std::ostream& os) const {

        using namespace cpplinq;

        if (os) {
            from(games) >> reverse() >> for_each([this, &os](game* g) {
                os << g->format_report((*this)) << std::endl;
            });
        }

        return true;
    }
}
