#ifndef SCHED_DATA_CONTEXT_H
#define SCHED_DATA_CONTEXT_H

#include "game.h"
#include "player.h"

#include <string>
#include <vector>
#include <functional>
#include <ostream>

namespace sched {

    struct data_context {

        std::string name;

        std::vector<game*> games;

        std::vector<player*> players;

        data_context(const char* n);

        virtual ~data_context();

        bool load();

    private:

        void reset();

        bool load(const std::string& path, std::vector<std::string>& lines);

        bool parse_lines(const std::vector<std::string>& lines);

    private:

        static bool default_any_handler(data_context* pdc);

    public:

        bool try_get_team_size(size_t& size) const;

        game* data_context::get_game_by_id(long id) const;

        game* get_game_by_name(const std::string& name) const;

        bool is_satisfied(
            const std::function<bool(data_context*)>& handler
            = default_any_handler);

        bool evaluate();

        bool report(std::ostream& os) const;
    };
}

#endif //SCHED_DATA_CONTEXT_H
