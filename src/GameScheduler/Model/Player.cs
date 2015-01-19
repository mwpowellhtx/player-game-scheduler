using System.Collections.Generic;
using System.IO;
using System.Linq;
using GameScheduler.Collections.Generic;

namespace GameScheduler.Model
{
    public class Player : Scheduled<Player>, IAssignedReporter
    {
        private IList<Preference> _preferences = new List<Preference>();

        public virtual IList<Preference> Preferences
        {
            get { return _preferences; }
            set { _preferences = value; }
        }

        public virtual IList<Preference> InternalPreferences
        {
            get
            {
                return Preferences.ToBidirectionalList(
                    added => added.Player = this,
                    removed => removed.Player = null);
            }
        }

        private Assigned _assignment;

        public virtual Assigned Assignment
        {
            get { return _assignment; }
            set
            {
                if (_assignment != null)
                    _assignment.Player = null;

                if ((_assignment = value) != null)
                    _assignment.Player = this;
            }
        }

        public override void Report(TextWriter writer)
        {
            var prefs = string.Join(@",", Preferences.Select(pref => pref.Game.Name));
            writer.WriteLine(@"P {0} H:{1} E:{2} P:{3} {4}", Name, H, E, P, prefs);
        }

        public virtual void ReportAssigned(TextWriter writer)
        {
            writer.Write(@" {0}", Name);

            foreach (var pref in Preferences)
                writer.Write(@" {0}:{1}", pref.Game.Name, pref.Game*this);
        }
    }
}
