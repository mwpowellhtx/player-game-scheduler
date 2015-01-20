#ifndef SCHED_FILE_STREAM_STREAMER_H
#define SCHED_FILE_STREAM_STREAMER_H

#include "streamer_base.h"

#include <string>
#include <ostream>
#include <fstream>
#include <memory>

namespace sched {

    struct file_stream_streamer : streamer_base {

        file_stream_streamer(const char* n);

        std::ostream& stream() const;

    private:

        std::unique_ptr<std::ofstream> pofs;
    };
}

#endif //SCHED_FILE_STREAM_STREAMER_H
