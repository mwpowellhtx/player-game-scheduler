using FluentNHibernate.Mapping;
using GameScheduler.Model;

namespace GameScheduler.Mappings
{
    public class AssignedMap : ClassMap<Assigned>
    {
        public AssignedMap()
        {
            Id(x => x.Id)
                .Column(@"Id");

            References(x => x.Game)
                .Column(@"GameId")
                .Nullable().LazyLoad();

            References(x => x.Player)
                .Column(@"PlayerId")
                .Nullable().LazyLoad();
        }
    }
}
