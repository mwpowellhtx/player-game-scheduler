using System;

namespace GameScheduler.Model
{
    public abstract class EntityBase<T>
        where T : EntityBase<T>
    {
        public static Func<T, bool> IsTransient = e => e.Id == 0;

        public virtual long Id { get; set; }
    }
}
