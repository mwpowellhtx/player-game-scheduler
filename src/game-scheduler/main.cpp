
#include "scenario.hpp"
#include "best_match_scheduler.h"
#include "evaluation_scheduler.h"
#include "candidate_scheduler.h"
#include "mapping_scheduler.h"

#include "std_out_streamer.h"
#include "file_stream_streamer.h"

int main(int argc, const char* argv[])
{
    using namespace sched;

    /* Having exercised several scheduling scenarios, I'm not sure there is any one
    best possible solution. Any way you slice it in a problem domain such as this,
    you are evaluating permutations and combinations for best possible fit. I'm not
    sure there is one best answer on the other side of that equation, but there are
    several, indeed could be potentially many, plausible answers, of one degree or
    strength or another. */

    //Remember zero (0) is the application name.
    for (auto i = 1; i < argc; i++) {
        //sched::scenario<best_match_scheduler, std_cout_streamer>(argv[i]).run();
        //sched::scenario<best_match_scheduler, file_stream_streamer>(argv[i]).run();
        //sched::scenario<evaluation_scheduler, file_stream_streamer>(argv[i]).run();
        //sched::scenario<candidate_scheduler, file_stream_streamer>(argv[i]).run();
        sched::scenario<mapping_scheduler, file_stream_streamer>(argv[i]).run();
    }

    return 0;
}
