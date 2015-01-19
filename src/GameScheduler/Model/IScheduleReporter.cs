using System.IO;

namespace GameScheduler.Model
{
    public interface IScheduleReporter
    {
        void Report(TextWriter writer);
    }
}
