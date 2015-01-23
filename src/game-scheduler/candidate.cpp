#include "candidate.h"
#include "game.h"
#include "player.h"

#include <cmath>

#include "../../tools/cpplinq/cpplinq.hpp"

namespace sched {

    candidate::candidate(game* g, player* p)
        : g(g), p(p),
        value(0),
        potential_value(0),
        delta_value(0) {

        refresh();
    }

    candidate::candidate(candidate const & other)
        : g(other.g), p(other.p),
        value(other.value),
        potential_value(other.potential_value),
        delta_value(other.delta_value) {
    }

    void candidate::fulfill() {

        if (is_assigned() || g->has_candidate(this)) return;

        g->assigned.emplace(this);
        p->assignment = this;

        g->refresh();
    }

    void candidate::refresh() {

        if (g && p) value = (*g) * (*p);

        //using namespace cpplinq;

        //potential_value = g ? from(g->assigned)
        //    >> select([](candidate* x) { return x->value; })
        //    >> concat(from(std::vector<long>({ value }))) >> avg() : 0;

        //delta_value = std::abs(value - potential_value);
    }

    bool candidate::is_assigned() const {
        return p
            && g
            && p->assignment != nullptr
            && p->assignment->g == g;
    }

    bool candidate::is_valid() const {
        return !(g == nullptr || p == nullptr);
    }
}
