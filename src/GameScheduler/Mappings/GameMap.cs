using GameScheduler.Model;

namespace GameScheduler.Mappings
{
    public class GameMap : ScheduledMap<Game>
    {
        public GameMap()
        {
            HasMany(x => x.Assigned)
                .Inverse()
                .KeyColumns.Add(@"GameId")
                .Cascade.AllDeleteOrphan()
                .LazyLoad();

            HasMany(x => x.Preferences)
                .Inverse()
                .KeyColumns.Add(@"GameId")
                .Cascade.AllDeleteOrphan()
                .LazyLoad();
        }
    }
}
