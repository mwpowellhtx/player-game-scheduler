#ifndef SCHED_CANDIDATOR_H
#define SCHED_CANDIDATOR_H

namespace sched {

    struct game;

    struct player;

    struct candidator {

        game* g;

        player* p;

        long value;

        candidator(game* g, player* p);

        candidator(candidator const & other);
    };
}

#endif //SCHED_CANDIDATOR_H
