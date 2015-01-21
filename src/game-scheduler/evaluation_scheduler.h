#ifndef SCHED_EVALUATION_SCHEDULER_H
#define SCHED_EVALUATION_SCHEDULER_H

#include "scheduler.h"
#include "game.h"
#include "player.h"

namespace sched {

    struct evaluation_scheduler : scheduler_base {

        evaluation_scheduler();

        virtual bool run(data_context & dc);

    private:

        bool evaluate(size_t team_size,
            std::vector<game*>& games,
            std::vector<player*>& players);

        void add(size_t team_size,
            player* p, game* g,
            std::vector<game*>& games,
            std::vector<player*>& players);
    };
}

#endif //SCHED_EVALUATION_SCHEDULER_H
