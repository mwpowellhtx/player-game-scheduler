#include "candidator.h"

#include "game.h"

namespace sched {

    candidator::candidator(game* g, player* p)
        : g(g), p(p), value((*g) * (*p)) {
    }

    candidator::candidator(candidator const & other)
        : g(other.g), p(other.p), value(other.value) {
    }
}
