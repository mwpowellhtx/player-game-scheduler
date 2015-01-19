using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using NHibernate;
using NHibernate.Linq;

namespace GameScheduler.Data
{
    public class Repository : IDisposable
    {
        public Repository(ISession session)
        {
            _session = session;
        }

        private readonly ISession _session;

        public IQueryable<T> Query<T>()
        {
            return _session.Query<T>();
        }

        public IEnumerable<T> GetAll<T>(Func<T, bool> predicate = null)
        {
            predicate = predicate ?? (x => true);
            return Query<T>().AsEnumerable().Where(predicate);
        }

        public void DeleteItem<T>(T item)
            where T : class
        {
            DeleteItems(new[] { item });
        }

        public void DeleteItems<T>(IEnumerable<T> items)
            where T : class
        {
            try
            {
                foreach (var item in items.Where(x => !ReferenceEquals(null, x)))
                    _session.Delete(item);
            }
            catch (Exception)
            {
                //TODO: danger: swallowed exception
                throw;
            }
        }

        public void InsertOrUpdateItem<T>(T items)
            where T : class
        {
            InsertOrUpdateItems(new[] {items});
        }

        public void InsertOrUpdateItems<T>(IEnumerable<T> items)
            where T : class
        {
            //TODO: may need/want to expose more of the DbSet, or even DbEntityEntry, details
            try
            {
                foreach (var item in items.Where(x => !ReferenceEquals(null, x)))
                    _session.SaveOrUpdate(item);
            }
            catch (Exception)
            {
                //TODO: danger: swallowed exception
                throw;
            }
        }

        public IUnitOfWork Work(
            IsolationLevel isolationLevel = IsolationLevel.ReadCommitted)
        {
            return new UnitOfWork(_session);
        }

        public void Dispose()
        {
            _session.Dispose();
        }
    }
}
