#ifndef SCHED_GAME_H
#define SCHED_GAME_H

#include "scheduled.h"

#include <set>
#include <deque>
#include <memory>
#include <regex>
#include <functional>

namespace sched
{
    struct player;
    struct candidate;

    struct game : public scheduled {
    private:

        static std::regex pattern;

    public:

        double value;

        size_t remaining_size;

        std::set<candidate*> assigned;

        std::vector<candidate*> available;

        game(data_context* pdc, const char* n = "");

        virtual ~game();

        bool has_candidate(candidate* x) const;

        void refresh();

        static bool is_line_of(std::string const & line);

        static game* try_parse(data_context* pdc, std::string const & line);

        virtual bool is_game() const;

        long operator*(player const & pp) const;

    private:

        long dot_product(player const & pp) const;

    public:

        std::string format_report() const;
    };
}

#endif //SCHED_GAME_H
