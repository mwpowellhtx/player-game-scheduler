using System.IO;
using System.Linq;

namespace GameScheduler.Model
{
    public abstract class Scheduled<T> : EntityBase<T>, IScheduleReporter
        where T : Scheduled<T>
    {
        public virtual DataContext DataContext { get; set; }

        public virtual string Name { get; set; }

        /// <summary>
        /// Gets the NameId based on the Name.
        /// </summary>
        /// <see cref="Name"/>
        public virtual int NameId
        {
            get
            {
                if (string.IsNullOrEmpty(Name) || Name.Length < 2) return 0;
                return int.Parse(Name.Skip(1).Aggregate(string.Empty, (s, c) => s + c));
            }
            set { }
        }

        public virtual int H { get; set; }

        public virtual int E { get; set; }

        public virtual int P { get; set; }

        public abstract void Report(TextWriter writer);
    }
}
