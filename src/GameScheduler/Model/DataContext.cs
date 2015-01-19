using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
using GameScheduler.Collections.Generic;

namespace GameScheduler.Model
{
    public class DataContext : EntityBase<DataContext>
    {
        public virtual string Name { get; set; }

        private IList<Game> _games = new List<Game>();

        public virtual IList<Game> Games
        {
            get { return _games; }
            set { _games = value; }
        }

        public virtual IList<Game> InternalGames
        {
            get
            {
                return Games.ToBidirectionalList(
                    added => added.DataContext = this,
                    removed => removed.DataContext = null);
            }
        } 

        private IList<Player> _players = new List<Player>();

        public virtual IList<Player> Players
        {
            get { return _players; }
            set { _players = value; }
        }

        public virtual IList<Player> InternalPlayers
        {
            get
            {
                return Players.ToBidirectionalList(
                    added => added.DataContext = this,
                    removed => removed.DataContext = null);
            }
        } 

        private const string GamePattern = @"^G (?<name>G\d+) H:(?<h>\d+) E:(?<e>\d+) P:(?<p>\d+)?$";

        private const string PlayerPattern = @"^P (?<name>P\d+) H:(?<h>\d+) E:(?<e>\d+) P:(?<p>\d+)( (?<prefs>G\d+(,G\d+)*))?$";

        private static readonly Regex GamePatternRegex = new Regex(GamePattern, RegexOptions.Compiled);

        private static readonly Regex PlayerPatternRegex = new Regex(PlayerPattern, RegexOptions.Compiled);

        private static bool TryParse<T>(string line, Regex pattern,
            out T scheduled, IDictionary<string, Action<T, string>> elements)
            where T : Scheduled<T>, new()
        {
            scheduled = null;

            var match = pattern.Match(line);

            if (match.Success)
            {
                scheduled = new T();

                var groups = match.Groups;

                foreach (var e in elements)
                    e.Value(scheduled, groups[e.Key].Value);
            }

            return scheduled != null;
        }

        public static Game ParseGame(string line)
        {
            Game game;

            var result = TryParse(line, GamePatternRegex, out game,
                new Dictionary<string, Action<Game, string>>
                {
                    {@"name", (c, s) => c.Name = s},
                    {@"h", (c, s) => c.H = int.Parse(s)},
                    {@"e", (c, s) => c.E = int.Parse(s)},
                    {@"p", (c, s) => c.P = int.Parse(s)},
                });

            return game;
        }

        public static Player ParsePlayer(string line, IEnumerable<Game> games)
        {
            Player player;

            var result = TryParse(line, PlayerPatternRegex, out player,
                new Dictionary<string, Action<Player, string>>
                {
                    {@"name", (j, s) => j.Name = s},
                    {@"h", (j, s) => j.H = int.Parse(s)},
                    {@"e", (j, s) => j.E = int.Parse(s)},
                    {@"p", (j, s) => j.P = int.Parse(s)},
                    {
                        @"prefs", (j, s) => games.Where(c => s.Contains(c.Name))
                            .Select(c => new Preference {Game = c})
                            .ToList().ForEach(j.InternalPreferences.Add)
                    },
                });

            return player;
        }
    }
}
