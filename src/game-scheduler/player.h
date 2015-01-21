#ifndef SCHED_PLAYER_H
#define SCHED_PLAYER_H

#include "scheduled.h"
#include "candidator.h"

#include <vector>
#include <regex>
#include <functional>

namespace sched
{
    struct game;
    struct data_context;

    struct player : public scheduled {
    private:

        static std::regex pattern;

    public:

        //Still need to maintain preference for final report but also cross reference available
        std::vector<candidator*> preferences;

        game* assignment;

        player();

        virtual ~player();

        static bool is_line_of(std::string const & line);

        static player* try_parse(std::string const & line, data_context* dc);

        virtual bool is_player() const;

        bool is_assigned() const;

    private:

        std::string format_preferences(data_context const & dc) const;

    public:

        std::string format_report(data_context const & dc) const;
    };
}

#endif //SCHED_PLAYER_H
