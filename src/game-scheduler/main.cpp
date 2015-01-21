
#include "scenario.hpp"
#include "best_match_scheduler.h"
#include "evaluation_scheduler.h"

#include "std_out_streamer.h"
#include "file_stream_streamer.h"

int main(int argc, const char* argv[])
{
    using namespace sched;

    //Remember zero (0) is the application name.
    for (auto i = 1; i < argc; i++) {
        //sched::scenario<best_match_scheduler, std_cout_streamer>(argv[i]).run();
        //sched::scenario<best_match_scheduler, file_stream_streamer>(argv[i]).run();
        sched::scenario<evaluation_scheduler, file_stream_streamer>(argv[i]).run();
    }

    return 0;
}
