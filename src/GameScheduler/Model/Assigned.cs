namespace GameScheduler.Model
{
    public class Assigned : EntityBase<Assigned>
    {
        public virtual Game Game { get; set; }

        public virtual Player Player { get; set; }
    }
}
