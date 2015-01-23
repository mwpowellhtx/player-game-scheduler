#ifndef SCHED_MAPPING_SCHEDULER_H
#define SCHED_MAPPING_SCHEDULER_H

#include "scheduler.h"

#include <map>

namespace sched {

    struct mapping_scheduler : scheduler_base {

        mapping_scheduler();

        virtual bool run(data_context & dc);

    private:

        typedef std::map<game*, std::map<game*, candidate>> candidate_map_type;

        candidate_map_type candidate_map;

        bool evaluate(data_context & dc);

        bool verify(data_context & dc);

        void fulfill_front_candidate(data_context & dc);
    };
}

#endif //SCHED_MAPPING_SCHEDULER_H
