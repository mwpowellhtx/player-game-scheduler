#include "file_stream_streamer.h"

namespace sched {

    file_stream_streamer::file_stream_streamer(const char* n)
        : streamer_base(n), pofs() {

        auto path = name + ".out";
        pofs = std::make_unique<std::ofstream>(
            path.c_str(), std::ios::out);
    }

    std::ostream& file_stream_streamer::stream() const {
        return *(dynamic_cast<std::ostream*>(pofs.get()));
    }
}
