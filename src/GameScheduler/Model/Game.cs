using System.Collections.Generic;
using System.IO;
using System.Linq;
using GameScheduler.Collections.Generic;

namespace GameScheduler.Model
{
    public class Game : Scheduled<Game>, IAssignedReporter
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
                    added => added.Game = this,
                    removed => removed.Game = null);
            }
        }

        private IList<Assigned> _assigned = new List<Assigned>();

        public virtual IList<Assigned> Assigned
        {
            get { return _assigned; }
            set { _assigned = value; }
        }

        public virtual IList<Assigned> InternalAssigned
        {
            get
            {
                return Assigned.ToBidirectionalList(
                    added =>
                    {
                        added.Game = this;
                        added.Player.Assignment = added;
                    },
                    removed =>
                    {
                        removed.Game = null;
                        removed.Player.Assignment = null;
                    });
            }
        }

        private int DotProduct(Player player)
        {
            return new[]
            {
                H*player.H,
                E*player.E,
                P*player.P,
            }.Sum();
        }

        public static int operator *(Game game, Player player)
        {
            return game.DotProduct(player);
        }

        /// <summary>
        /// Gets the ActualValue across <see cref="Model.Assigned.Player"/>
        /// entries.
        /// </summary>
        public virtual int ActualValue
        {
            get { return Assigned.Select(a => this*a.Player).Sum(); }
        }

        public virtual int GetPotentialValue(IEnumerable<Player> players)
        {
            return players.Select(p => this*p).Sum();
        }

        public override void Report(TextWriter writer)
        {
            writer.WriteLine(@"G {0} H:{1} E:{2} P:{3}",
                Name, H, E, P);
        }

        public virtual void ReportAssigned(TextWriter writer)
        {
            writer.Write(Name);

            var delimit = false;
            var assigned = Assigned.OrderByDescending(a => this*a.Player);

            foreach (var a in assigned)
            {
                if (delimit) writer.Write(@",");
                a.Player.ReportAssigned(writer);
                if (!delimit) delimit = true;
            }

            writer.WriteLine();
        }
    }
}
