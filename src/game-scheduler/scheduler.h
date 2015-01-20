#ifndef SCHED_SCHEDULER_H
#define SCHED_SCHEDULER_H

#include "data_context.h"

namespace sched {

    struct scheduler_base {

    protected:

        scheduler_base();

    public:

        virtual bool run(data_context const & dc) = 0;
    };
}

#endif //SCHED_SCHEDULER_H
