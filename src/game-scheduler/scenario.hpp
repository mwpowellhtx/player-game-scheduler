#ifndef SCHED_SCENARIO_H
#define SCHED_SCENARIO_H

#include "data_context.h"

#include <memory>

namespace sched
{
    template<class Scheduler, class Streamer>
    struct scenario {

        std::string name;

        scenario(const char* n)
            : name(n) {
        }

        ~scenario() {
        }

        bool run() {

            //TODO: this could potentially be severely parallelized! promises, futures, etc...
            data_context dc(name.c_str());

            if (!dc.load()) return false;

            Scheduler sched;

            if (!sched.run(dc)) return false;

            Streamer streamer(name.c_str());

            return !dc.report(streamer.stream());
        }
    };
}

#endif //SCHED_SCENARIO_H
