using GameScheduler.Model;

namespace GameScheduler.Mappings
{
    public class PlayerMap : ScheduledMap<Player>
    {
        public PlayerMap()
        {
            HasMany(x => x.Preferences)
                .Inverse()
                .Table(@"Preference")
                .KeyColumns.Add(@"PlayerId")
                .Cascade.AllDeleteOrphan()
                .LazyLoad();

            HasOne(x => x.Assignment)
                .PropertyRef(r => r.Player)
                .Cascade.All()
                .LazyLoad();
        }
    }
}
