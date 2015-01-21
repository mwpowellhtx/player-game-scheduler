#ifndef SCHED_GAME_H
#define SCHED_GAME_H

#include "scheduled.h"
#include "candidator.h"

#include <set>
#include <deque>
#include <memory>
#include <regex>
#include <functional>

namespace sched
{
    struct player;
    struct data_context;

    struct game : public scheduled {
    private:

        static std::regex pattern;

    public:

        std::set<player*> assigned;

        std::deque<candidator*> available;

        game();

        game(const char* n);

        void add(player* p);

        void prepare();

        void refresh();

        std::vector<player*>::size_type true_available_size();

        size_t get_remaining_choices(size_t max_size);

        long next_value();

        void evaluate();

        static bool is_line_of(std::string const & line);

        static game* try_parse(std::string const & line);

        virtual bool is_game() const;

        long operator*(player const & pp) const;

    private:

        long dot_product(player const & pp) const;

    public:

        std::string format_report(data_context const & dc) const;
    };
}

#endif //SCHED_GAME_H
