namespace GameScheduler.Model
{
    public class Preference : EntityBase<Preference>
    {
        public virtual Player Player { get; set; }

        public virtual Game Game { get; set; }
    }
}
