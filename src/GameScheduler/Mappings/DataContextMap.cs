using FluentNHibernate.Mapping;
using GameScheduler.Model;

namespace GameScheduler.Mappings
{
    public class DataContextMap : ClassMap<DataContext>
    {
        public DataContextMap()
        {
            Id(x => x.Id).Column(@"Id");

            Map(x => x.Name).Not.Nullable().Length(127);

            HasMany(x => x.Games)
                .Inverse()
                .Table(@"Game")
                .KeyColumns.Add(@"DataContextId")
                .Cascade.AllDeleteOrphan()
                .LazyLoad();

            HasMany(x => x.Players)
                .Inverse()
                .Table(@"Player")
                .KeyColumns.Add(@"DataContextId")
                .Cascade.AllDeleteOrphan()
                .LazyLoad();
        }
    }
}
