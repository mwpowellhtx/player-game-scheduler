using System.IO;

namespace GameScheduler.Model
{
    public interface IAssignedReporter
    {
        void ReportAssigned(TextWriter writer);
    }
}
