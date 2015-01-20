#ifndef SCHED_CANDIDATOR_H
#define SCHED_CANDIDATOR_H

#include "game.h"
#include "player.h"

namespace sched {

    struct candidator {

        game* g;
        player* p;

        candidator(game* g, player* p);

        candidator(candidator const & other);

    private:

        long product() const;

        bool contains() const;

        static bool contains(std::vector<game*> const & prefs, game* g);

        friend bool operator <(candidator const &, candidator const &);

        friend bool operator >(candidator const &, candidator const &);
    };

    bool operator <(candidator const & a, candidator const & b);

    bool operator >(candidator const & a, candidator const & b);
}

#endif //SCHED_CANDIDATOR_H
