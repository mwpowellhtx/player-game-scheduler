#ifndef SCHED_STD_OUT_STREAMER_H
#define SCHED_STD_OUT_STREAMER_H

#include "streamer_base.h"

#include <string>
#include <iostream>

namespace sched {

    struct std_cout_streamer : streamer_base {

        std_cout_streamer(const char* n);

        std::ostream& stream() const;
    };
}

#endif //SCHED_STD_OUT_STREAMER_H
