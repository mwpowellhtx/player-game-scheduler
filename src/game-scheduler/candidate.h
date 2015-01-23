#ifndef SCHED_CANDIDATE_H
#define SCHED_CANDIDATE_H

namespace sched {

    struct game;
    struct player;

    struct candidate {

        game* g;

        player* p;

        long value;

        double potential_value;

        double delta_value;

        candidate(game* g = nullptr, player* p = nullptr);

        candidate(candidate const & other);

        void fulfill();

        void refresh();

        bool is_assigned() const;

        bool is_valid() const;
    };
}

#endif //SCHED_CANDIDATE_H
