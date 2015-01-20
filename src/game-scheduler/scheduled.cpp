#include "scheduled.h"

#include <exception>

namespace sched
{
    scheduled::scheduled()
        : id(0L),
        name(),
        aspects({
            { AspectType::HandEyeCoordination, 0L },
            { AspectType::Endurance, 0L },
            { AspectType::Pizzazz, 0L }
    }) {
    }

    scheduled::scheduled(const char* n)
        : id(0L),
        name(n),
        aspects({
            { AspectType::HandEyeCoordination, 0L },
            { AspectType::Endurance, 0L },
            { AspectType::Pizzazz, 0L }
    }) {
    }

    void scheduled::set_aspect(AspectType key, long value) {
        aspects[key] = value;
    }

    bool scheduled::is_game() const {
        return false;
    }

    bool scheduled::is_player() const {
        return false;
    }

    AspectType to_aspect_type(char ch) {

        switch (ch)
        {
        case AspectType::HandEyeCoordination:
            return AspectType::HandEyeCoordination;
        case AspectType::Endurance:
            return AspectType::Endurance;
        case AspectType::Pizzazz:
            return AspectType::Pizzazz;
        }

        throw std::exception();
    }
}
