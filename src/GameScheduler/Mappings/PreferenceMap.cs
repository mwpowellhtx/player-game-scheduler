using FluentNHibernate.Mapping;
using GameScheduler.Model;

namespace GameScheduler.Mappings
{
    public class PreferenceMap : ClassMap<Preference>
    {
        public PreferenceMap()
        {
            Id(x => x.Id)
                .Column(@"Id");

            References(x => x.Player)
                .Column(@"PlayerId")
                .Nullable().LazyLoad();

            References(x => x.Game)
                .Column(@"GameId")
                .Nullable().LazyLoad();
        }
    }
}
