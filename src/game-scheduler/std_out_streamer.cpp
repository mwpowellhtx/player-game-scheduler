#include "std_out_streamer.h"

namespace sched {

    std_cout_streamer::std_cout_streamer(const char* n)
        : streamer_base(n) {
    }

    std::ostream& std_cout_streamer::stream() const {
        return std::cout;
    }
}
