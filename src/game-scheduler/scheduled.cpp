#include "scheduled.h"

#include <exception>

namespace sched
{
    scheduled::scheduled(data_context* pdc, const char* n)
        : id(0L),
        name(n),
        aspects(),
        pdc(pdc) {

        init();
    }

    void scheduled::init() {
        set_aspect(AspectType::HandEyeCoordination, 0L);
        set_aspect(AspectType::Endurance, 0L);
        set_aspect(AspectType::Pizzazz, 0L);
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
