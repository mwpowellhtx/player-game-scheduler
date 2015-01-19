using FluentNHibernate.Mapping;
using GameScheduler.Model;

namespace GameScheduler.Mappings
{
    public abstract class ScheduledMap<T> : ClassMap<T>
        where T : Scheduled<T>
    {
        protected ScheduledMap()
        {
            Id(x => x.Id, @"Id");

            References(x => x.DataContext)
                .Column(@"DataContextId")
                .Nullable();

            Map(x => x.Name).Not.Nullable().Length(24);
            Map(x => x.NameId).Not.Nullable();
            Map(x => x.H).Not.Nullable();
            Map(x => x.E).Not.Nullable();
            Map(x => x.P).Not.Nullable();
        }
    }
}
