#ifndef SCHED_SCHEDULER_H
#define SCHED_SCHEDULER_H

#include "data_context.h"

namespace sched {

    struct scheduler_base {

        virtual bool run(data_context & dc) = 0;

    protected:

        scheduler_base();
    };
}

#endif //SCHED_SCHEDULER_H
