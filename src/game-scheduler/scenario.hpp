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

            //TODO: could put some hooks in for event processing, monitor number of solutions, timeout, and so forth
            //TODO: potentially go boost-enabled, for instance; using signals, the date/time, etc
            Scheduler sched;

            if (!sched.run(dc)) return false;

            Streamer streamer(name.c_str());

            return !dc.report(streamer.stream());
        }
    };
}

#endif //SCHED_SCENARIO_H
