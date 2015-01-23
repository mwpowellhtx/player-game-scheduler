#ifndef SCHED_PLAYER_H
#define SCHED_PLAYER_H

#include "scheduled.h"
#include "candidate.h"

#include <vector>
#include <regex>
#include <functional>

namespace sched
{
    struct player : public scheduled {
    private:

        static std::regex pattern;

    public:

        std::vector<candidate*> preferences;

        //Still need to maintain preference for final report but also cross reference available
        candidate* assignment;

        player(data_context* pdc, const char* n = "");

        virtual ~player();

        static bool is_line_of(std::string const & line);

        static player* try_parse(data_context* pdc, std::string const & line);

        virtual bool is_player() const;

        bool is_assigned() const;

        bool is_preferred(game* g) const;

        candidate* get_preference(game* g) const;

    private:

        std::string format_preferences() const;

    public:

        std::string format_report() const;
    };
}

#endif //SCHED_PLAYER_H
