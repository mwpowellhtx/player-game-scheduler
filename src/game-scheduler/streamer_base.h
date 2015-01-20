#ifndef SCHED_STREAMER_BASE_H
#define SCHED_STREAMER_BASE_H

#include <string>
#include <ostream>

namespace sched {

    struct streamer_base {

        std::string name;

        virtual std::ostream& stream() const = 0;

    protected:

        streamer_base(const char* n);
    };
}

#endif //SCHED_STREAMER_BASE_H
