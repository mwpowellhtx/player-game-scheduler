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

        size_t team_size;

        std::string name;

        std::vector<game*> games;

        std::vector<player*> players;

        std::vector<candidate*> candidates;

        data_context(const char* n);

        virtual ~data_context();

        bool load();

    private:

        void reset();

        bool load(std::string const & path, std::vector<std::string> & lines);

        bool parse_lines(std::vector<std::string> const & lines);

    private:

        static bool default_any_handler(data_context* pdc);

    public:

        game* data_context::get_game_by_id(long id) const;

        game* get_game_by_name(std::string const & name) const;

        bool is_satisfied(
            const std::function<bool(data_context*)>& handler
            = default_any_handler);

        bool report(std::ostream& os) const;
    };
}

#endif //SCHED_DATA_CONTEXT_H
