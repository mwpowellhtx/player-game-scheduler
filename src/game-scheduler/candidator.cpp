#include "candidator.h"

#include "../../tools/cpplinq/cpplinq.hpp"

namespace sched {

    candidator::candidator(game* g, player* p)
        : g(g), p(p) {
    }

    candidator::candidator(candidator const & other)
        : g(other.g), p(other.p) {
    }

    long candidator::product() const {
        return (*g) * (*p);
    }

    bool candidator::contains() const {
        return contains(p->preferences, g);
    }

    bool candidator::contains(std::vector<game*> const & prefs, game* g) {
        return cpplinq::from(prefs) >> cpplinq::contains(g);
    }

    bool operator <(candidator const & a, candidator const & b) {

        auto a_contains = a.contains();
        auto b_contains = b.contains();

        //Player preferences containing the game precedes
        if (!a_contains && b_contains)
            return true;

        //Defer to dot products otherwise.
        if (b_contains == a_contains)
            return a.product() < b.product();

        //Finally is not less for sure.
        return false;
    }

    bool operator >(candidator const & a, candidator const & b) {

        auto a_contains = a.contains();
        auto b_contains = b.contains();

        //Player preferences containing the game precedes
        if (a_contains && !b_contains)
            return true;

        //Defer to dot products otherwise.
        if (b_contains == a_contains)
            return a.product() > b.product();

        //Finally is not greater for sure.
        return false;
    }
}
