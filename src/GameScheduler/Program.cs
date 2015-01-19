using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reactive.Linq;
using System.Threading.Tasks;
using GameScheduler.Data;
using GameScheduler.Extensions;
using GameScheduler.Model;

namespace GameScheduler
{
    class Program
    {
        private static Stream GetResourceStream(string arg)
        {
            var type = typeof (Repository);
            var path = string.Format(@"{0}.{1}", type.Namespace, arg);
            return type.Assembly.GetManifestResourceStream(path);
        }

        private static Task<DataContext> LoadDataContext(string arg)
        {
            var loader = new Task<DataContext>(() =>
            {
                var lines = new List<string>();

                using (var rs = GetResourceStream(arg))
                {
                    using (var sr = new StreamReader(rs))
                    {
                        string line;
                        while ((line = sr.ReadLineAsync().Result) != null)
                            lines.Add(line);
                    }
                }

                var games = lines.Select(DataContext.ParseGame)
                    .Where(g => g != null).ToList();

                var players = lines.Select(l => DataContext.ParsePlayer(l, games))
                    .Where(p => p != null).ToList();

                var dc = new DataContext {Name = arg};

                foreach(var g in games)
                    dc.InternalGames.Add(g);

                foreach (var p in players)
                    dc.InternalPlayers.Add(p);

                return dc;
            });
            loader.Start();
            return loader;
        }

        private static Task SaveDataContext(DataContext context, string arg)
        {
            var saver = new Task(() =>
            {
                var path = string.Format(@"{0}.out",
                    Path.GetFileNameWithoutExtension(arg));

                using (var fs = new FileStream(path,
                    FileMode.Create, FileAccess.Write, FileShare.Read))
                {
                    using (var sw = new StreamWriter(fs))
                    {
                        context.Games.OrderByDescending(g => g.NameId)
                            .ToList().ForEach(g => g.ReportAssigned(sw));
                    }
                }
            });
            saver.Start();
            return saver;
        }

        //private static void PlacePlayerCombinationsWithGame(Game game,
        //    int size, IEnumerable<Player> players)
        //{
        //    //TODO: wow the scale definitely blows up!
        //    //var estimated = players.EstimatedCombinations(size);

        //    IEnumerable<Player> chosen = null;

        //    //TODO: potentially an application for Reactive Extensions, observables...
        //    players.CombinationsIterative(size).ToObservable()
        //        .Aggregate((IEnumerable<Player>) null, (current, ps) =>
        //        {
        //            if (current == null) return ps;

        //            if (game.GetPotentialValue(current) < game.GetPotentialValue(ps))
        //                return ps;

        //            return current.Max(_ => game*_) < ps.Max(_ => game*_)
        //                ? ps
        //                : current;
        //        }).Subscribe(p => chosen = p);

        //    if (chosen == null || !chosen.Any()) return;

        //    foreach (var p in chosen)
        //        game.InternalAssigned.Add(new Assigned {Player = p});
        //}

        private static void PlacePlayersFifoWithGame(Game game,
            int size, IEnumerable<Player> players)
        {
            var chosen = players.OrderByDescending(p => game*p).Take(size);

            chosen.Select(p => new Assigned {Player = p})
                .ToList().ForEach(game.InternalAssigned.Add);
        }

        private static void PlacePlayersWithGame(Game game,
            int size, IEnumerable<Player> players)
        {
            //TODO: there are a few that are unassigned afterwards...
            PlacePlayersFifoWithGame(game, size, players);
        }

        static void Main(string[] args)
        {
            foreach (var arg in args)
            {
                DatabaseManager.Transaction((w, r) =>
                {
                    //Seed the data domain with some workable data but only if necessary.
                    var context = r.GetAll<DataContext>(dc => dc.Name == arg)
                        .SingleOrDefault();

                    if (context != null) return;

                    context = LoadDataContext(arg).Result;

                    r.InsertOrUpdateItem(context);
                });

                DatabaseManager.Transaction((w, r) =>
                {
                    var context = r.GetAll<DataContext>(dc => dc.Name == arg)
                        .SingleOrDefault();

                    //TODO: may put some guards in to verify counts are divisible.
                    var size = context.Players.Count/context.Games.Count;

                    var unassigned = context.Players
                        .Where(p => p.Assignment == null).ToArray();

                    foreach (var game in context.Games
                        .Where(g => !g.Assigned.Any()))
                    {
                        unassigned = unassigned
                            .Where(p => p.Assignment == null).ToArray();

                        //TODO: TBD: I wonder if I should really just order the candidates and take them that way...
                        var players = unassigned
                            .Where(p => p.Preferences.Any(
                                pref => pref.Game.Name == game.Name)).ToArray();

                        //Use the selected players if possible, or the unassigned if necessary.
                        PlacePlayersWithGame(game, size,
                            players.Length < size
                                ? unassigned
                                : players);
                    }
                });

                DatabaseManager.Transaction((w, r) =>
                {
                    var context = r.GetAll<DataContext>(dc => dc.Name == arg)
                        .SingleOrDefault();

                    SaveDataContext(context, arg).Wait();
                });
            }
        }
    }
}
